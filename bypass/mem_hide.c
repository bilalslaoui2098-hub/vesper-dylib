#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <mach/mach.h>
#include <mach/vm_region.h>
#include <mach/vm_map.h>

// نخبيو صفحات الذاكرة اللي فيها الـ Hooks
// هادشي كيتعمل تلقائياً حيت الـ Hooks ديالنا فـ صفحات مشبوهة
