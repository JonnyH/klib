#include "test.h"
#include "klib/klist.h"

struct dlist_struct
{
	int magic_before;
	struct kdlist_t list_member;
	int magic_after;
};

void d_empty(void)
{
	struct kdlist_t list_head;
	kdlist_init_head(&list_head);
	TEST_FAIL_IF(!kdlist_empty(&list_head));
	struct dlist_struct s = {0};
	kdlist_add(&list_head, &s.list_member);
	TEST_FAIL_IF(kdlist_empty(&list_head));
}
TEST_FN(d_empty);

void d_count(void)
{
	const int count = 100;
	struct kdlist_t list_head;
	kdlist_init_head(&list_head);
	struct dlist_struct *s = malloc(sizeof(struct dlist_struct) * count);
	for (int i = 0; i < count;i++)
		kdlist_add(&list_head, &s[i].list_member);
	TEST_FAIL_IF(kdlist_size(&list_head) != count);
	free(s);
}
TEST_FN(d_count);

void d_iterate(void)
{
	const int count = 100;
	int i_count = 0;
	struct kdlist_t list_head;
	struct kdlist_t *entry;
	kdlist_init_head(&list_head);
	struct dlist_struct *s = malloc(sizeof(struct dlist_struct) * count);
	for (int i = 0; i < count; i++)
	{
		kdlist_add_tail(&list_head, &s[i].list_member);
		s[i].magic_before = i;
		s[i].magic_after = i;
	}
	kdlist_for_each(&list_head, entry)
	{
		struct dlist_struct *e = kdlist_entry(entry, struct dlist_struct, list_member);
		TEST_FAIL_IF(e->magic_before != i_count);
		TEST_FAIL_IF(e->magic_after != i_count);
		i_count++;
	}
	TEST_FAIL_IF(i_count != count);
	free(s);
}
TEST_FN(d_iterate);

void d_riterate(void)
{
	const int count = 100;
	int i_count = 0;
	struct kdlist_t list_head;
	struct kdlist_t *entry;
	kdlist_init_head(&list_head);
	struct dlist_struct *s = malloc(sizeof(struct dlist_struct) * count);
	for (int i = 0; i < count; i++)
	{
		kdlist_add(&list_head, &s[i].list_member);
		s[i].magic_before = i;
		s[i].magic_after = i;
	}
	kdlist_for_each_rev(&list_head, entry)
	{
		struct dlist_struct *e = kdlist_entry(entry, struct dlist_struct, list_member);
		TEST_FAIL_IF(e->magic_before != i_count);
		TEST_FAIL_IF(e->magic_after != i_count);
		i_count++;
	}
	TEST_FAIL_IF(i_count != count);
	free(s);
}
TEST_FN(d_riterate);

void d_safeiterate(void)
{
	const int count = 100;
	int i_count = 0;
	struct kdlist_t list_head;
	struct kdlist_t *entry, *tmp;
	kdlist_init_head(&list_head);
	struct dlist_struct *s = malloc(sizeof(struct dlist_struct) * count);
	for (int i = 0; i < count; i++)
	{
		kdlist_add_tail(&list_head, &s[i].list_member);
		s[i].magic_before = i;
		s[i].magic_after = i;
	}
	kdlist_for_each_safe(&list_head, entry, tmp)
	{
		struct dlist_struct *e = kdlist_entry(entry, struct dlist_struct, list_member);
		TEST_FAIL_IF(e->magic_before != i_count);
		TEST_FAIL_IF(e->magic_after != i_count);
		i_count++;
	}
	TEST_FAIL_IF(i_count != count);
	free(s);
}
TEST_FN(d_safeiterate);

void d_saferiterate(void)
{
	const int count = 100;
	int i_count = 0;
	struct kdlist_t list_head;
	struct kdlist_t *entry, *tmp;
	kdlist_init_head(&list_head);
	struct dlist_struct *s = malloc(sizeof(struct dlist_struct) * count);
	for (int i = 0; i < count; i++)
	{
		kdlist_add(&list_head, &s[i].list_member);
		s[i].magic_before = i;
		s[i].magic_after = i;
	}
	kdlist_for_each_safe_rev(&list_head, entry, tmp)
	{
		struct dlist_struct *e = kdlist_entry(entry, struct dlist_struct, list_member);
		TEST_FAIL_IF(e->magic_before != i_count);
		TEST_FAIL_IF(e->magic_after != i_count);
		i_count++;
	}
	TEST_FAIL_IF(i_count != count);
	free(s);
}
TEST_FN(d_saferiterate);
