#include "klib/khashtable.h"
#include "klib/klist.h"
#include "klib/kexport.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>


struct kht_item_t
{
	struct kslist_t list;
	void* key;
	void* data;
	uint32_t hash;
};

struct kht_t
{
	struct kht_functions_t fns;
	size_t tableSize;
	size_t itemCount;
	struct kslist_t *table;
};

#ifndef MAX
#define MAX(x,y) ((x)>(y)?(x):(y))
#endif

#define ROTL32(num,amount) (((num) << (amount)) | ((num) >> (32 - (amount))))
//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

static uint32_t getblock ( const uint32_t * p, int i )
{
	return p[i];
}

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

static uint32_t fmix ( uint32_t h )
{
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;

	return h;
}

static void MurmurHash3_x86_32 ( const void * key, int len,
                          uint32_t seed, void * out )
{
	const uint8_t * data = (const uint8_t*)key;
	const int nblocks = len / 4;

	uint32_t h1 = seed;

	uint32_t c1 = 0xcc9e2d51;
	uint32_t c2 = 0x1b873593;

	//----------
	// body

	const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

	for(int i = -nblocks; i; i++)
	{
		uint32_t k1 = getblock(blocks,i);

		k1 *= c1;
		k1 = ROTL32(k1,15);
		k1 *= c2;

		h1 ^= k1;
		h1 = ROTL32(h1,13);
		h1 = h1*5+0xe6546b64;
	}

	//----------
	// tail

	const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

	uint32_t k1 = 0;

	switch(len & 3)
	{
	case 3:
		k1 ^= tail[2] << 16;
	case 2:
		k1 ^= tail[1] << 8;
	case 1:
		k1 ^= tail[0];
		k1 *= c1;
		k1 = ROTL32(k1,15);
		k1 *= c2;
		h1 ^= k1;
	};

	//----------
	// finalization

	h1 ^= len;

	h1 = fmix(h1);

	*(uint32_t*)out = h1;
}

KLIB_EXPORT uint32_t kht_hash_function(char* data, size_t dataLength)
{
	uint32_t hashOutput;
	static const uint32_t seed = 0;
	MurmurHash3_x86_32 (data, dataLength, seed, &hashOutput);
	return hashOutput;
}

KLIB_EXPORT struct kht_t* kht_create(size_t initialSize, struct kht_functions_t fns)
{
	struct kht_t *table = malloc(sizeof(struct kht_t));

	table->fns = fns;
	table->tableSize = initialSize;
	table->itemCount = 0;
	table->table = calloc(table->tableSize,sizeof(table->table));
	for (size_t listHead = 0; listHead < table->tableSize; listHead++)
	{
		kslist_init_head(&table->table[listHead]);
	}

	return table;
}

KLIB_EXPORT void kht_destroy(struct kht_t *table, kht_destroy_function_t destructor)
{
	size_t hashValue = 0;
	for (hashValue = 0; hashValue < table->tableSize; hashValue++)
	{
		struct kslist_t *entry, *tmp, *prev = &table->table[hashValue];
		kslist_for_each_safe(&table->table[hashValue], entry, tmp)
		{
			struct kht_item_t *item = klist_entry(entry, struct kht_item_t, list);
			kslist_del(entry, prev);
			if (destructor)
				destructor(item->key, item->data);
			free(item);
		}
	}
	free(table->table);
	free(table);
}

KLIB_EXPORT bool kht_add(struct kht_t *table, void *key, void *data)
{
	uint32_t hash = table->fns.hash_function(key);
	uint32_t tableOffset = hash % table->tableSize;
	if (kht_get(table, key) != NULL)
		return false;
	struct kht_item_t *item = malloc(sizeof(struct kht_item_t));
	item->data = data;
	item->key = key;
	item->hash = hash;
	kslist_add(&table->table[tableOffset], &item->list);

	/* TODO Check for resize */

	return true;
}

KLIB_EXPORT void* kht_get(struct kht_t *table, void* key)
{
	uint32_t hash = table->fns.hash_function(key);
	uint32_t tableOffset = hash % table->tableSize;
	struct kslist_t *entry;
	kslist_for_each(&table->table[tableOffset], entry)
	{
		struct kht_item_t *item = klist_entry(entry, struct kht_item_t, list);
		if (item->hash == hash &&
		    table->fns.compare_function(key, item->key) == 0)
		{
			return item->data;
		}
	}
	return NULL;
}

KLIB_EXPORT bool kht_remove(struct kht_t *table, void *key, void **removedKey, void **removedData)
{
	uint32_t hash = table->fns.hash_function(key);
	uint32_t tableOffset = hash % table->tableSize;
	struct kslist_t *entry, *tmp, *prev = &table->table[tableOffset];
	kslist_for_each_safe(&table->table[tableOffset], entry, tmp)
	{
		struct kht_item_t *item = klist_entry(entry, struct kht_item_t, list);
		if (item->hash == hash &&
		    table->fns.compare_function(key, item->key) == 0)
		{
			kslist_del(entry, prev);
			if (removedKey)
				*removedKey = item->key;
			if (removedData)
				*removedData = item->data;
			free(item);
			/* TODO Check for resize */
			return true;
		}
		prev = entry;
	}
	return false;
}

KLIB_EXPORT void kht_resize(struct kht_t *table, size_t newSize)
{
	/* TODO Resize */
}

struct kht_iterator_t
{
	struct kht_t *table;
	uint32_t tableOffset;
	uint32_t listOffset;
};

KLIB_EXPORT struct kht_iterator_t* kht_iterator_create(struct kht_t *table)
{
	struct kht_iterator_t* it = malloc(sizeof(struct kht_iterator_t));
	it->table = table;
	it->tableOffset = 0;
	it->listOffset = 0;
	return it;
}

KLIB_EXPORT void kht_iterator_destroy(struct kht_iterator_t *it)
{
	free(it);
}

KLIB_EXPORT bool kht_iterator_next(struct kht_iterator_t *it, void **key, void **data)
{
	while (it->tableOffset < it->table->tableSize)
	{
		struct kslist_t *entry;
		int listOffset = 0;
		kslist_for_each(&it->table->table[it->tableOffset], entry)
		{
			struct kht_item_t *item = klist_entry(entry, struct kht_item_t, list);
			if (listOffset == it->listOffset)
			{
				it->listOffset++;
				if (key)
					*key = item->key;
				if (data)
					*data = item->data;
				return true;
			}
			listOffset++;
		}
		it->listOffset = 0;
		it->tableOffset++;
	}
	return false;
}
