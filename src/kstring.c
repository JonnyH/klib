#include "klib/kstring.h"
#include "klib/kexport.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

struct kstring_t
{
	size_t stringLength;
	size_t allocatedLength;
	char* stringData;
};

KLIB_EXPORT struct kstring_t *kstr_create(size_t initialSize)
{
	struct kstring_t *str = malloc(sizeof(struct kstring_t));
	const size_t defaultSize = 11;

	str->stringLength = 0;
	str->allocatedLength = MAX(initialSize, defaultSize);
	str->stringData = malloc(str->allocatedLength);
	str->stringData[0] = '\0';

	return str;
}

KLIB_EXPORT void kstr_destroy(struct kstring_t *str)
{
	free(str->stringData);
	free(str);
}

KLIB_EXPORT char* kstr_cstr(const struct kstring_t *str)
{
	return str->stringData;
}

KLIB_EXPORT void kstr_printf(struct kstring_t *str, const char* format, ...)
{
	va_list argList;
	int sizeLeft, sizeWritten;

	sizeLeft = str->allocatedLength - str->stringLength;

	va_start(argList, format);

	sizeWritten = vsnprintf(&str->stringData[str->stringLength], sizeLeft, format, argList);

	va_end(argList);
	while (sizeWritten >= sizeLeft)
	{
		str->allocatedLength *= 2;
		str->stringData = realloc(str->stringData, str->allocatedLength);
		sizeLeft = str->allocatedLength - str->stringLength;
		va_start(argList, format);
		sizeWritten = vsnprintf(&str->stringData[str->stringLength], sizeLeft, format, argList);
		va_end(argList);
	}

	str->stringLength += sizeWritten;
}

KLIB_EXPORT size_t kstr_length(const struct kstring_t *str)
{
	return str->stringLength;
}

KLIB_EXPORT uint32_t kstr_hash(const struct kstring_t *str)
{
	return kht_hash_function(kstr_cstr(str), kstr_length(str));
}

KLIB_EXPORT int kstr_cmp(const struct kstring_t *strA, const struct kstring_t *strB)
{
	if (kstr_length(strA) != kstr_length(strB))
		return 1;
	return strncmp(kstr_cstr(strA), kstr_cstr(strB), kstr_length(strA));
}
