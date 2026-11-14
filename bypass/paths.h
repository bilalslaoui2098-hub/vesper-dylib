#ifndef PATHS_H
#define PATHS_H
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct { const uint8_t *b; size_t l; uint8_t k; } obf_e;

// مسارات مخفية (Placeholders، عوضهم بالـ obfuscated blobs إذا بغيتي)
static const uint8_t p0[]  = {0x01, 0x02};
static const uint8_t p1[]  = {0x01, 0x02};
static const uint8_t p2[]  = {0x01, 0x02};
static const uint8_t p3[]  = {0x01, 0x02};
static const uint8_t p4[]  = {0x01, 0x02};
static const uint8_t p5[]  = {0x01, 0x02};
static const uint8_t p6[]  = {0x01, 0x02};
static const uint8_t p7[]  = {0x01, 0x02};
static const uint8_t p8[]  = {0x01, 0x02};
static const uint8_t p9[]  = {0x01, 0x02};
static const uint8_t p10[] = {0x01, 0x02};
static const uint8_t p11[] = {0x01, 0x02};

static const obf_e kHiddenBlobs[] = {
    {p0,sizeof(p0),0xA7}, {p1,sizeof(p1),0xA7}, {p2,sizeof(p2),0xA7}, {p3,sizeof(p3),0xA7},
    {p4,sizeof(p4),0xA7}, {p5,sizeof(p5),0xA7}, {p6,sizeof(p6),0xA7}, {p7,sizeof(p7),0xA7},
    {p8,sizeof(p8),0xA7}, {p9,sizeof(p9),0xA7}, {p10,sizeof(p10),0xA7}, {p11,sizeof(p11),0xA7}
};

extern char *g_dec_paths[sizeof(kHiddenBlobs)/sizeof(kHiddenBlobs[0])];
extern bool  g_paths_ready;
void paths_init(void);
bool path_is_hidden(const char *p);
#endif
