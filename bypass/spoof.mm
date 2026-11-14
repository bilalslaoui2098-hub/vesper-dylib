#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <objc/runtime.h>
#import <AdSupport/AdSupport.h>

// متغيرات باش نحفظو الـ UUID (باش يبقى ثابت)
static NSString* g_fake_idfv = nil;
static NSUUID* g_fake_idfa = nil;

static NSString* (*orig_idfv)(id, SEL);
static NSString* new_idfv(id self, SEL _cmd) {
    // إلا كان مخبي، نولدوه مرة وحدة
    if (!g_fake_idfv) {
        g_fake_idfv = [[[NSUUID UUID] UUIDString] copy];
    }
    // كل مرة كنرجعو نفس الـ UUID
    return g_fake_idfv;
}

static NSUUID* (*orig_idfa)(id, SEL);
static NSUUID* new_idfa(id self, SEL _cmd) {
    if (!g_fake_idfa) {
        g_fake_idfa = [[NSUUID alloc] initWithUUIDString:@"00000000-0000-0000-0000-000000000000"];
    }
    return g_fake_idfa;
}

__attribute__((constructor))
static void init_spoof() {
    Method m_idfv = class_getInstanceMethod([UIDevice class], @selector(identifierForVendor));
    if (m_idfv) {
        orig_idfv = (NSString* (*)(id, SEL))method_getImplementation(m_idfv);
        method_setImplementation(m_idfv, (IMP)new_idfv);
    }
    
    Class asmClass = NSClassFromString(@"ASIdentifierManager");
    if (asmClass) {
        Method m_idfa = class_getInstanceMethod(asmClass, @selector(advertisingIdentifier));
        if (m_idfa) {
            orig_idfa = (NSUUID* (*)(id, SEL))method_getImplementation(m_idfa);
            method_setImplementation(m_idfa, (IMP)new_idfa);
        }
    }
}
