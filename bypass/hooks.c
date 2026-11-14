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
#include <substrate.h> 
#include "paths.h"
#include "anti_crash.h"
#include "anti_scan.h"

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
    
    void *a;
    a = dlsym(RTLD_DEFAULT, "access");    if(a) MSHookFunction(a, (void*)r_access, (void**)&o_access);
    a = dlsym(RTLD_DEFAULT, "stat");      if(a) MSHookFunction(a, (void*)r_stat, (void**)&o_stat);
    a = dlsym(RTLD_DEFAULT, "lstat");     if(a) MSHookFunction(a, (void*)r_lstat, (void**)&o_lstat);
    a = dlsym(RTLD_DEFAULT, "open");      if(a) MSHookFunction(a, (void*)r_open, (void**)&o_open);
    a = dlsym(RTLD_DEFAULT, "openat");    if(a) MSHookFunction(a, (void*)r_openat, (void**)&o_openat);
    a = dlsym(RTLD_DEFAULT, "faccessat"); if(a) MSHookFunction(a, (void*)r_faccessat, (void**)&o_faccessat);
    a = dlsym(RTLD_DEFAULT, "fork");      if(a) MSHookFunction(a, (void*)r_fork, (void**)&o_fork);
    a = dlsym(RTLD_DEFAULT, "sysctl");    if(a) MSHookFunction(a, (void*)r_sysctl, (void**)&o_sysctl);
    a = dlsym(RTLD_DEFAULT, "csops");     if(a) MSHookFunction(a, (void*)r_csops, (void**)&o_csops);
    a = dlsym(RTLD_DEFAULT, "ptrace");    if(a) MSHookFunction(a, (void*)r_ptrace, (void**)&o_ptrace);
}
