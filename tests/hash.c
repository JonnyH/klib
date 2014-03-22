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

struct rhash_test_data
{
	uint32_t data[3];
};
void t_rhash(void)
{
	struct rhash_test_data d1, d2;
	uint32_t h1, h2;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			d1.data[j] = rand();
			d2.data[j] = rand();
		}
		h1 = krhash(&d1, sizeof(d1), 0);
		h2 = krhash(&d2, sizeof(d2), 0);
		for (int j = 0; j < 3; j++)
		{
			h2 = krhash_mod(h2, d2.data[j], d1.data[j], j);
			d2.data[j] = d1.data[j];
		}
		TEST_FAIL_IF(h1 != h2);
		TEST_FAIL_IF(h1 != krhash(&d1, sizeof(d1), 0));
		TEST_FAIL_IF(h2 != krhash(&d2, sizeof(d2), 0));
	}
	for (int i = 2; i >= 0; i--)
	{
		for (int j = 0; j < 3; j++)
		{
			d1.data[j] = i;
			d2.data[j] = j;
		}
		h1 = krhash(&d1, sizeof(d1), 0);
		h2 = krhash(&d2, sizeof(d2), 0);
		for (int j = 0; j < 3; j++)
		{
			h2 = krhash_mod(h2, d2.data[j], d1.data[j], j);
			d2.data[j] = d1.data[j];
		}
		TEST_FAIL_IF(h1 != h2);
		TEST_FAIL_IF(h1 != krhash(&d1, sizeof(d1), 0));
		TEST_FAIL_IF(h2 != krhash(&d2, sizeof(d2), 0));
	}
}
TEST_FN(t_rhash);
