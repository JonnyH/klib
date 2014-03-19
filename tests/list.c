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
TEST_FN(d_empty)

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
TEST_FN(d_count)
