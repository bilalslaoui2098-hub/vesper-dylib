#include <stdint.h>
#include <string.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <dlfcn.h>

// نلقاو الـ dyld_all_image_infos
static struct dyld_all_image_infos *get_all_image_infos(void) {
    void *handle = dlopen("/usr/lib/dyld", RTLD_LAZY);
    if (!handle) return NULL;
    struct dyld_all_image_infos *infos = (struct dyld_all_image_infos *)dlsym(handle, "dyld_all_image_infos");
    dlclose(handle);
    return infos;
}

// نخبيو الـ Dylib ديالنا من الـ image list
void hide_our_dylib(void) {
    struct dyld_all_image_infos *infos = get_all_image_infos();
    if (!infos) return;
    
    // نلقاو الـ Dylib ديالنا
    const char *our_name = "vesper.dylib"; // بدلها بالاسم ديالك
    for (uint32_t i = 0; i < infos->infoArrayCount; i++) {
        const char *name = infos->infoArray[i].imageFilePath;
        if (name && strstr(name, our_name)) {
            // نمسحو الاسم
            infos->infoArray[i].imageFilePath = "";
            break;
        }
    }
}
