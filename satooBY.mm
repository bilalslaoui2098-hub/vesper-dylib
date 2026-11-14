#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>

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

static const char *(*o_get_name)(uint32_t);
static uint32_t (*o_count)(void);
static const struct mach_header *(*o_get_header)(uint32_t);

static int is_hidden_image(const char *n){
    if(!n)return 0;
    return strstr(n,"VIG4R") || strstr(n,"NexoraFF")
        || strstr(n,"frida") || strstr(n,"ellekit")
        || strstr(n,"MobileSubstrate") || strstr(n,"libhooker");
}

static uint32_t g_first=UINT32_MAX, g_hidden=0; static int g_built=0;
static void build(void){
    if(g_built)return;
    uint32_t n=o_count(), f=UINT32_MAX, h=0;
    for(uint32_t i=0;i<n;i++){ const char *nm=o_get_name(i);
        if(is_hidden_image(nm)){ if(f==UINT32_MAX)f=i; h++; } }
    g_first=f; g_hidden=h; g_built=1;
}
static uint32_t r_count(void){ build(); uint32_t r=o_count(); return (g_hidden>0&&r>g_hidden)?r-g_hidden:r; }
static const char *r_name(uint32_t i){
    build();
    if(g_hidden>0&&i>=g_first){ uint32_t n=o_count();
        for(uint32_t j=i;j<n;j++){ const char *nm=o_get_name(j); if(!is_hidden_image(nm))return nm; } return NULL; }
    return o_get_name(i);
}
static const struct mach_header *r_hdr(uint32_t i){
    build();
    if(g_hidden>0&&i>=g_first){ uint32_t n=o_count();
        for(uint32_t j=i;j<n;j++){ const char *nm=o_get_name(j); if(!is_hidden_image(nm))return o_get_header(j); } return NULL; }
    return o_get_header(i);
}
void acbypass_install_dyld_hooks(void){
    struct rebinding r[] = {
        {"_dyld_get_image_name",   (void*)r_name,   (void**)&o_get_name},
        {"_dyld_get_image_header", (void*)r_hdr,    (void**)&o_get_header},
        {"_dyld_image_count",      (void*)r_count,  (void**)&o_count},
    };
    fishhook(r, sizeof(r)/sizeof(r[0]));
}
