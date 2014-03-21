#include "klib/khash.h"

#include "test.h"

void t_genfunction(void)
{
	volatile uint8_t readBuffer[8];
	uint8_t buffer[8];
	for (int i = 0; i < 8; i++)
		buffer[i] = readBuffer[i];
	/* Relies on gcc nested functions */
	KHT_DEFINE_HASH_FN(fn1, sizeof(buffer));
	KHT_DEFINE_HASH_FN(fn2, 8);
	uint32_t h1 = fn1(buffer);
	for (int i = 0; i < 8; i++)
		buffer[i] = readBuffer[i];
	uint32_t h2 = fn2(buffer);
	TEST_FAIL_IF(h1 != h2);
}
TEST_FN(t_genfunction);

void t_genfunctionsimple(void)
{
	volatile uint8_t readBuffer[8];
	uint8_t buffer[8];
	for (int i = 0; i < 8; i++)
		buffer[i] = readBuffer[i];
	/* Relies on gcc nested functions */
	KHT_DEFINE_SIMPLE_HASH_FN(fn1, sizeof(buffer));
	KHT_DEFINE_SIMPLE_HASH_FN(fn2, 8);
	uint32_t h1 = fn1(buffer);
	for (int i = 0; i < 8; i++)
		buffer[i] = readBuffer[i];
	uint32_t h2 = fn2(buffer);
	TEST_FAIL_IF(h1 != h2);
}
TEST_FN(t_genfunctionsimple);

void t_string(void)
{
	const char string[] = "Hello world!";
	KHT_DEFINE_HASH_FN(fn1, strlen(ptr));
	KHT_DEFINE_HASH_FN(fn2, sizeof(string)-1);
	TEST_FAIL_IF(fn1(string) != fn2(string));
}
TEST_FN(t_string);
