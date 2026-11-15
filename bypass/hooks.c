#include <substrate.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>
#include "install.h"

// هيكل السلسلة ديال Unity (Il2CppString)
struct Il2CppString {
    void *klass;
    void *monitor;
    int length;
    uint16_t chars[1];
};

// مؤشرات للأدوات
static Il2CppString* (*il2cpp_string_new)(const char*);
static Il2CppString* (*il2cpp_string_new_utf16)(const uint16_t*, int);

// دالة باش نلقاو il2cpp_string_new
static void init_il2cpp_string() {
    if (il2cpp_string_new) return;
    void* h = dlopen("UnityFramework", RTLD_LAZY);
    if (!h) h = dlopen("@rpath/UnityFramework.framework/UnityFramework", RTLD_LAZY);
    if (h) {
        il2cpp_string_new = (Il2CppString*(*)(const char*))dlsym(h, "il2cpp_string_new");
        il2cpp_string_new_utf16 = (Il2CppString*(*)(const uint16_t*,int))dlsym(h, "il2cpp_string_new_utf16");
        dlclose(h);
    }
}

// نصنعو سلسلة وهمية
static Il2CppString* create_fake_string(const char* str) {
    init_il2cpp_string();
    if (il2cpp_string_new) {
        return il2cpp_string_new(str);
    }
    return NULL;
}

// ID وهمي ديال الجهاز
static const char* FAKE_ID = "A1B2C3D4-E5F6-7890-ABCD-EF1234567890";

// دالة مؤقتة باش نخبيو الحقيقي
static Il2CppString* (*orig_get_device_id)();

static Il2CppString* hook_get_device_id() {
    Il2CppString* fake = create_fake_string(FAKE_ID);
    if (fake) return fake;
    if (orig_get_device_id) return orig_get_device_id();
    return NULL;
}

// تثبيت الـ Hook
void Install_Unity_Spoofer(void) {
    void* h = dlopen("UnityFramework", RTLD_LAZY);
    if (!h) h = dlopen("@rpath/UnityFramework.framework/UnityFramework", RTLD_LAZY);
    if (!h) return;

    void* base = dlsym(h, "il2cpp_string_new");
    if (!base) { dlclose(h); return; }
    
    // نلقاو base diyal UnityFramework
    void* main = (void*)dlsym(h, "il2cpp_init");
    if (!main) { dlclose(h); return; }
    
    // نستعملو RVAs li l9ina f dump.cs (khassna njibou base mzyan)
    // هادي عملية دقيقة، خاصنا نعرفو base ديال UnityFramework بالضبط
    // هاد الكود غادي يخدم غير إلى l9ina base mzyan
    
    // fallback: hook on the offset from _dyld_get_image_header for UnityFramework
    uint32_t count = _dyld_image_count();
    void* unity_base = NULL;
    for (uint32_t i = 0; i < count; i++) {
        const char* name = _dyld_get_image_name(i);
        if (name && strstr(name, "UnityFramework")) {
            unity_base = (void*)_dyld_get_image_header(i);
            break;
        }
    }
    if (!unity_base) { dlclose(h); return; }
    
    // RVA diyal GetDeviceUniqueIdentifier (private)
    uint64_t rva = 0x8D67418;
    void* addr = (uint8_t*)unity_base + rva;
    
    MSHookFunction(addr, (void*)hook_get_device_id, (void**)&orig_get_device_id);
    
    dlclose(h);
}
