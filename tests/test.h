#ifndef __TEST_H
#define __TEST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool globalTestResult = true;
static bool testStatus = true;
static const char *testName = NULL;

struct __test_definition
{
	const char *name;
	void (*fn)(void);
	struct __test_definition *next;
};

static struct __test_definition *__tests = NULL;

static void __add_test(const char *name, void (*fn)(void))
{
	struct __test_definition *def = malloc(sizeof(struct __test_definition));
	def->name = name;
	def->fn = fn;
	def->next = __tests;
	__tests = def;
}

void __test_fail(const char *expr, const char *fn, unsigned int line)
{
	printf("FAIL: (%s) failed at %s:%u\n", expr, fn, line);
	testStatus = false;
}

void __run_test(const char *name, void (*fn)(void))
{
	testName = name;
	testStatus = true;
	printf("%s:\t", name);
	fflush(stdout);
	fn();
	if (testStatus == false)
	{
		globalTestResult = false;
		printf("%s FAILED\n", testName);
	}
	else
	{
		printf("\n%s PASSED\n", testName);
	}
}

#define xstr(s) str(s)
#define str(s) #s
#define __concat_i(x,y) x##y
#define __concat(x,y) __concat_i(x,y)

#define TEST_FN(x) \
	static void __attribute__((constructor)) __concat(__TEST_INIT_, __COUNTER__ )(void) {__add_test(xstr(x), x);}

#define TEST_FAIL_IF(x) \
	if (x) __test_fail(xstr(x), __func__, __LINE__);

int main(int argc, char **argv)
{
	if (argc > 1)
	{
		for (int testNr = 1; testNr < argc; testNr++)
		{
			struct __test_definition *t = __tests;
			while (t)
			{
				if (strcmp(argv[testNr], t->name) == 0)
				{
					__run_test(t->name, t->fn);
					break;
				}
				t = t->next;
			}
			if (!t)
			{
				printf("TEST %s NOT FOUND\n%s FAILED\n", argv[testNr], argv[testNr]);
				globalTestResult = false;
			}
		}
	}
	else
	{
		struct __test_definition *t = __tests;
		while (t)
		{
			__run_test(t->name, t->fn);
			t = t->next;
		}
	}
	if (globalTestResult)
	{
		printf("Test success\n");
		return EXIT_SUCCESS;
	}
	else
	{
		printf("Test failure\n");
		return EXIT_FAILURE;
	}
}


#endif /* __TEST_H */
