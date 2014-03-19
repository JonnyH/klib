#ifndef __KHASHTABLE_H
#define __KHASHTABLE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Returns 32bit hash of *key */
typedef uint32_t (*kht_hash_function_t)(void* key);
/* Returns '0' is *key1 == *key2, otherwise they are assumed to be not equal */
typedef int (*kht_compare_function_t)(void* key1, void* key2);
/* Called at kht_destroy() for every key/data pair left in the table */
typedef void (*kht_destroy_function_t)(void* key, void* data);

struct kht_functions_t
{
	kht_hash_function_t hash_function;
	kht_compare_function_t compare_function;
};

struct kht_t;
struct kht_iterator_t;

uint32_t kht_hash_function(char* data, size_t dataLength);

struct kht_t *kht_create(size_t initialSize, struct kht_functions_t fns);

void kht_destroy(struct kht_t *table, kht_destroy_function_t destructor);

/* Returns false if failed to add (e.g. already have item with that key) */
bool kht_add(struct kht_t *table, void* key, void* data);

void* kht_get(struct kht_t *table, void* key);

/* Returns false if failed to remove (e.g. key doesn't exist in table) */
/* removedKey/removedData - optionally non-NULL - pointers to the data removed from the table
 * (for freeing etc.) */
bool kht_remove(struct kht_t *table, void* key, void **removedKey, void **removedData);

void kht_resize(struct kht_t *table, size_t newSize);

struct kht_iterator_t *kht_iterator_create(struct kht_t *table);
void kht_iterator_destroy(struct kht_iterator_t *it);

bool kht_iterator_next(struct kht_iterator_t *it, void **key, void **data);

#endif //__KHASHTABLE_H
