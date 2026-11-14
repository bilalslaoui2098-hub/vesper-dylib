#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach/vm_region.h>
#include <mach/vm_map.h>
#include <libkern/OSCacheControl.h>

// نشفرو صفحة ذاكرة
void encrypt_page(void *addr, size_t size) {
    uint8_t *p = (uint8_t *)addr;
    for (size_t i = 0; i < size; i++) {
        p[i] ^= 0xAA; // XOR بسيط
    }
    sys_icache_invalidate(addr, size);
}

// نفكو التشفير
void decrypt_page(void *addr, size_t size) {
    uint8_t *p = (uint8_t *)addr;
    for (size_t i = 0; i < size; i++) {
        p[i] ^= 0xAA;
    }
    sys_icache_invalidate(addr, size);
}
