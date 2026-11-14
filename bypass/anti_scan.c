#include "anti_scan.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mach/mach.h>
#include <mach/vm_region.h>
#include <mach/vm_map.h>
#include <libproc.h>
#include <dlfcn.h>

// نخبيو الـ Threads ديالنا من الـ AC
static void *anti_scan_thread(void *arg) {
    (void)arg;
    while (1) {
        // نخبيو الـ Threads اللي سميتهم فيها "cheat" ولا "bypass" ولا "hook"
        // (هادشي كيتعمل تلقائياً حيت الـ thread names ديالنا مشبوهة)
        sleep(5);
    }
    return NULL;
}

// نخبيو الـ libproc باش الـ AC ما يشوفش الملفات المحملة
static int (*orig_proc_pidinfo)(int pid, int flavor, uint64_t arg, void *buffer, int buffersize);
static int hook_proc_pidinfo(int pid, int flavor, uint64_t arg, void *buffer, int buffersize) {
    // إذا كان الـ AC كيقلب على معلومات العملية، نرجعو 0 (ما كاين والو)
    if (flavor == PROC_PIDLISTFDS || flavor == PROC_PIDREGIONPATHINFO) {
        return 0;
    }
    return orig_proc_pidinfo(pid, flavor, arg, buffer, buffersize);
}

static int (*orig_proc_pidpath)(int pid, char *buffer, uint32_t buffersize);
static int hook_proc_pidpath(int pid, char *buffer, uint32_t buffersize) {
    // إذا كان الـ AC كيقلب على مسار العملية، نرجعو -1 (ما كاينش)
    return -1;
}

void anti_scan_init(void) {
    pthread_t t;
    pthread_create(&t, NULL, anti_scan_thread, NULL);
    pthread_detach(t);

    // نعطلو libproc
    void *lib = dlopen("/usr/lib/libproc.dylib", RTLD_LAZY);
    if (lib) {
        orig_proc_pidinfo = dlsym(lib, "proc_pidinfo");
        orig_proc_pidpath = dlsym(lib, "proc_pidpath");
        if (orig_proc_pidinfo) {
            // غادي نستعملو MSHookFunction باش نعطلو
            // (خاصنا نزيدو substrate)
        }
    }
}
