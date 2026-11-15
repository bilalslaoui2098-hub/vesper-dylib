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

static Il2CppString* (*il2cpp_string_new)(const char*);

static void init_il2cpp_string() {
    if (il2cpp_string_new) return;
    void* h = dlopen("UnityFramework", RTLD_LAZY);
    if (!h) h = dlopen("@rpath/UnityFramework.framework/UnityFramework", RTLD_LAZY);
    if (h) {
        il2cpp_string_new = (Il2CppString*(*)(const char*))dlsym(h, "il2cpp_string_new");
        dlclose(h);
    }
}

static Il2CppString* create_fake_string(const char* str) {
    init_il2cpp_string();
    if (il2cpp_string_new) {
        return il2cpp_string_new(str);
    }
    return NULL;
}

// =====================================================================
// 1. Spoofing IDs (الطبقة القديمة ديال HWID)
// =====================================================================
static const char* FAKE_DEVICE_ID = "A1B2C3D4-E5F6-7890-ABCD-EF1234567890";
static const char* FAKE_DEVICE_MODEL = "iPhone14,5"; 
static const char* FAKE_GOP_ID = "GOP_1234567890_ABCDEF";

static Il2CppString* (*orig_ReadiOSDeviceIdentifier)();
static Il2CppString* hook_ReadiOSDeviceIdentifier() {
    Il2CppString* fake = create_fake_string(FAKE_DEVICE_ID);
    if (fake) return fake;
    return orig_ReadiOSDeviceIdentifier();
}

static Il2CppString* (*orig_get_deviceUniqueIdentifier)();
static Il2CppString* hook_get_deviceUniqueIdentifier() {
    Il2CppString* fake = create_fake_string(FAKE_DEVICE_ID);
    if (fake) return fake;
    return orig_get_deviceUniqueIdentifier();
}

static Il2CppString* (*orig_getSystemInfo_Machine)();
static Il2CppString* hook_getSystemInfo_Machine() {
    Il2CppString* fake = create_fake_string(FAKE_DEVICE_MODEL);
    if (fake) return fake;
    return orig_getSystemInfo_Machine();
}

static void (*orig_CompleteGetGOPDeviceID)(void* self, Il2CppString* deviceID);
static void hook_CompleteGetGOPDeviceID(void* self, Il2CppString* deviceID) {
    Il2CppString* fake = create_fake_string(FAKE_GOP_ID);
    if (orig_CompleteGetGOPDeviceID) orig_CompleteGetGOPDeviceID(self, fake);
}

// =====================================================================
// 2. Blocking Anti-Cheat Reports (الطبقة الجديدة الأقوى)
// =====================================================================

// EventLogger.LogReportCheat
// RVA: 0x53CAF64 (هاد الدالة كتصيفط التقرير للسيرفر)
static void (*orig_LogReportCheat)(void* cheater, uint32_t reason, void* cheaterPlayerID, void* subReason, bool inGame, uint32_t reporteeType, int reportScene, void* cheaterClientVersion, uint32_t cheaterClientType, int reportMethod);
static void hook_LogReportCheat(void* cheater, uint32_t reason, void* cheaterPlayerID, void* subReason, bool inGame, uint32_t reporteeType, int reportScene, void* cheaterClientVersion, uint32_t cheaterClientType, int reportMethod) {
    // كنسكتو عليها، ماكنخليوهاش تصيفط والو
    return; 
}

// UIInGameScene.ShowHackerDetectedUI
// RVA: 0x5E3FD58 (هاد الشاشة اللي كتطلع "Hacker Detected")
static void (*orig_ShowHackerDetectedUI)(void* self, void* OnNormalMatchEnd, bool isHackerTeam);
static void hook_ShowHackerDetectedUI(void* self, void* OnNormalMatchEnd, bool isHackerTeam) {
    // كنمنعو الشاشة ما تطلعش
    return;
}

// UIAntiHackWindowController.OnNavigationShowed
// RVA: 0x2AC7D40 (واجهة الـ AntiHack)
static void (*orig_OnNavigationShowed_AntiHack)(void* self, void* navigationData, bool isRoot);
static void hook_OnNavigationShowed_AntiHack(void* self, void* navigationData, bool isRoot) {
    // كنمنعو شاشة الـ AntiHack تبان
    return;
}


// =====================================================================
// دالة التركيب الرئيسية
// =====================================================================
void Install_Unity_Spoofer(void) {
    void* h = dlopen("UnityFramework", RTLD_LAZY);
    if (!h) h = dlopen("@rpath/UnityFramework.framework/UnityFramework", RTLD_LAZY);
    if (!h) return;

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

    // --- 1. Spoofs ---
    uint64_t rva_ReadiOSDeviceIdentifier   = 0x55278B8;
    uint64_t rva_get_deviceUniqueIdentifier = 0x8D673D8;
    uint64_t rva_getSystemInfo_Machine      = 0x6B90330;
    uint64_t rva_CompleteGetGOPDeviceID     = 0x6D0BE18;

    MSHookFunction((void*)(unity_base + rva_ReadiOSDeviceIdentifier), (void*)hook_ReadiOSDeviceIdentifier, (void**)&orig_ReadiOSDeviceIdentifier);
    MSHookFunction((void*)(unity_base + rva_get_deviceUniqueIdentifier), (void*)hook_get_deviceUniqueIdentifier, (void**)&orig_get_deviceUniqueIdentifier);
    MSHookFunction((void*)(unity_base + rva_getSystemInfo_Machine), (void*)hook_getSystemInfo_Machine, (void**)&orig_getSystemInfo_Machine);
    MSHookFunction((void*)(unity_base + rva_CompleteGetGOPDeviceID), (void*)hook_CompleteGetGOPDeviceID, (void**)&orig_CompleteGetGOPDeviceID);

    // --- 2. Anti-Cheat Blockers ---
    uint64_t rva_LogReportCheat             = 0x53CAF64;
    uint64_t rva_ShowHackerDetectedUI       = 0x5E3FD58;
    uint64_t rva_OnNavigationShowed_AntiHack = 0x2AC7D40;

    MSHookFunction((void*)(unity_base + rva_LogReportCheat), (void*)hook_LogReportCheat, (void**)&orig_LogReportCheat);
    MSHookFunction((void*)(unity_base + rva_ShowHackerDetectedUI), (void*)hook_ShowHackerDetectedUI, (void**)&orig_ShowHackerDetectedUI);
    MSHookFunction((void*)(unity_base + rva_OnNavigationShowed_AntiHack), (void*)hook_OnNavigationShowed_AntiHack, (void**)&orig_OnNavigationShowed_AntiHack);

    dlclose(h);
}

// =====================================================================
// تأخير التحميل (Delayed Injection)
// =====================================================================
__attribute__((constructor))
static void init_unity_spoofer() {
    // كنسناو 3 ثواني باش نضمنو أن Unity واجد وكامل تحمل
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        Install_Unity_Spoofer();
    });
}
