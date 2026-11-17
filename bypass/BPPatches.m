#import "bypass/BPSpoof.h"
// BPPatches.m
#import "BPPatches.h"
#import "BPEngine.h"
#import "BPPatches_generated.h"
#import "fishhook.h"
#import <objc/runtime.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <sys/stat.h>
#import <sys/sysctl.h>
#import <dlfcn.h>

@implementation BPPatches

+ (void)applyAll {
    BPEngine *e = [BPEngine shared];
    NSLog(@"[BP] image: %@ base=0x%llX slide=0x%llX",
          [e mainImagePath], [e mainImageBase], [e mainImageSlide]);
    if (![e mainImageBase]) {
        NSLog(@"[BP] main image not found — abort");
        return;
    }

    [self applyGodMode:e];
    [self applyAliveState:e];
    [self applyVehicleGod:e];
    [self applyAntiCheatSilence:e];
    [self applyAimModifiers:e];
    [self applyCFunctionHooks];
    apply_sha256_hook();
    apply_file_redirect_hooks();
    apply_anti_debug_hooks();
    [self applyMethodSwizzles];
}

#pragma mark - Feature groups

+ (void)applyGodMode:(BPEngine *)e {
    // TakeDamage is void -> ret kills it
    [e patchVA:BP_FragmentAttackableComponent_TakeDamage     hex:@"C0035FD6"];
    [e patchVA:BP_BountyWaitingUAV_TakeDamage                hex:@"C0035FD6"];
    [e patchVA:BP_BountyWaitingUAV_SimpleTakeDamage          hex:@"C0035FD6"];
    [e patchVA:BP_BountyWaitingUAV_RandomTakeDamageSelf      hex:@"C0035FD6"];
    NSLog(@"[BP] god mode applied");
}

+ (void)applyAliveState:(BPEngine *)e {
    // IsLocalPlayerAlive: bool -> force true
    [e patchVA:BP_MatchGame_IsLocalPlayerAlive               hex:@"200080D2C0035FD6"];
    // OnStateEnter: void -> block death state transition
    [e patchVA:BP_PlayerAliveState_OnStateEnter              hex:@"C0035FD6"];
    NSLog(@"[BP] alive state applied");
}

+ (void)applyVehicleGod:(BPEngine *)e {
    // KillEngine is void -> ret prevents engine death
    [e patchVA:BP_RCC_CarControllerV3_KillEngine             hex:@"C0035FD6"];
    [e patchVA:BP_RMCRealisticMotorcycleController_KillEngine hex:@"C0035FD6"];
    NSLog(@"[BP] vehicle god applied");
}

+ (void)applyAntiCheatSilence:(BPEngine *)e {
    // Report* are void -> ret so nothing gets reported
    [e patchVA:BP_Debugger_ReportException                   hex:@"C0035FD6"];
    [e patchVA:BP_Debugger_ReportError                       hex:@"C0035FD6"];
    // get_IsUnused: bool -> return 0 (encoder used)
    [e patchVA:BP_MinimalEncoderCameraHook_get_IsUnused      hex:@"000080D2C0035FD6"];
    NSLog(@"[BP] anti-cheat reporting silenced");
}

+ (void)applyAimModifiers:(BPEngine *)e {
    // GetHeadShotDamageScale returns float -> fmov s0,#1.0 ; ret
    [e patchVA:BP_BigHeadDataConfig_GetHeadShotDamageScale   hex:@"1F00701EC0035FD6"];
    // GetHeadScale returns float -> fmov s0,#1.0 ; ret (neutral scale)
    [e patchVA:BP_BigHeadDataConfig_GetHeadScale             hex:@"1F00701EC0035FD6"];
    NSLog(@"[BP] aim modifiers applied");
}

#pragma mark - Bypass: ObjC swizzles

static BOOL bp_isBannedPath(NSString *path) {
    if (!path) return NO;
    static NSArray *banned;
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        banned = @[
            @"/Applications/Cydia.app", @"/Applications/Sileo.app",
            @"/Applications/Zebra.app", @"/var/jb", @"/bin/bash",
            @"/usr/sbin/sshd", @"/etc/apt", @"/private/var/lib/dpkg",
            @"/Library/MobileSubstrate", @"/usr/lib/libsubstrate.dylib",
            @"/usr/lib/libellekit.dylib", @"/usr/lib/libhooker.dylib",
            @"/usr/lib/libcycript.dylib", @"/usr/bin/cycript",
            @"/usr/lib/libfrida", @"/usr/sbin/frida-server",
        ];
    });
    for (NSString *b in banned)
        if ([path isEqualToString:b] || [path hasPrefix:b]) return YES;
    return NO;
}

+ (void)applyMethodSwizzles {
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        Class fm = [NSFileManager class];
        method_exchangeImplementations(
            class_getInstanceMethod(fm, @selector(fileExistsAtPath:)),
            class_getInstanceMethod(fm, @selector(bp_fileExistsAtPath:)));
        method_exchangeImplementations(
            class_getInstanceMethod(fm, @selector(fileExistsAtPath:isDirectory:)),
            class_getInstanceMethod(fm, @selector(bp_fileExistsAtPath:isDirectory:)));
        method_exchangeImplementations(
            class_getInstanceMethod(fm, @selector(isReadableFileAtPath:)),
            class_getInstanceMethod(fm, @selector(bp_isReadableFileAtPath:)));

        Class app = [UIApplication class];
        method_exchangeImplementations(
            class_getInstanceMethod(app, @selector(canOpenURL:)),
            class_getInstanceMethod(app, @selector(bp_canOpenURL:)));
    });
    NSLog(@"[BP] ObjC swizzles applied");
}

#pragma mark - Bypass: fishhook C functions

static int (*orig_stat)(const char *, struct stat *);
static int (*orig_lstat)(const char *, struct stat *);
static int (*orig_access)(const char *, int);
static int (*orig_fork)(void);
static int (*orig_sysctl)(int *, u_int, void *, size_t *, void *, size_t);
static int (*orig_sysctlbyname)(const char *, void *, size_t *, void *, size_t);
static const char *(*orig_dyld_get_image_name)(uint32_t);

static int bp_stat(const char *path, struct stat *buf) {
    if (path) { NSString *p = @(path); if (bp_isBannedPath(p)) return -1; }
    return orig_stat(path, buf);
}
static int bp_lstat(const char *path, struct stat *buf) {
    if (path) { NSString *p = @(path); if (bp_isBannedPath(p)) return -1; }
    return orig_lstat(path, buf);
}
static int bp_access(const char *path, int mode) {
    if (path) { NSString *p = @(path); if (bp_isBannedPath(p)) return -1; }
    return orig_access(path, mode);
}
static int bp_fork(void) { return -1; }
static int bp_sysctl(int *name, u_int namelen, void *oldp, size_t *oldlenp, void *newp, size_t newlen) {
    if (namelen >= 2 && name[0] == CTL_KERN && name[1] == KERN_PROC) return -1;
    return orig_sysctl(name, namelen, oldp, oldlenp, newp, newlen);
}
static int bp_sysctlbyname(const char *name, void *oldp, size_t *oldlenp, void *newp, size_t newlen) {
    if (!name) return -1;
    if (strcmp(name, "kern.proc.all") == 0 || strcmp(name, "kern.proc.pid") == 0) return -1;
    return orig_sysctlbyname(name, oldp, oldlenp, newp, newlen);
}
static const char *bp_dyld_get_image_name(uint32_t index) {
    const char *name = orig_dyld_get_image_name(index);
    if (!name) return name;
    if (strstr(name, "bypass") || strstr(name, "Substrate") ||
        strstr(name, "ElleKit") || strstr(name, "TweakInject") ||
        strstr(name, "libhooker") || strstr(name, "cycript") ||
        strstr(name, "frida")) {
        return "/usr/lib/libsystem_c.dylib";
    }
    return name;
}

+ (void)applyCFunctionHooks {
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        struct rebinding r[] = {
            {"stat",                  bp_stat,              (void **)&orig_stat},
            {"lstat",                 bp_lstat,             (void **)&orig_lstat},
            {"access",                bp_access,            (void **)&orig_access},
            {"fork",                  bp_fork,              (void **)&orig_fork},
            {"sysctl",                bp_sysctl,            (void **)&orig_sysctl},
            {"sysctlbyname",          bp_sysctlbyname,      (void **)&orig_sysctlbyname},
            {"_dyld_get_image_name",  bp_dyld_get_image_name, (void **)&orig_dyld_get_image_name},
        };
        rebind_symbols(r, sizeof(r) / sizeof(r[0]));
    });
    NSLog(@"[BP] C hooks applied");
}

@end

#pragma mark - Swizzle impls

@implementation NSFileManager (BPBypass)
- (BOOL)bp_fileExistsAtPath:(NSString *)path {
    if (bp_isBannedPath(path)) return NO;
    return [self bp_fileExistsAtPath:path];
}
- (BOOL)bp_fileExistsAtPath:(NSString *)path isDirectory:(BOOL *)isDir {
    if (bp_isBannedPath(path)) { if (isDir) *isDir = NO; return NO; }
    return [self bp_fileExistsAtPath:path isDirectory:isDir];
}
- (BOOL)bp_isReadableFileAtPath:(NSString *)path {
    if (bp_isBannedPath(path)) return NO;
    return [self bp_isReadableFileAtPath:path];
}
@end

@implementation UIApplication (BPBypass)
- (BOOL)bp_canOpenURL:(NSURL *)url {
    NSString *scheme = url.scheme.lowercaseString;
    NSArray *banned = @[@"cydia", @"sileo", @"zbra", @"filza", @"frida",
                        @"unc0ver", @"taurine", @"palera1n", @"checkra1n"];
    if ([banned containsObject:scheme]) return NO;
    return [self bp_canOpenURL:url];
}
@end
