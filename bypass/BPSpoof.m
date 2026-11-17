#import "BPSpoof.h"
#import "fishhook.h"
#import <CommonCrypto/CommonCrypto.h>
#import <sys/fcntl.h>
#import <sys/stat.h>
#import <unistd.h>
#import <string.h>
#import <stdlib.h>
#import <stdarg.h>

// ---------------------------------------------------------
// 1. HASH SPOOFING (CC_SHA256)
// ---------------------------------------------------------
static unsigned char *(*orig_CC_SHA256)(const void *data, CC_LONG len, unsigned char *md);

// Replace these bytes with the CLEAN UnityFramework SHA-256 hash!
static const unsigned char clean_sha256[CC_SHA256_DIGEST_LENGTH] = { 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static unsigned char *fake_CC_SHA256(const void *data, CC_LONG len, unsigned char *md) {
    unsigned char *result = orig_CC_SHA256(data, len, md);
    
    // If the data is large enough to be UnityFramework (usually >10MB)
    if (data && len > 10000000) {
        uint32_t *magic = (uint32_t *)data;
        // Mach-O 64-bit magic (0xFEEDFACF) or 32-bit (0xFEEDFACE)
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
    // XOR or obfuscate this string if needed
    return (strstr(path, "UnityFramework") != NULL);
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
    mode_t mode = va_arg(args, mode_t);
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
