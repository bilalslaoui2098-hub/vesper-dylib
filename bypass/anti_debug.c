#include <pthread.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <mach/mach_types.h>
#include <string.h>
#include <dlfcn.h>

// نتحققو واش كاين شي حد كيتبعنا
static int is_being_traced(void) {
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    struct kinfo_proc info;
    size_t size = sizeof(info);
    
    if (sysctl(mib, 4, &info, &size, NULL, 0) == 0) {
        return (info.kp_proc.p_flag & P_TRACED) ? 1 : 0;
    }
    return 0;
}

// نمسحو أي اثر ديال التتبع
static void detach_tracer(void) {
    // نحاولو نفكو الـ task من أي tracer
    mach_port_t task = mach_task_self();
    mach_port_t target = MACH_PORT_NULL;
    
    // هاد الحل الناعم - كانخليو الـ AC يظن أننا عاديين
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    struct kinfo_proc info;
    size_t size = sizeof(info);
    if (sysctl(mib, 4, &info, &size, NULL, 0) == 0) {
        info.kp_proc.p_flag &= ~P_TRACED;
    }
}

// Thread ديال المراقبة
static void *watchdog_thread(void *arg) {
    (void)arg;
    while (1) {
        if (is_being_traced()) {
            detach_tracer();
        }
        sleep(5);
    }
    return NULL;
}

void anti_debug_init(void) {
    pthread_t t;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&t, &attr, watchdog_thread, NULL);
    pthread_attr_destroy(&attr);
}
