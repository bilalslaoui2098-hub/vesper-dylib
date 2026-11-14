
#include "obf.h"
#include <stdlib.h>
#include <string.h>

char *obf_runtime_decode(const uint8_t *blob, size_t len, uint8_t key) {
    if (!blob || !len) return NULL;
    char *out = malloc(len + 1);
    if (!out) return NULL;
    for (size_t i = 0; i < len; i++)
        out[i] = (char)(blob[i] ^ key ^ (uint8_t)(i * 31));
    out[len] = 0;
    return out;
}
void obf_wipe(void *p, size_t n) {
    if (!p) return;
    volatile uint8_t *v = p;
    for (size_t i = 0; i < n; i++) v[i] = 0;
}
