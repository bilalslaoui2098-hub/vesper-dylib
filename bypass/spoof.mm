#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <objc/runtime.h>
#import <Security/Security.h>
#import <AdSupport/AdSupport.h>
#include <dlfcn.h>
#include <substrate.h>

// 1. Spoof IDFV
static NSString* (*orig_idfv)(id, SEL);
static NSString* new_idfv(id self, SEL _cmd) {
    return [[NSUUID UUID] UUIDString];
}

// 2. Spoof IDFA
static NSUUID* (*orig_idfa)(id, SEL);
static NSUUID* new_idfa(id self, SEL _cmd) {
    return [[NSUUID alloc] initWithUUIDString:@"00000000-0000-0000-0000-000000000000"];
}

// 3. Spoof Keychain (نمسحو أي بصمة قديمة)
static OSStatus (*orig_SecItemCopyMatching)(CFDictionaryRef query, CFTypeRef *result);
static OSStatus new_SecItemCopyMatching(CFDictionaryRef query, CFTypeRef *result) {
    return errSecItemNotFound;
}

__attribute__((constructor))
static void init_spoof() {
    // Spoof IDFV
    Method m_idfv = class_getInstanceMethod([UIDevice class], @selector(identifierForVendor));
    if (m_idfv) {
        orig_idfv = (void*)method_getImplementation(m_idfv);
        method_setImplementation(m_idfv, (IMP)new_idfv);
    }
    
    // Spoof IDFA
    Class asmClass = NSClassFromString(@"ASIdentifierManager");
    if (asmClass) {
        Method m_idfa = class_getInstanceMethod(asmClass, @selector(advertisingIdentifier));
        if (m_idfa) {
            orig_idfa = (void*)method_getImplementation(m_idfa);
            method_setImplementation(m_idfa, (IMP)new_idfa);
        }
    }
    
    // Spoof Keychain
    void *secHandle = dlopen("/System/Library/Frameworks/Security.framework/Security", RTLD_LAZY);
    if (secHandle) {
        orig_SecItemCopyMatching = dlsym(secHandle, "SecItemCopyMatching");
        if (orig_SecItemCopyMatching) {
            MSHookFunction((void*)orig_SecItemCopyMatching, (void*)new_SecItemCopyMatching, (void**)&orig_SecItemCopyMatching);
        }
    }
}
