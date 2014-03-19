#ifndef __KSTRING_H
#define __KSTRING_H

#include "khashtable.h"

#include <stddef.h>

struct kstring_t;

struct kstring_t *kstr_create(size_t initialSize);
void kstr_destroy(struct kstring_t *str);
char* kstr_cstr(const struct kstring_t *str);
/* Appends the printf-stype format/args to 'str' */
void kstr_printf(struct kstring_t *str, const char* format, ...)
	__attribute__ ((format (printf, 2, 3)));
size_t kstr_length(const struct kstring_t *str);

uint32_t kstr_hash(const struct kstring_t *str);
int kstr_cmp(const struct kstring_t *strA, const struct kstring_t *strB);

static const struct kht_functions_t kstr_hash_functions =
{
	.hash_function = (kht_hash_function_t)kstr_hash,
	.compare_function = (kht_compare_function_t)kstr_cmp,
};

#endif
