
#include <mach/mach.h>
#include <mach/exception_types.h>
#include <dlfcn.h>
#include <substrate.h>
#include <string.h>

// نعرفو الدالة الأصلية
static kern_return_t (*orig_task_set_exception_ports)(
    task_t task, exception_mask_t masks, mach_port_t new_port,
    exception_behavior_t behavior, thread_state_flavor_t flavor
);

// الدالة البديلة
static kern_return_t hook_task_set_exception_ports(
    task_t task, exception_mask_t masks, mach_port_t new_port,
    exception_behavior_t behavior, thread_state_flavor_t flavor
) {
    // إلا حاول الـ AC يحط Exception Port على العملية ديالنا => نرفض
    if (task == mach_task_self()) {
        // نرفض EXC_BAD_ACCESS (بحال شي مفتش)
        if (masks & EXC_MASK_BAD_ACCESS) {
            return KERN_PROTECTION_FAILURE;
        }
    }
    // إلا كان طلب عادي من النظام نفسه، نخليوه يعدي
    return orig_task_set_exception_ports(task, masks, new_port, behavior, flavor);
}

void install_exception_hook(void) {
    void *a = dlsym(RTLD_DEFAULT, "task_set_exception_ports");
    if (a) MSHookFunction(a, (void*)hook_task_set_exception_ports,
                         (void**)&orig_task_set_exception_ports);
}
