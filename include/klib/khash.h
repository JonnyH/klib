#ifndef __KHASH_H
#define __KHASH_H

#include <stdint.h>

#define KHT_DEFINE_HASH_FN(name, size_expr) \
inline uint32_t name (const void *ptr) {return kht_murmur_hash(ptr, size_expr, 0);}

#define KHT_DEFINE_SIMPLE_HASH_FN(name, size_expr) \
inline uint32_t name (const void *ptr) {return kht_simple_hash(ptr, size_expr, 0);}

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

static inline uint32_t kht_murmur_hash(const void *key, uint32_t length, uint32_t seed)
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

        return h1;
}

static inline uint32_t __simple_mix(uint32_t h1, uint32_t k1)
{
	/* Constant from murmur3 */
	const uint32_t c1 = 0xcc9e2d51;

	h1 ^= c1;
	h1 += k1;
	h1 = __ROTL32(h1, 11);
	h1 ^= k1;
	h1 = __ROTL32(h1, 17);

	return h1;
}

static inline uint32_t kht_simple_hash(const void *key, uint32_t length, uint32_t seed)
{
	uint32_t h = seed;
	const uint32_t *ptr = key;
	unsigned int blocks = length / 4;
	uint32_t k = 0;
	while (--blocks)
	{
		h = __simple_mix(h, *(ptr++));
	}

	const uint8_t *tail = (const uint8_t*)(key + blocks * 4);
	switch (length%3)
	{
		case 3:
			k ^= tail[2] << 16;
		case 2:
			k ^= tail[1] << 8;
		case 1:
			k ^= tail[0];
			h = __simple_mix(h,k);
		case 0:
			break;
			/* no action */
	}
	return h;
}

#endif //__KHASHTABLE_H
