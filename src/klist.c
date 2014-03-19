#include "klib/klist.h"
#include "klib/kexport.h"

KLIB_EXPORT void kdlist_init_head(struct kdlist_t *head)
{
	head->next = head;
	head->prev = head;
}

KLIB_EXPORT void kdlist_add_at(struct kdlist_t *member, struct kdlist_t *prev, struct kdlist_t *next)
{
	next->prev = member;
	member->next = next;
	member->prev = prev;
	prev->next = member;
}

KLIB_EXPORT void kdlist_add(struct kdlist_t *head, struct kdlist_t *member)
{
	kdlist_add_at(member, head, head->next);
}

KLIB_EXPORT void kdlist_add_tail(struct kdlist_t *head, struct kdlist_t *member)
{
	kdlist_add_at(member, head->prev, head);
}

KLIB_EXPORT void kdlist_del_at(struct kdlist_t *prev, struct kdlist_t *next)
{
	next->prev = prev;
	prev->next = next;
}

KLIB_EXPORT void kdlist_del(struct kdlist_t *entry)
{
	kdlist_del_at(entry->prev, entry->next);
}


void kslist_init_head(struct kslist_t *head)
{
	head->next = KSLIST_EMPTY;
}
void kslist_add_at(struct kslist_t *member, struct kslist_t *prev)
{
	member->next = prev->next;
	prev->next = member;
}

void kslist_add(struct kslist_t *head, struct kslist_t *member)
{
	kslist_add_at(member, head);
}

void kslist_del_at(struct kslist_t *prev, struct kslist_t *next)
{
	prev->next = next;
}

void kslist_del(struct kslist_t *entry, struct kslist_t *prev)
{
	kslist_del_at(prev, entry->next);
}
