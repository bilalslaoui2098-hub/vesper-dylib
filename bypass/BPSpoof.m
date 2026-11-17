#import "bypass/BPSpoof.h"
#import "fishhook.h"
#import <CommonCrypto/CommonCrypto.h>
#import <sys/fcntl.h>
#import <sys/stat.h>
#import <sys/sysctl.h>
#import <unistd.h>
#import <string.h>
#import <stdlib.h>
#import <stdarg.h>
#import <dlfcn.h>
#import <mach-o/dyld.h>
#import <mach/task.h>
#import <mach/mach_init.h>
#import <mach/mach_traps.h>

// ---------------------------------------------------------
// 1. HASH SPOOFING (CC_SHA256)
// ---------------------------------------------------------
static unsigned char *(*orig_CC_SHA256)(const void *data, CC_LONG len, unsigned char *md);

static const unsigned char clean_sha256[CC_SHA256_DIGEST_LENGTH] = { 
    0x2D, 0xE0, 0x05, 0xAE, 0x6C, 0x9D, 0x26, 0x3D, 
    0xDD, 0x70, 0x5A, 0xED, 0xDA, 0x83, 0x3C, 0x29, 
    0xA5, 0xB2, 0x70, 0x44, 0xD2, 0xF5, 0x66, 0x55, 
    0xEC, 0xC3, 0xCF, 0x98, 0xDC, 0x7B, 0xE5, 0x4A 
};

static unsigned char *fake_CC_SHA256(const void *data, CC_LONG len, unsigned char *md) {
    unsigned char *result = orig_CC_SHA256(data, len, md);
    
    if (data && len > 10000000) {
        uint32_t *magic = (uint32_t *)data;
        if (*magic == 0xFEEDFACF || *magic == 0xFEEDFACE) {
            memcpy(md, clean_sha256, CC_SHA256_DIGEST_LENGTH);
            return md;
        }
    }
    return result;
}

void apply_sha256_hook(void) {
    struct rebinding r[] = {
        {"CC_SHA256", fake_CC_SHA256, (void **)&orig_CC_SHA256},
    };
    rebind_symbols(r, 1);
}

// ---------------------------------------------------------
// 2. FILE REDIRECT HOOKS (Pure C)
// ---------------------------------------------------------
static int (*orig_open)(const char *, int, ...);
static FILE *(*orig_fopen)(const char *, const char *);
static int (*orig_stat)(const char *, struct stat *);
static int (*orig_access)(const char *, int);

static char clean_framework_path[1024] = {0};

static int is_target_path(const char *path) {
    if (!path) return 0;
    unsigned char xor_str[] = {0x0B, 0x25, 0x2D, 0x27, 0x20, 0x1C, 0x25, 0x3D, 0x36, 0x2C, 0x20, 0x3C, 0x25, 0x0B, 0x00};
    char target[32];
    for(int i=0; i<15; i++) target[i] = xor_str[i] ^ 0x55;
    return (strstr(path, target) != NULL);
}

void setup_clean_path() {
    const char *home = getenv("HOME");
    if (home) {
        snprintf(clean_framework_path, sizeof(clean_framework_path), "%s/Documents/clean_framework", home);
    }
}

static int bp_open(const char *path, int oflag, ...) {
    if (is_target_path(path) && clean_framework_path[0] != '\0') {
        return orig_open(clean_framework_path, oflag);
    }
    va_list args;
    va_start(args, oflag);
    mode_t mode = va_arg(args, int);
    va_end(args);
    return orig_open(path, oflag, mode);
}

static FILE *bp_fopen(const char *path, const char *mode) {
    if (is_target_path(path) && clean_framework_path[0] != '\0') {
        return orig_fopen(clean_framework_path, mode);
    }
    return orig_fopen(path, mode);
}

static int bp_stat(const char *path, struct stat *buf) {
    if (is_target_path(path) && clean_framework_path[0] != '\0') {
        return orig_stat(clean_framework_path, buf);
    }
    return orig_stat(path, buf);
}

static int bp_access(const char *path, int amode) {
    if (is_target_path(path) && clean_framework_path[0] != '\0') {
        return orig_access(clean_framework_path, amode);
    }
    return orig_access(path, amode);
}

void apply_file_redirect_hooks(void) {
    setup_clean_path();
    struct rebinding r[] = {
        {"open", bp_open, (void **)&orig_open},
        {"fopen", bp_fopen, (void **)&orig_fopen},
        {"stat", bp_stat, (void **)&orig_stat},
        {"access", bp_access, (void **)&orig_access},
    };
    rebind_symbols(r, sizeof(r) / sizeof(r[0]));
}

// ---------------------------------------------------------
// 3. ANTI-DEBUG & ANTI-JAILBREAK HOOKS
// ---------------------------------------------------------
static int (*orig_ptrace)(int request, pid_t pid, caddr_t addr, int data);
static int (*orig_sysctl)(int *name, u_int namelen, void *oldp, size_t *oldlenp, void *newp, size_t newlen);
static int (*orig_sysctlbyname)(const char *name, void *oldp, size_t *oldlenp, void *newp, size_t newlen);
static int (*orig_fork)(void);
static const char *(*orig_dyld_get_image_name)(uint32_t index);
static const struct mach_header_64 *(*orig_dyld_get_image_header)(uint32_t index);
static uint32_t (*orig_dyld_image_count)(void);
static kern_return_t (*orig_task_get_exception_ports)(task_t task, exception_mask_t mask, exception_mask_array_t masks, mach_msg_type_number_t *masksCnt, exception_handler_array_t old_handlers, exception_behavior_array_t old_behaviors, exception_flavor_array_t old_flavors);
static char *(*orig_getenv)(const char *name);

static int bp_ptrace(int request, pid_t pid, caddr_t addr, int data) {
    if (request == 31 || request == 0) return 0; 
    return orig_ptrace(request, pid, addr, data);
}

static int bp_sysctl(int *name, u_int namelen, void *oldp, size_t *oldlenp, void *newp, size_t newlen) {
    if (namelen >= 2 && name[0] == CTL_KERN && name[1] == KERN_PROC) {
        if (oldp && oldlenp) memset(oldp, 0, *oldlenp);
        return 0;
    }
    return orig_sysctl(name, namelen, oldp, oldlenp, newp, newlen);
}

static int bp_sysctlbyname(const char *name, void *oldp, size_t *oldlenp, void *newp, size_t newlen) {
    if (name && (strstr(name, "kern.proc") || strstr(name, "kern.argmax"))) {
        if (oldp && oldlenp) memset(oldp, 0, *oldlenp);
        return 0;
    }
    return orig_sysctlbyname(name, oldp, oldlenp, newp, newlen);
}

static int bp_fork(void) {
    return -1; 
}

static const char *bp_dyld_get_image_name(uint32_t index) {
    const char *name = orig_dyld_get_image_name(index);
    if (!name) return name;
    
    if (strstr(name, "bypass") || strstr(name, "BPSpoof") || 
        strstr(name, "Substrate") || strstr(name, "ElleKit") || 
        strstr(name, "fishhook") || strstr(name, "vesper")) {
        return "/usr/lib/libsystem_c.dylib"; 
    }
    return name;
}

static uint32_t bp_dyld_image_count(void) {
    uint32_t count = orig_dyld_image_count();
    return count > 0 ? count - 1 : 0; 
}

static const struct mach_header_64 *bp_dyld_get_image_header(uint32_t index) {
    const struct mach_header_64 *header = orig_dyld_get_image_header(index);
    if (header) {
        const char *name = orig_dyld_get_image_name(index);
        if (name && (strstr(name, "vesper") || strstr(name, "BPSpoof") || strstr(name, "Substrate"))) {
            return NULL; 
        }
    }
    return header;
}

static kern_return_t bp_task_get_exception_ports(task_t task, exception_mask_t mask, exception_mask_array_t masks, mach_msg_type_number_t *masksCnt, exception_handler_array_t old_handlers, exception_behavior_array_t old_behaviors, exception_flavor_array_t old_flavors) {
    kern_return_t kr = orig_task_get_exception_ports(task, mask, masks, masksCnt, old_handlers, old_behaviors, old_flavors);
    if (kr == KERN_SUCCESS && masksCnt) {
        *masksCnt = 0; 
    }
    return kr;
}

static char *bp_getenv(const char *name) {
    if (name && (strcmp(name, "DYLD_INSERT_LIBRARIES") == 0 || strcmp(name, "DYLD_PRINT_STATISTICS") == 0 || strcmp(name, "DYLD_PRINT_LIBRARIES") == 0)) {
        return NULL; 
    }
    return orig_getenv(name);
}

void apply_anti_debug_hooks(void) {
    struct rebinding r[] = {
        {"ptrace", bp_ptrace, (void **)&orig_ptrace},
        {"sysctl", bp_sysctl, (void **)&orig_sysctl},
        {"sysctlbyname", bp_sysctlbyname, (void **)&orig_sysctlbyname},
        {"fork", bp_fork, (void **)&orig_fork},
        {"_dyld_get_image_name", bp_dyld_get_image_name, (void **)&orig_dyld_get_image_name},
        {"_dyld_image_count", bp_dyld_image_count, (void **)&orig_dyld_image_count},
        {"_dyld_get_image_header", bp_dyld_get_image_header, (void **)&orig_dyld_get_image_header},
        {"task_get_exception_ports", bp_task_get_exception_ports, (void **)&orig_task_get_exception_ports},
        {"getenv", bp_getenv, (void **)&orig_getenv},
    };
    rebind_symbols(r, sizeof(r) / sizeof(r[0]));
}
