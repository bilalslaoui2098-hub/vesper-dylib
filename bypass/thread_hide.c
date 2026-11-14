#include <pthread.h>
#include <mach/mach.h>
#include <dlfcn.h>
#include <string.h>
#include <substrate.h>

// نخبيو Threads من task_threads
static kern_return_t (*orig_task_threads)(
    task_t, thread_act_array_t *, mach_msg_type_number_t *
);

static kern_return_t hook_task_threads(
    task_t task, thread_act_array_t *threads_out,
    mach_msg_type_number_t *count
) {
    kern_return_t r = orig_task_threads(task, threads_out, count);
    if (r == KERN_SUCCESS && task == mach_task_self() && threads_out && count) {
        // هنا يمكن نفلترو الـ Threads ديالنا إلا بغينا
    }
    return r;
}

void install_thread_hide(void) {
    // فـ iOS، ما يمكنش نسميو thread آخر. 
    // الحل هو نسميو الـ Threads ديالنا من داخلهم (فـ anti_crash.c و anti_scan.c).
    // هاد الدالة دابا غير كتحمي task_threads.
    void *a = dlsym(RTLD_DEFAULT, "task_threads");
    if (a) MSHookFunction(a, (void*)hook_task_threads,
                         (void**)&orig_task_threads);
}
