#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include "paths.h"
#include "anti_crash.h"
#include "anti_scan.h"

// --- Fishhook Definitions ---
struct rebinding { const char *name; void *replacement; void **replaced; };
struct rb_entry { struct rebinding *r; size_t n; struct rb_entry *next; };
static struct rb_entry *_head;
static int prepend(struct rb_entry **h, struct rebinding r[], size_t n){
    struct rb_entry *e=(struct rb_entry *)malloc(sizeof(*e)); if(!e)return -1;
    e->r=(struct rebinding *)malloc(sizeof(struct rebinding)*n); if(!e->r){free(e);return -1;}
    memcpy(e->r,r,sizeof(struct rebinding)*n); e->n=n; e->next=*h; *h=e; return 0;
}
static void rebind_sec(struct rb_entry *e, struct mach_header_64 *h, intptr_t s,
                       struct section_64 *sec, struct symtab_command *st, char *str){
    uint32_t *ind=(uint32_t*)((char*)h+sec->offset+s);
    struct nlist_64 *sy=(struct nlist_64*)((char*)h+st->symoff+s);
    for(uint32_t i=0;i<sec->size/sizeof(uint32_t);i++){
        uint32_t ix=ind[i]; if(!ix)continue;
        struct nlist_64 *nl=&sy[ix]; if((nl->n_type&N_TYPE)!=N_UNDF)continue;
        char *nm=str+nl->n_un.n_strx; if(*nm=='_')nm++;
        for(size_t j=0;j<e->n;j++){
            if(strcmp(nm,e->r[j].name)==0){
                if(e->r[j].replaced && *e->r[j].replaced==NULL)
                    *e->r[j].replaced=(void*)((uintptr_t)ind[i]+s);
                ind[i]=(uint32_t)((uintptr_t)e->r[j].replacement-s);
            }
        }
    }
}
static void rebind_all(struct rb_entry *e){
    uint32_t c=_dyld_image_count();
    for(uint32_t im=0;im<c;im++){
        struct mach_header_64 *h=(struct mach_header_64*)_dyld_get_image_header(im);
        if(!h||h->magic!=MH_MAGIC_64)continue;
        intptr_t s=_dyld_get_image_vmaddr_slide(im);
        struct symtab_command *st=NULL; char *str=NULL;
        intptr_t cur=(intptr_t)h+sizeof(struct mach_header_64);
        for(uint32_t k=0;k<h->ncmds;k++){
            struct load_command *lc=(struct load_command*)cur;
            if(lc->cmd==LC_SYMTAB){st=(struct symtab_command*)lc;str=(char*)((char*)h+st->stroff+s);}
            cur+=lc->cmdsize;
        }
        if(!st||!str)continue;
        intptr_t sc=(intptr_t)h+sizeof(struct mach_header_64);
        for(uint32_t k=0;k<h->ncmds;k++){
            struct load_command *lc=(struct load_command*)sc;
            if(lc->cmd==LC_SEGMENT_64){
                struct segment_command_64 *sg=(struct segment_command_64*)lc;
                struct section_64 *ss=(struct section_64*)(sg+1);
                for(uint32_t si=0;si<sg->nsects;si++){
                    uint32_t t=ss[si].flags & SECTION_TYPE;
                    if(t==S_LAZY_SYMBOL_POINTERS || t==S_NON_LAZY_SYMBOL_POINTERS)
                        rebind_sec(e,h,s,&ss[si],st,str);
                }
            }
            sc+=lc->cmdsize;
        }
    }
}
static int fishhook(struct rebinding r[], size_t n){
    if(prepend(&_head,r,n)!=0)return -1;
    rebind_all(_head); return 0;
}
// ----------------------------

#define PT_TRACE_ME 0
#define PT_ATTACH   10
#define CS_OPS_STATUS 0
#define CS_DEBUGGED 0x10000000
#define CS_HARD     0x00000100
#define CS_RESTRICT 0x00000800

static int (*o_access)(const char *, int);
static int (*o_stat)(const char *, struct stat *);
static int (*o_lstat)(const char *, struct stat *);
static int (*o_open)(const char *, int, ...);
static int (*o_openat)(int, const char *, int, ...);
static int (*o_faccessat)(int, const char *, int, int);
static pid_t (*o_fork)(void);
static int (*o_csops)(pid_t, uint32_t, void *, size_t);
static int (*o_sysctl)(int *, u_int, void *, size_t *, void *, size_t);
typedef long (*ptrace_t)(int, pid_t, caddr_t, int);
static ptrace_t o_ptrace;

static int r_access(const char *p, int m){ if(path_is_hidden(p)){errno=ENOENT;return -1;} return o_access(p,m); }
static int r_stat(const char *p, struct stat *b){ if(path_is_hidden(p)){errno=ENOENT;return -1;} return o_stat(p,b); }
static int r_lstat(const char *p, struct stat *b){ if(path_is_hidden(p)){errno=ENOENT;return -1;} return o_lstat(p,b); }
static int r_open(const char *p, int f, ...){
    mode_t m=0; if(f&O_CREAT){va_list a;va_start(a,f);m=(mode_t)va_arg(a,int);va_end(a);}
    if(path_is_hidden(p)){errno=ENOENT;return -1;} return o_open(p,f,m);
}
static int r_openat(int d, const char *p, int f, ...){
    mode_t m=0; if(f&O_CREAT){va_list a;va_start(a,f);m=(mode_t)va_arg(a,int);va_end(a);}
    if(path_is_hidden(p)){errno=ENOENT;return -1;} return o_openat(d,p,f,m);
}
static int r_faccessat(int d, const char *p, int m, int fl){ if(path_is_hidden(p)){errno=ENOENT;return -1;} return o_faccessat(d,p,m,fl); }
static pid_t r_fork(void){ errno=EAGAIN; return -1; }

static int r_csops(pid_t pid, uint32_t ops, void *u, size_t s){
    int r=o_csops(pid,ops,u,s);
    if(r==0 && ops==CS_OPS_STATUS && u && s>=4){
        uint32_t *f=(uint32_t*)u; *f&=~CS_DEBUGGED; *f&=~CS_HARD; *f&=~CS_RESTRICT;
    }
    return r;
}
static int r_sysctl(int *n, u_int nl, void *o, size_t *ol, void *nw, size_t nwl){
    int r=o_sysctl(n,nl,o,ol,nw,nwl);
    if(r==0 && n && nl>=4 && n[0]==CTL_KERN && n[1]==KERN_PROC && o && ol && *ol>=sizeof(struct kinfo_proc)){
        struct kinfo_proc *kp=(struct kinfo_proc*)o; kp->kp_proc.p_flag &= ~P_TRACED;
    }
    return r;
}
static long r_ptrace(int req, pid_t pid, caddr_t a, int d){
    if(req==PT_ATTACH || req==PT_TRACE_ME){ errno=EPERM; return -1; }
    return o_ptrace(req,pid,a,d);
}

void acbypass_install_hooks(void) {
    paths_init();
    anti_crash_init();
    anti_scan_init();
    
    struct rebinding r[] = {
        {"access",    (void*)r_access,    (void**)&o_access},
        {"stat",      (void*)r_stat,      (void**)&o_stat},
        {"lstat",     (void*)r_lstat,     (void**)&o_lstat},
        {"open",      (void*)r_open,      (void**)&o_open},
        {"openat",    (void*)r_openat,    (void**)&o_openat},
        {"faccessat", (void*)r_faccessat, (void**)&o_faccessat},
        {"fork",      (void*)r_fork,      (void**)&o_fork},
        {"sysctl",    (void*)r_sysctl,    (void**)&o_sysctl},
        {"csops",     (void*)r_csops,     (void**)&o_csops},
        {"ptrace",    (void*)r_ptrace,    (void**)&o_ptrace},
    };
    fishhook(r, sizeof(r)/sizeof(r[0]));
}
