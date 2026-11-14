#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <objc/runtime.h>
#import <AdSupport/AdSupport.h>
#include <dlfcn.h>

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

__attribute__((constructor))
static void init_spoof() {
    // Spoof IDFV
    Method m_idfv = class_getInstanceMethod([UIDevice class], @selector(identifierForVendor));
    if (m_idfv) {
        orig_idfv = (NSString* (*)(id, SEL))method_getImplementation(m_idfv);
        method_setImplementation(m_idfv, (IMP)new_idfv);
    }
    
    // Spoof IDFA
    Class asmClass = NSClassFromString(@"ASIdentifierManager");
    if (asmClass) {
        Method m_idfa = class_getInstanceMethod(asmClass, @selector(advertisingIdentifier));
        if (m_idfa) {
            orig_idfa = (NSUUID* (*)(id, SEL))method_getImplementation(m_idfa);
            method_setImplementation(m_idfa, (IMP)new_idfa);
        }
    }
}
