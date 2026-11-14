#include "anti_crash.h"
#include <pthread.h>
#include <unistd.h>

static void *anti_crash_thread(void *arg) {
    (void)arg;
    pthread_setname_np("com.apple.dispatch-helper"); // نسميو الـ thread ديالنا
    while (1) {
        sleep(10);
    }
    return NULL;
}

void anti_crash_init(void) {
    pthread_t t;
    pthread_create(&t, NULL, anti_crash_thread, NULL);
    pthread_detach(t);
}
