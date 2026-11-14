#ifndef OBF_H
#define OBF_H
#include <stdint.h>
#include <stddef.h>

char *obf_runtime_decode(const uint8_t *blob, size_t len, uint8_t key);
void  obf_wipe(void *p, size_t len);
#endif
