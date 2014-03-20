#include "klib/khash.h"

#include "test.h"

void t_genfunction(void)
{
	uint8_t buffer[8];
	/* Relies on gcc nested functions */
	KHT_DEFINE_HASH_FN(fn1, sizeof(buffer));
	KHT_DEFINE_HASH_FN(fn2, 8);
	TEST_FAIL_IF(fn1(buffer) != fn2(buffer));
}
TEST_FN(t_genfunction);
