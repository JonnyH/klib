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

#define kdlist_for_each_rev(head, entry) \
	for (entry = (head)->prev; entry != (head); entry = entry->prev)

#define kdlist_for_each_safe(head, entry, tmp) \
	for (entry = (head)->next, tmp = entry->next; entry != (head); entry = tmp, tmp = entry->next)

#define kdlist_entry(ptr, type, member) \
	kcontainer_of(ptr,type,member)

static inline void kdlist_init_head(struct kdlist_t *head)
{
	head->next = head;
	head->prev = head;
}
static inline void kdlist_add(struct kdlist_t *head, struct kdlist_t *member)
{
	member->next = head->next;
	member->prev = head;
	head->next = member;
	member->next->prev = member;
}
static inline void kdlist_add_tail(struct kdlist_t *head, struct kdlist_t *member)
{
	member->next = head;
	member->prev = head->prev;
	head->prev = member;
	member->prev->next = member;
}
static inline void kdlist_del(struct kdlist_t *entry)
{
	entry->prev->next = entry->next;
	entry->next->prev = entry->prev;
}

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

struct kslist_t
{
	struct kslist_t *next;
};

static inline bool kslist_empty(struct kslist_t *head)
{
	return (head->next == head);
}

#define kslist_for_each(head, entry) \
	for (entry = (head)->next; entry != head; entry = entry->next)

#define kslist_for_each_safe(head, entry, tmp) \
	for (entry = (head)->next, tmp = entry->next; entry != head; entry = tmp, tmp = entry->next)
static inline struct kslist_t* __kslist_prev(struct kslist_t *head, struct kslist_t *node)
{
	struct kslist_t *pos;
	kslist_for_each(head, pos)
	{
		if (pos->next == node)
			return pos;
	}
	return head;
}

static inline unsigned int kslist_size(struct kslist_t *head)
{
	unsigned int count = 0;
	struct kslist_t *entry;
	kslist_for_each(head, entry)
		count++;
	return count;
}

#define kslist_for_each_rev(head, entry) \
	for (entry = __kslist_prev(head, head); entry != head; entry = __kslist_prev(head, entry))


#define kslist_for_each_safe_rev(head, entry, tmp) \
	for (entry = __kslist_prev(head, head), tmp = __kslist_prev(entry); entry != head; entry = tmp, tmp = __kslist_prev(entry))



#define kslist_entry(ptr, type, member) \
	kcontainer_of(ptr,type,member)

static inline void kslist_init_head(struct kslist_t *head)
{
	head->next = head;
}

static inline void kslist_add(struct kslist_t *head, struct kslist_t *member)
{
	member->next = head->next;
	head->next = member;
}

static inline void kslist_del(struct kslist_t *head, struct kslist_t *member)
{
	struct kslist_t *prev = __kslist_prev(head, head);
	prev->next = member->next;
}

static inline void kslist_add_tail(struct kslist_t *head, struct kslist_t *member)
{
	kslist_add(__kslist_prev(head, head), member);
}

#endif /* __KLIST_H */
