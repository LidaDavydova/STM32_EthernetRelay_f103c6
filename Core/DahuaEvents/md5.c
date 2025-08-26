#include <string.h>
#include "md5.h"

// Удалены все проверки на другие архитектуры, оставляем только надежный little-endian вариант
// Переименовал макросы в более простые и убрал лишние скобки где возможно

#define F(x, y, z)    ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z)    ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z)    ((x) ^ (y) ^ (z))
#define I(x, y, z)    ((y) ^ ((x) | ~(z)))

// Упрощенный макрос STEP. Теперь он явно использует функцию раунда
#define STEP(round_func, a, b, c, d, x, t, s) \
    a += round_func(b, c, d) + x + t; \
    a = (a << s) | (a >> (32 - s)); \
    a += b;

// Функция для корректного копирования данных из uint8_t[] в uint32_t[] (little-endian)
static inline uint32_t get_word(const uint8_t *data, int n) {
    // Явное копирование для избежания unaligned access на ARM
    return (uint32_t)data[n*4] |
          ((uint32_t)data[n*4+1] << 8) |
          ((uint32_t)data[n*4+2] << 16) |
          ((uint32_t)data[n*4+3] << 24);
}

// Основная функция преобразования блока
static void md5_transform(MD5_CTX *ctx, const uint8_t data[64]) {
    uint32_t a = ctx->a;
    uint32_t b = ctx->b;
    uint32_t c = ctx->c;
    uint32_t d = ctx->d;

    uint32_t x[16];

    // Копируем данные во временный буфер гарантированно правильным образом
    for(int i = 0; i < 16; i++) {
        x[i] = get_word(data, i);
    }

    // Round 1
    STEP(F, a, b, c, d, x[0],  0xd76aa478,  7);
    STEP(F, d, a, b, c, x[1],  0xe8c7b756, 12);
    STEP(F, c, d, a, b, x[2],  0x242070db, 17);
    STEP(F, b, c, d, a, x[3],  0xc1bdceee, 22);
    STEP(F, a, b, c, d, x[4],  0xf57c0faf,  7);
    STEP(F, d, a, b, c, x[5],  0x4787c62a, 12);
    STEP(F, c, d, a, b, x[6],  0xa8304613, 17);
    STEP(F, b, c, d, a, x[7],  0xfd469501, 22);
    STEP(F, a, b, c, d, x[8],  0x698098d8,  7);
    STEP(F, d, a, b, c, x[9],  0x8b44f7af, 12);
    STEP(F, c, d, a, b, x[10], 0xffff5bb1, 17);
    STEP(F, b, c, d, a, x[11], 0x895cd7be, 22);
    STEP(F, a, b, c, d, x[12], 0x6b901122,  7);
    STEP(F, d, a, b, c, x[13], 0xfd987193, 12);
    STEP(F, c, d, a, b, x[14], 0xa679438e, 17);
    STEP(F, b, c, d, a, x[15], 0x49b40821, 22);

    // Round 2
    STEP(G, a, b, c, d, x[1],  0xf61e2562,  5);
    STEP(G, d, a, b, c, x[6],  0xc040b340,  9);
    STEP(G, c, d, a, b, x[11], 0x265e5a51, 14);
    STEP(G, b, c, d, a, x[0],  0xe9b6c7aa, 20);
    STEP(G, a, b, c, d, x[5],  0xd62f105d,  5);
    STEP(G, d, a, b, c, x[10], 0x02441453,  9);
    STEP(G, c, d, a, b, x[15], 0xd8a1e681, 14);
    STEP(G, b, c, d, a, x[4],  0xe7d3fbc8, 20);
    STEP(G, a, b, c, d, x[9],  0x21e1cde6,  5);
    STEP(G, d, a, b, c, x[14], 0xc33707d6,  9);
    STEP(G, c, d, a, b, x[3],  0xf4d50d87, 14);
    STEP(G, b, c, d, a, x[8],  0x455a14ed, 20);
    STEP(G, a, b, c, d, x[13], 0xa9e3e905,  5);
    STEP(G, d, a, b, c, x[2],  0xfcefa3f8,  9);
    STEP(G, c, d, a, b, x[7],  0x676f02d9, 14);
    STEP(G, b, c, d, a, x[12], 0x8d2a4c8a, 20);

    // Round 3
    STEP(H, a, b, c, d, x[5],  0xfffa3942,  4);
    STEP(H, d, a, b, c, x[8],  0x8771f681, 11);
    STEP(H, c, d, a, b, x[11], 0x6d9d6122, 16);
    STEP(H, b, c, d, a, x[14], 0xfde5380c, 23);
    STEP(H, a, b, c, d, x[1],  0xa4beea44,  4);
    STEP(H, d, a, b, c, x[4],  0x4bdecfa9, 11);
    STEP(H, c, d, a, b, x[7],  0xf6bb4b60, 16);
    STEP(H, b, c, d, a, x[10], 0xbebfbc70, 23);
    STEP(H, a, b, c, d, x[13], 0x289b7ec6,  4);
    STEP(H, d, a, b, c, x[0],  0xeaa127fa, 11);
    STEP(H, c, d, a, b, x[3],  0xd4ef3085, 16);
    STEP(H, b, c, d, a, x[6],  0x04881d05, 23);
    STEP(H, a, b, c, d, x[9],  0xd9d4d039,  4);
    STEP(H, d, a, b, c, x[12], 0xe6db99e5, 11);
    STEP(H, c, d, a, b, x[15], 0x1fa27cf8, 16);
    STEP(H, b, c, d, a, x[2],  0xc4ac5665, 23);

    // Round 4
    STEP(I, a, b, c, d, x[0],  0xf4292244,  6);
    STEP(I, d, a, b, c, x[7],  0x432aff97, 10);
    STEP(I, c, d, a, b, x[14], 0xab9423a7, 15);
    STEP(I, b, c, d, a, x[5],  0xfc93a039, 21);
    STEP(I, a, b, c, d, x[12], 0x655b59c3,  6);
    STEP(I, d, a, b, c, x[3],  0x8f0ccc92, 10);
    STEP(I, c, d, a, b, x[10], 0xffeff47d, 15);
    STEP(I, b, c, d, a, x[1],  0x85845dd1, 21);
    STEP(I, a, b, c, d, x[8],  0x6fa87e4f,  6);
    STEP(I, d, a, b, c, x[15], 0xfe2ce6e0, 10);
    STEP(I, c, d, a, b, x[6],  0xa3014314, 15);
    STEP(I, b, c, d, a, x[13], 0x4e0811a1, 21);
    STEP(I, a, b, c, d, x[4],  0xf7537e82,  6);
    STEP(I, d, a, b, c, x[11], 0xbd3af235, 10);
    STEP(I, c, d, a, b, x[2],  0x2ad7d2bb, 15);
    STEP(I, b, c, d, a, x[9],  0xeb86d391, 21);

    ctx->a += a;
    ctx->b += b;
    ctx->c += c;
    ctx->d += d;
}

void MD5_Init(MD5_CTX *ctx) {
    memset(ctx, 0, sizeof(*ctx)); // Обнуляем всю структуру
    ctx->a = 0x67452301;
    ctx->b = 0xefcdab89;
    ctx->c = 0x98badcfe;
    ctx->d = 0x10325476;
}

void MD5_Update(MD5_CTX *ctx, const void *vdata, uint32_t len) {
    const uint8_t *data = (const uint8_t*)vdata;
    uint32_t have;

    // Вычисляем сколько байт уже есть в буфере
    have = (ctx->lo >> 3) & 0x3F; // lo - счетчик бит, сдвигаем чтобы получить байты

    // Обновляем счетчик бит
    ctx->lo += len << 3;
    if(ctx->lo < (len << 3)) ctx->hi++; // Перенос если было переполнение
    ctx->hi += len >> 29;

    if(have) { // Если в буфере уже есть данные
        uint32_t need = 64 - have; // Сколько нужно до полного блока
        if(len < need) {
            memcpy(ctx->buffer + have, data, len);
            return;
        }
        memcpy(ctx->buffer + have, data, need);
        md5_transform(ctx, ctx->buffer);
        data += need;
        len -= need;
    }

    while(len >= 64) { // Обрабатываем полные блоки
        md5_transform(ctx, data);
        data += 64;
        len -= 64;
    }

    if(len) { // Сохраняем оставшиеся данные
        memcpy(ctx->buffer, data, len);
    }
}

void MD5_Final(uint8_t digest[16], MD5_CTX *ctx) {
    uint32_t size;
    uint32_t padlen;

    // Сохраняем размер сообщения в битах
    uint64_t bits = ((uint64_t)ctx->hi << 32) | ctx->lo;

    // Паддинг: добавляем 0x80 и нули
    size = (ctx->lo >> 3) & 0x3F; // Размер текущих данных в буфере в байтах
    padlen = (size < 56) ? (56 - size) : (120 - size);

    MD5_Update(ctx, "\x80", 1); // Добавляем байт 0x80
    padlen--;

    // Добавляем нули
    if(padlen) {
        uint8_t zeros[64] = {0};
        MD5_Update(ctx, zeros, padlen);
    }

    // Добавляем размер сообщения в битах (little-endian)
    uint8_t length_bytes[8];
    for(int i = 0; i < 8; i++) {
        length_bytes[i] = (bits >> (i * 8)) & 0xFF;
    }
    MD5_Update(ctx, length_bytes, 8);

    // Формируем итоговый хэш (little-endian)
    for(int i = 0; i < 4; i++) {
        digest[i]      = (ctx->a >> (i * 8)) & 0xFF;
        digest[i + 4]  = (ctx->b >> (i * 8)) & 0xFF;
        digest[i + 8]  = (ctx->c >> (i * 8)) & 0xFF;
        digest[i + 12] = (ctx->d >> (i * 8)) & 0xFF;
    }

    // Очищаем контекст
    memset(ctx, 0, sizeof(*ctx));
}
