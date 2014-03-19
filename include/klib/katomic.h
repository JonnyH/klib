#ifndef __KATOMIC_H
#define __KATOMIC_H

#include <stdint.h>
#include <stdbool.h>

#ifndef __GNUC__
#error only gcc supported
#endif

#if (__GNUC__ < 4)
#error atomic builtins seem to only exist in gcc 4.7 annd above
#endif
#if (__GNUC__ == 4 && __GNUC_MINOR__ < 7)
#error atomic builtins seem to only exist in gcc 4.7 annd above
#endif

typedef uint32_t katomic_t;

static inline uint32_t katomic_get(katomic_t *a)
{
	return __atomic_load_n(a,  __ATOMIC_SEQ_CST);
}

static inline void katomic_set(katomic_t *a, uint32_t v)
{
	__atomic_store_n(a, v, __ATOMIC_SEQ_CST);
}

static inline uint32_t katomic_add_return(katomic_t *a, uint32_t v)
{
	return __atomic_add_fetch(a, v, __ATOMIC_SEQ_CST);
}

static inline uint32_t katomic_sub_return(katomic_t *a, uint32_t v)
{
	return __atomic_sub_fetch(a, v, __ATOMIC_SEQ_CST);
}

static inline uint32_t katomic_dec_return(katomic_t *a)
{
	return katomic_sub_return(a, 1);
}

static inline uint32_t katomic_inc_return(katomic_t *a)
{
	return katomic_add_return(a, 1);
}

static inline uint32_t katomic_exchange(katomic_t *a, uint32_t v)
{
	return __atomic_exchange_n(a, v, __ATOMIC_SEQ_CST);
}

static inline bool katomic_compare_exchange(katomic_t *a, katomic_t *compare, uint32_t desired)
{
	return __atomic_compare_exchange_n(a, compare, desired, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

#endif /* __KATOMIC_H */
