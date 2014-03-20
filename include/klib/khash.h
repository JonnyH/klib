#ifndef __KHASH_H
#define __KHASH_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define KHT_DEFINE_HASH_FN(name, size_expr) \
inline uint32_t name (void *ptr) {uint32_t hash = 0; kht_murmur_hash(ptr, size_expr, 0, &hash); return hash;}

#define __ROTL32(num,amount) (((num) << (amount)) | ((num) >> (32 - (amount))))

static inline uint32_t __murmur_fmix ( uint32_t h )
{
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;

        return h;
}

static inline void kht_murmur_hash(const void *key, uint32_t length, uint32_t seed, uint32_t *out)
{
        const uint8_t * data = (const uint8_t*)key;
        const int nblocks = length / 4;

        uint32_t h1 = seed;

        uint32_t c1 = 0xcc9e2d51;
        uint32_t c2 = 0x1b873593;

        //----------
        // body

        const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

        for(int i = -nblocks; i; i++)
        {
                uint32_t k1 = blocks[i];

                k1 *= c1;
                k1 = __ROTL32(k1,15);
                k1 *= c2;

                h1 ^= k1;
                h1 = __ROTL32(h1,13);
                h1 = h1*5+0xe6546b64;
        }

        //----------
        // tail

        const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

        uint32_t k1 = 0;

        switch(length & 3)
        {
        case 3:
                k1 ^= tail[2] << 16;
        case 2:
                k1 ^= tail[1] << 8;
        case 1:
                k1 ^= tail[0];
                k1 *= c1;
                k1 = __ROTL32(k1,15);
                k1 *= c2;
                h1 ^= k1;
        };

        //----------
        // finalization

        h1 ^= length;

        h1 = __murmur_fmix(h1);

        *out = h1;
}

#endif //__KHASHTABLE_H
