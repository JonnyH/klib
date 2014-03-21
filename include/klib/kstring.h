#ifndef __KSTRING_H
#define __KSTRING_H

#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

struct kstring
{
	uint32_t allocated_size;
	uint32_t used_size;
	uint8_t *cstr;
};

static inline void kstring_init(struct kstring *str)
{
	str->allocated_size = 0;
	str->used_size = 0;
	str->cstr = NULL;
}

static inline void kstr_destroy(struct kstring *str)
{
	free(str->cstr);
	kstring_init(str);
}

__attribute__ ((format (printf, 2, 3)))
static inline void kstr_printf(struct kstring *str, const char* format, ...)
{
        va_list argList;
        int sizeLeft, sizeWritten;

        sizeLeft = str->allocated_size - str->used_size;

        va_start(argList, format);

        sizeWritten = vsnprintf(str->cstr + str->used_size, sizeLeft, format, argList);

        va_end(argList);
        while (sizeWritten >= sizeLeft)
        {
                str->allocated_size *= 2;
                str->cstr = realloc(str->cstr, str->allocated_size);
                sizeLeft = str->allocated_size - str->used_size;
                va_start(argList, format);
                sizeWritten = vsnprintf(str->cstr + str->used_size, sizeLeft, format, argList);
                va_end(argList);
        }

        str->used_size += sizeWritten;
}

#endif
