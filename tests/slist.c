#include "test.h"
#include "klib/klist.h"

struct slist_struct
{
	int magic_before;
	struct kslist_t list_member;
	int magic_after;
};

void s_empty(void)
{
	struct kslist_t list_head;
	kslist_init_head(&list_head);
	TEST_FAIL_IF(!kslist_empty(&list_head));
	struct slist_struct s = {0};
	kslist_add(&list_head, &s.list_member);
	TEST_FAIL_IF(kslist_empty(&list_head));
}
TEST_FN(s_empty);

void s_count(void)
{
	const int count = 100;
	struct kslist_t list_head;
	kslist_init_head(&list_head);
	struct slist_struct *s = malloc(sizeof(struct slist_struct) * count);
	for (int i = 0; i < count;i++)
		kslist_add(&list_head, &s[i].list_member);
	TEST_FAIL_IF(kslist_size(&list_head) != count);
	free(s);
}
TEST_FN(s_count);

void s_iterate(void)
{
	const int count = 100;
	int i_count = 0;
	struct kslist_t list_head;
	struct kslist_t *entry;
	kslist_init_head(&list_head);
	struct slist_struct *s = malloc(sizeof(struct slist_struct) * count);
	for (int i = 0; i < count; i++)
	{
		kslist_add_tail(&list_head, &s[i].list_member);
		s[i].magic_before = i;
		s[i].magic_after = i;
	}
	kslist_for_each(&list_head, entry)
	{
		struct slist_struct *e = kslist_entry(entry, struct slist_struct, list_member);
		TEST_FAIL_IF(e->magic_before != i_count);
		TEST_FAIL_IF(e->magic_after != i_count);
		i_count++;
	}
	TEST_FAIL_IF(i_count != count);
	free(s);
}
TEST_FN(s_iterate);

void s_riterate(void)
{
	const int count = 100;
	int i_count = 0;
	struct kslist_t list_head;
	struct kslist_t *entry;
	kslist_init_head(&list_head);
	struct slist_struct *s = malloc(sizeof(struct slist_struct) * count);
	for (int i = 0; i < count; i++)
	{
		kslist_add(&list_head, &s[i].list_member);
		s[i].magic_before = i;
		s[i].magic_after = i;
	}
	kslist_for_each_rev(&list_head, entry)
	{
		struct slist_struct *e = kslist_entry(entry, struct slist_struct, list_member);
		TEST_FAIL_IF(e->magic_before != i_count);
		TEST_FAIL_IF(e->magic_after != i_count);
		i_count++;
	}
	TEST_FAIL_IF(i_count != count);
	free(s);
}
TEST_FN(s_riterate);

void s_previous(void)
{
	struct kslist_t list_head;
	kslist_init_head(&list_head);
	TEST_FAIL_IF(__kslist_prev(&list_head, &list_head) != &list_head);
	struct kslist_t m1;
	kslist_add(&list_head, &m1);
	TEST_FAIL_IF(__kslist_prev(&list_head, &m1) != &list_head);
	TEST_FAIL_IF(__kslist_prev(&list_head, &list_head) != &m1);
}
TEST_FN(s_previous);
