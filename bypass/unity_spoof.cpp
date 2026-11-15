#include <substrate.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <dispatch/dispatch.h>

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
    if (il2cpp_string_new) return il2cpp_string_new(str);
    return NULL;
}

// ============ IDs وهمية ============
static const char* FAKE_DEVICE_ID    = "A1B2C3D4-E5F6-7890-ABCD-EF1234567890";
static const char* FAKE_DEVICE_MODEL = "iPhone14,5";
static const char* FAKE_GOP_ID        = "GOP_1234567890_ABCDEF";

// ============ 1. Spoofing - HWID ============
static Il2CppString* (*orig_ReadiOSDeviceIdentifier)();
static Il2CppString* hook_ReadiOSDeviceIdentifier() {
    Il2CppString* f = create_fake_string(FAKE_DEVICE_ID);
    if (f) return f;
    return orig_ReadiOSDeviceIdentifier();
}

static Il2CppString* (*orig_get_deviceUniqueIdentifier)();
static Il2CppString* hook_get_deviceUniqueIdentifier() {
    Il2CppString* f = create_fake_string(FAKE_DEVICE_ID);
    if (f) return f;
    return orig_get_deviceUniqueIdentifier();
}

static Il2CppString* (*orig_GetDeviceUniqueIdentifier)();
static Il2CppString* hook_GetDeviceUniqueIdentifier() {
    Il2CppString* f = create_fake_string(FAKE_DEVICE_ID);
    if (f) return f;
    return orig_GetDeviceUniqueIdentifier();
}

static void (*orig_CompleteGetGOPDeviceID)(void* self, Il2CppString* deviceID);
static void hook_CompleteGetGOPDeviceID(void* self, Il2CppString* deviceID) {
    Il2CppString* f = create_fake_string(FAKE_GOP_ID);
    if (orig_CompleteGetGOPDeviceID) orig_CompleteGetGOPDeviceID(self, f);
}

// ============ 2. Spoofing - Device Model ============
static Il2CppString* (*orig_getSystemInfo_Machine)();
static Il2CppString* hook_getSystemInfo_Machine() {
    Il2CppString* f = create_fake_string(FAKE_DEVICE_MODEL);
    if (f) return f;
    return orig_getSystemInfo_Machine();
}

static Il2CppString* (*orig_get_deviceModel)();
static Il2CppString* hook_get_deviceModel() {
    Il2CppString* f = create_fake_string(FAKE_DEVICE_MODEL);
    if (f) return f;
    return orig_get_deviceModel();
}

static Il2CppString* (*orig_GetDeviceModel)();
static Il2CppString* hook_GetDeviceModel() {
    Il2CppString* f = create_fake_string(FAKE_DEVICE_MODEL);
    if (f) return f;
    return orig_GetDeviceModel();
}

// ============ 3. Block: Debug.isDebugBuild ============
static bool (*orig_get_isDebugBuild)();
static bool hook_get_isDebugBuild() {
    return false; // اللعبة كتظن أنها نسخة Production، ماشي Debug
}

// ============ 4. Block: GameConfig.ShowDebugUI ============
static bool (*orig_get_ShowDebugUI)();
static bool hook_get_ShowDebugUI() {
    return false;
}

static bool (*orig_get_OpenBetaDebugUI)();
static bool hook_get_OpenBetaDebugUI() {
    return false;
}

// ============ 5. Block: MobileMatchGameClient.FixDeviceID ============
// هادي دالة void كتعاود تقرا الـ Real Device ID فـ وسط الماتش
static void (*orig_FixDeviceID)(void* self);
static void hook_FixDeviceID(void* self) {
    return; // كنمنعوها، ما نعتوهاش تقرا الـ ID الحقيقي
}

// ============ 6. Block: Anti-Cheat Reports ============
static void (*orig_LogReportCheat)(void* cheater, uint32_t reason, void* cheaterPlayerID,
                                    void* subReason, bool inGame, uint32_t reporteeType,
                                    int reportScene, void* cheaterClientVersion,
                                    uint32_t cheaterClientType, int reportMethod);
static void hook_LogReportCheat(void* cheater, uint32_t reason, void* cheaterPlayerID,
                                 void* subReason, bool inGame, uint32_t reporteeType,
                                 int reportScene, void* cheaterClientVersion,
                                 uint32_t cheaterClientType, int reportMethod) {
    return; // نسكتو، ما تصيفطش تقارير
}

static void (*orig_ShowHackerDetectedUI)(void* self, void* OnNormalMatchEnd, bool isHackerTeam);
static void hook_ShowHackerDetectedUI(void* self, void* OnNormalMatchEnd, bool isHackerTeam) {
    return; // ما تطلعش الشاشة
}

static void (*orig_OnNavigationShowed_AntiHack)(void* self, void* navigationData, bool isRoot);
static void hook_OnNavigationShowed_AntiHack(void* self, void* navigationData, bool isRoot) {
    return; // ما تطلعش شاشة AntiHack
}


// ============ التركيب الرئيسي ============
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

    // --- HWID Spoofs ---
    MSHookFunction((void*)(unity_base + 0x55278B8), (void*)hook_ReadiOSDeviceIdentifier, (void**)&orig_ReadiOSDeviceIdentifier);
    MSHookFunction((void*)(unity_base + 0x8D673D8), (void*)hook_get_deviceUniqueIdentifier, (void**)&orig_get_deviceUniqueIdentifier);
    MSHookFunction((void*)(unity_base + 0x8D67418), (void*)hook_GetDeviceUniqueIdentifier, (void**)&orig_GetDeviceUniqueIdentifier);
    MSHookFunction((void*)(unity_base + 0x6D0BE18), (void*)hook_CompleteGetGOPDeviceID, (void**)&orig_CompleteGetGOPDeviceID);

    // --- Device Model Spoofs ---
    MSHookFunction((void*)(unity_base + 0x6B90330), (void*)hook_getSystemInfo_Machine, (void**)&orig_getSystemInfo_Machine);
    MSHookFunction((void*)(unity_base + 0x8D67458), (void*)hook_get_deviceModel, (void**)&orig_get_deviceModel);
    MSHookFunction((void*)(unity_base + 0x8D67498), (void*)hook_GetDeviceModel, (void**)&orig_GetDeviceModel);

    // --- Debug Blocking ---
    MSHookFunction((void*)(unity_base + 0x8D04A14), (void*)hook_get_isDebugBuild, (void**)&orig_get_isDebugBuild);
    MSHookFunction((void*)(unity_base + 0x507AAFC), (void*)hook_get_ShowDebugUI, (void**)&orig_get_ShowDebugUI);
    MSHookFunction((void*)(unity_base + 0x507AD48), (void*)hook_get_OpenBetaDebugUI, (void**)&orig_get_OpenBetaDebugUI);

    // --- FixDeviceID Block ---
    MSHookFunction((void*)(unity_base + 0x15C701C), (void*)hook_FixDeviceID, (void**)&orig_FixDeviceID);

    // --- Report / UI Blocking ---
    MSHookFunction((void*)(unity_base + 0x53CAF64), (void*)hook_LogReportCheat, (void**)&orig_LogReportCheat);
    MSHookFunction((void*)(unity_base + 0x5E3FD58), (void*)hook_ShowHackerDetectedUI, (void**)&orig_ShowHackerDetectedUI);
    MSHookFunction((void*)(unity_base + 0x2AC7D40), (void*)hook_OnNavigationShowed_AntiHack, (void**)&orig_OnNavigationShowed_AntiHack);

    dlclose(h);
}

__attribute__((constructor))
static void init_unity_spoofer() {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        Install_Unity_Spoofer();
    });
}
