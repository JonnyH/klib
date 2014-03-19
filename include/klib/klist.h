#ifndef __KLIST_H
#define __KLIST_H

#include "kcontainer_of.h"

#include <stddef.h>
#include <stdbool.h>

struct kdlist_t
{
	struct kdlist_t *next;
	struct kdlist_t *prev;
};

#define kdlist_for_each(head, entry) \
	for (entry = (head)->next; entry != (head); entry = entry->next)

#define kdlist_for_each_safe(head, entry, tmp) \
	for (entry = (head)->next, tmp = entry->next; entry != (head); entry = tmp, tmp = entry->next)

#define kdlist_entry(ptr, type, member) \
	kcontainer_of(ptr,type,member)

void kdlist_init_head(struct kdlist_t *head);
void kdlist_add_at(struct kdlist_t *member, struct kdlist_t *prev, struct kdlist_t *next);
void kdlist_add(struct kdlist_t *head, struct kdlist_t *member);
void kdlist_add_tail(struct kdlist_t *head, struct kdlist_t *member);
void kdlist_del_at(struct kdlist_t *prev, struct kdlist_t *next);
void kdlist_del(struct kdlist_t *entry);

static inline bool kdlist_empty(struct kdlist_t *head)
{
	return (head->next == head);
}




static inline unsigned int kdlist_size(struct kdlist_t *head)
{
	unsigned int count = 0;
	struct kdlist_t *entry;
	kdlist_for_each(head, entry)
		count++;
	return count;
}

#define KSLIST_EMPTY NULL

struct kslist_t
{
	struct kslist_t *next;
};


#define kslist_for_each(head, entry) \
	for (entry = (head)->next; entry != KSLIST_EMPTY; entry = entry->next)

#define kslist_for_each_safe(head, entry, tmp) \
	for (entry = (head)->next, tmp = (entry == KSLIST_EMPTY ? KSLIST_EMPTY : entry->next); entry != KSLIST_EMPTY; entry = tmp, tmp = entry->next)

#define kslist_entry(ptr, type, member) \
	kcontainer_of(ptr,type,member)

void kslist_init_head(struct kslist_t *head);
void kslist_add_at(struct kslist_t *member, struct kslist_t *prev);
void kslist_add(struct kslist_t *head, struct kslist_t *member);
void kslist_add_tail(struct kslist_t *head, struct kslist_t *member);
void kslist_del_at(struct kslist_t *prev, struct kslist_t *next);
void kslist_del(struct kslist_t *entry, struct kslist_t *prev);

#endif /* __KLIST_H */
