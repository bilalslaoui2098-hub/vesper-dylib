#include <pthread.h>
#include <mach/mach.h>
#include <dlfcn.h>
#include <string.h>
#include <substrate.h>

// نخليو الـ Threads ديالنا يبانو كأنهم من النظام
static int (*orig_pthread_create)(
    pthread_t *restrict, const pthread_attr_t *restrict,
    void *(*)(void *), void *restrict
);

static int hook_pthread_create(
    pthread_t *restrict th, const pthread_attr_t *restrict attr,
    void *(*start)(void *), void *restrict arg
) {
    int res = orig_pthread_create(th, attr, start, arg);
    if (res == 0 && th) {
        // نعطيوه سمية رسمية ديال Apple
        pthread_setname_np(*th, "com.apple.dispatch-helper");
    }
    return res;
}

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
        // نخليو الـ Threads ديال النظام برك، ونحيّدو اللي زدنا حنا
        // (تقدر تفلترهم بالـ thread ID إلا عندك، أو تخليهم كلهم)
        // دابا ما كنحيدو حتى حاجة، كنخليوه كيرجع عادي
    }
    return r;
}

void install_thread_hide(void) {
    void *a = dlsym(RTLD_DEFAULT, "pthread_create");
    if (a) MSHookFunction(a, (void*)hook_pthread_create,
                         (void**)&orig_pthread_create);

    a = dlsym(RTLD_DEFAULT, "task_threads");
    if (a) MSHookFunction(a, (void*)hook_task_threads,
                         (void**)&orig_task_threads);
}
