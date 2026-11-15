#include <substrate.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "Helper/Mem.h" // باش نستعملو getRealOffset

// تعريف هيكل السلسلة ديال Unity (Il2CppString)
struct Il2CppString {
    void *klass;
    void *monitor;
    int length;
    uint16_t chars[1];
};

// دالة باش نصنعو سلسلة جديدة فـ Unity
extern "C" Il2CppString* il2cpp_string_new(const char* str);

// المؤشرات للأدوال الأصلية
static Il2CppString* (*orig_get_DeviceUniqueIdentifier_Instance)(void* self);
static Il2CppString* (*orig_get_DeviceUniqueIdentifier_Static)();
static Il2CppString* (*orig_GetDeviceUniqueIdentifier)();

// الدالة الوهمية اللي غادي ترجع ID جديد
Il2CppString* new_get_DeviceUniqueIdentifier() {
    // هنا كنعطيو ID وهمي جديد (تقدر تبدلو كل مرة)
    return il2cpp_string_new("A1B2C3D4-E5F6-7890-ABCD-EF1234567890");
}

Il2CppString* new_get_DeviceUniqueIdentifier_Instance(void* self) {
    return new_get_DeviceUniqueIdentifier();
}

// دالة التركيب
void Install_Unity_Spoofer() {
    // العناوين اللي لقيتيها فـ dump.cs
    uint64_t rva_instance = 0x462A1AC;
    uint64_t rva_static = 0x8D673D8;
    uint64_t rva_private = 0x8D67418;

    // نلقاو العناوين الحقيقية فـ الذاكرة
    void* addr_instance = (void*)getRealOffset(rva_instance);
    void* addr_static = (void*)getRealOffset(rva_static);
    void* addr_private = (void*)getRealOffset(rva_private);

    if (addr_instance) {
        MSHookFunction(addr_instance, (void*)new_get_DeviceUniqueIdentifier_Instance, (void**)&orig_get_DeviceUniqueIdentifier_Instance);
        printf("[*] Hooked DeviceUniqueIdentifier (Instance) at %p\n", addr_instance);
    }
    if (addr_static) {
        MSHookFunction(addr_static, (void*)new_get_DeviceUniqueIdentifier, (void**)&orig_get_DeviceUniqueIdentifier_Static);
        printf("[*] Hooked DeviceUniqueIdentifier (Static) at %p\n", addr_static);
    }
    if (addr_private) {
        MSHookFunction(addr_private, (void*)new_get_DeviceUniqueIdentifier, (void**)&orig_GetDeviceUniqueIdentifier);
        printf("[*] Hooked GetDeviceUniqueIdentifier (Private) at %p\n", addr_private);
    }
}
