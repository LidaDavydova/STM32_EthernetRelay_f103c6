#ifndef MD5_H
#define MD5_H

#include <stdint.h>

typedef struct {
    uint32_t lo, hi;
    uint32_t a, b, c, d;
    uint8_t buffer[64];
} MD5_CTX;

void MD5_Init(MD5_CTX *ctx);
void MD5_Update(MD5_CTX *ctx, const void *data, uint32_t len);
void MD5_Final(uint8_t digest[16], MD5_CTX *ctx);

#endif
