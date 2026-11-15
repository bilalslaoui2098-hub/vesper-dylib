#include <substrate.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <dispatch/dispatch.h>

// هيكل السلسلة ديال Unity (Il2CppString)
struct Il2CppString {
    void *klass;
    void *monitor;
    int length;
    uint16_t chars[1];
};

// مؤشر للدالة اللي كتصنع النصوص فـ Unity
static Il2CppString* (*il2cpp_string_new)(const char*);

// دالة باش نلقاو il2cpp_string_new فـ وقت التشغيل
static void init_il2cpp_string() {
    if (il2cpp_string_new) return;
    void* h = dlopen("UnityFramework", RTLD_LAZY);
    if (!h) h = dlopen("@rpath/UnityFramework.framework/UnityFramework", RTLD_LAZY);
    if (h) {
        il2cpp_string_new = (Il2CppString*(*)(const char*))dlsym(h, "il2cpp_string_new");
        dlclose(h);
    }
}

// نصنعو سلسلة وهمية (Fake String) مقروءة من طرف Unity
static Il2CppString* create_fake_string(const char* str) {
    init_il2cpp_string();
    if (il2cpp_string_new) {
        return il2cpp_string_new(str);
    }
    return NULL;
}

// الـ IDs الوهمية اللي غادي نعطيوهم للعبة (تقدر تبدلهم كيفما بغيتي)
static const char* FAKE_DEVICE_ID = "A1B2C3D4-E5F6-7890-ABCD-EF1234567890";
static const char* FAKE_DEVICE_MODEL = "iPhone14,5"; 
static const char* FAKE_GOP_ID = "GOP_1234567890_ABCDEF";

// =====================================================================
// 1. DeviceIdentifier.ReadiOSDeviceIdentifier
// =====================================================================
static Il2CppString* (*orig_ReadiOSDeviceIdentifier)();
static Il2CppString* hook_ReadiOSDeviceIdentifier() {
    Il2CppString* fake = create_fake_string(FAKE_DEVICE_ID);
    if (fake) return fake;
    return orig_ReadiOSDeviceIdentifier();
}

// =====================================================================
// 2. SystemInfo.get_deviceUniqueIdentifier
// =====================================================================
static Il2CppString* (*orig_get_deviceUniqueIdentifier)();
static Il2CppString* hook_get_deviceUniqueIdentifier() {
    Il2CppString* fake = create_fake_string(FAKE_DEVICE_ID);
    if (fake) return fake;
    return orig_get_deviceUniqueIdentifier();
}

// =====================================================================
// 3. PlatformUtility_iOS.getSystemInfo_Machine
// =====================================================================
static Il2CppString* (*orig_getSystemInfo_Machine)();
static Il2CppString* hook_getSystemInfo_Machine() {
    Il2CppString* fake = create_fake_string(FAKE_DEVICE_MODEL);
    if (fake) return fake;
    return orig_getSystemInfo_Machine();
}

// =====================================================================
// 4. GarenaMSDKMgr._CompleteGetGOPDeviceID
// =====================================================================
// هادي دالة void كتاخد (self, deviceID). غادي نبدلو الـ deviceID بالوهمي
static void (*orig_CompleteGetGOPDeviceID)(void* self, Il2CppString* deviceID);
static void hook_CompleteGetGOPDeviceID(void* self, Il2CppString* deviceID) {
    Il2CppString* fake = create_fake_string(FAKE_GOP_ID);
    if (orig_CompleteGetGOPDeviceID) {
        // كنعيطو للدالة الأصلية ولكن كنعطيوهالها الـ ID الوهمي
        orig_CompleteGetGOPDeviceID(self, fake);
    }
}


// =====================================================================
// دالة التركيب الرئيسية
// =====================================================================
void Install_Unity_Spoofer(void) {
    void* h = dlopen("UnityFramework", RTLD_LAZY);
    if (!h) h = dlopen("@rpath/UnityFramework.framework/UnityFramework", RTLD_LAZY);
    if (!h) return;

    // نلقاو الـ base ديال UnityFramework فـ الذاكرة
    uint32_t count = _dyld_image_count();
    uintptr_t unity_base = 0;
    for (uint32_t i = 0; i < count; i++) {
        const char* name = _dyld_get_image_name(i);
        if (name && strstr(name, "UnityFramework")) {
            unity_base = (uintptr_t)_dyld_get_image_header(i);
            break;
        }
    }
    if (!unity_base) { dlclose(h); return; }

    // العناوين (RVAs) اللي لقيناهم فـ dump.cs
    uint64_t rva_ReadiOSDeviceIdentifier   = 0x55278B8;
    uint64_t rva_get_deviceUniqueIdentifier = 0x8D673D8;
    uint64_t rva_getSystemInfo_Machine      = 0x6B90330;
    uint64_t rva_CompleteGetGOPDeviceID     = 0x6D0BE18;

    // نضربو الـ Hooks
    MSHookFunction((void*)(unity_base + rva_ReadiOSDeviceIdentifier), (void*)hook_ReadiOSDeviceIdentifier, (void**)&orig_ReadiOSDeviceIdentifier);
    MSHookFunction((void*)(unity_base + rva_get_deviceUniqueIdentifier), (void*)hook_get_deviceUniqueIdentifier, (void**)&orig_get_deviceUniqueIdentifier);
    MSHookFunction((void*)(unity_base + rva_getSystemInfo_Machine), (void*)hook_getSystemInfo_Machine, (void**)&orig_getSystemInfo_Machine);
    MSHookFunction((void*)(unity_base + rva_CompleteGetGOPDeviceID), (void*)hook_CompleteGetGOPDeviceID, (void**)&orig_CompleteGetGOPDeviceID);

    dlclose(h);
}

// =====================================================================
// تأخير التحميل (Delayed Injection)
// =====================================================================
// هاد الـ Constructor غادي يتحمل مع الديلب، ولكن غادي يستنى 3 ثواني
// باش يضمن أن UnityFramework كامل تحمل قبل ما نضربو الـ Hooks
__attribute__((constructor))
static void init_unity_spoofer() {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        Install_Unity_Spoofer();
    });
}
