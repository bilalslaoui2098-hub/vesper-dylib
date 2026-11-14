#include "anti_scan.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <substrate.h>

// نخبيو الـ Threads ديالنا
static void *anti_scan_thread(void *arg) {
    (void)arg;
    while (1) {
        sleep(5);
    }
    return NULL;
}

// نعطلو proc_pidinfo
static int (*orig_proc_pidinfo)(int pid, int flavor, uint64_t arg, void *buffer, int buffersize);
static int hook_proc_pidinfo(int pid, int flavor, uint64_t arg, void *buffer, int buffersize) {
    if (flavor == 1 || flavor == 3) { // PROC_PIDLISTFDS = 1, PROC_PIDREGIONPATHINFO = 3
        return 0;
    }
    return orig_proc_pidinfo(pid, flavor, arg, buffer, buffersize);
}

// نعطلو proc_pidpath
static int (*orig_proc_pidpath)(int pid, char *buffer, uint32_t buffersize);
static int hook_proc_pidpath(int pid, char *buffer, uint32_t buffersize) {
    return -1;
}

void anti_scan_init(void) {
    pthread_t t;
    pthread_create(&t, NULL, anti_scan_thread, NULL);
    pthread_detach(t);

    // نلقاو libproc فـ وقت التشغيل
    void *lib = dlopen("/usr/lib/libproc.dylib", RTLD_LAZY);
    if (lib) {
        void *a = dlsym(lib, "proc_pidinfo");
        if (a) MSHookFunction(a, (void*)hook_proc_pidinfo, (void**)&orig_proc_pidinfo);

        a = dlsym(lib, "proc_pidpath");
        if (a) MSHookFunction(a, (void*)hook_proc_pidpath, (void**)&orig_proc_pidpath);

        dlclose(lib);
    }
}
