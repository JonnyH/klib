#ifndef __KBINTREE_H
#define __KBINTREE_H

#include "kcontainer_of.h"

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

typedef uint32_t kbintree_key_t;
#define KBINTREE_DEBUG_KEY_FORMAT_STRING "0x%08x"

struct kbintree_t
{
	kbintree_key_t key;
	struct kbintree_t *children[2];
	struct kbintree_t *parent;
};

static inline void kbintree_init_head(struct kbintree_t *head)
{
	unsigned int i = 0;
	head->key = 0;
	head->parent = NULL;
	for (i = 0; i < 2; i++)
		head->children[i] = NULL;
}

static inline void __kbintree_insert(struct kbintree_t *parent, struct kbintree_t *node)
{
	int i;
	if (parent->parent == NULL)
		i = 0;
	else if (node->key < parent->key)
		i = 0;
	else
		i = 1;
	if (parent->children[i] == NULL)
	{
		node->parent = parent;
		parent->children[i] = node;
	}
	else
	{
		__kbintree_insert(parent->children[i], node);
	}

}

static inline void kbintree_insert(struct kbintree_t *head, struct kbintree_t *node)
{
	unsigned int i = 0;
	for (i = 0; i < 2; i++)
		node->children[i] = NULL;
	__kbintree_insert(head, node);
}

static inline struct kbintree_t *__kbintree_find_head(struct kbintree_t *node)
{
	while (node->parent)
		node = node->parent;
	return node;
}

static inline void kbintree_remove(struct kbintree_t *node)
{
	struct kbintree_t *head = __kbintree_find_head(node);
	if (node->parent->children[0] == node)
		node->parent->children[0] = NULL;
	else if (node->parent->children[1] == node)
		node->parent->children[1] = NULL;
	else
	{
		assert(0);
	}
	node->parent = NULL;

	if (node->children[0])
	{
		__kbintree_insert(head, node->children[0]);
		node->children[0] = NULL;
	}
	if (node->children[1])
	{
		__kbintree_insert(head, node->children[1]);
		node->children[1] = NULL;
	}
}

static inline unsigned int kbintree_size(struct kbintree_t *head)
{
	if (!head)
		return 0;
	else
	{
		unsigned int ret = 0;
		if (head->children[0])
			ret += 1 + kbintree_size(head->children[0]);
		if (head->children[1])
			ret += 1 + kbintree_size(head->children[1]);
		return ret;
	}
}
static inline struct kbintree_t *kbintree_find_first(struct kbintree_t *node)
{
	while (true)
	{
		if (node->children[0])
			node = node->children[0];
		else if (node->children[1])
			node = node->children[1];
		else
			return node;
	}
}

static inline struct kbintree_t * __kbintree_next_right(struct kbintree_t *node)
{
	struct kbintree_t *parent;
	if (node == NULL)
		return NULL;
	parent = node->parent;
	if (parent == NULL)
		return NULL;
	if (parent->children[1] == node || parent->children[1] == NULL)
		return parent;
	else
		return kbintree_find_first(parent->children[1]);
}

static inline struct kbintree_t *kbintree_find_next(struct kbintree_t *node)
{
	struct kbintree_t *ret =  __kbintree_next_right(node);
	if (!ret)
		return NULL;
	if (!ret->parent)
		return NULL;
	return ret;
}

static inline struct kbintree_t *kbintree_find(struct kbintree_t *head, kbintree_key_t key)
{
	struct kbintree_t *node = head->children[0];
	while (node)
	{
		if (node->key == key)
			return node;
		if (key < node->key)
			node = node->children[0];
		else
			node = node->children[1];
	}
	return NULL;
}


#ifdef __KBINTREE_DEBUG_PRINT
#include <stdio.h>
static inline void kbintree_debug_print_prefix(int depth)
{
	int i = 0;
	for (i = 0; i < depth; i++)
		printf("*");
}
static inline void kbintree_debug_print(struct kbintree_t *head, int depth)
{
	int i = 0;
	if (!head)
		return;
	kbintree_debug_print_prefix(depth);
	printf("ptr = %p, parent = %p, key = " KBINTREE_DEBUG_KEY_FORMAT_STRING "\n", head, head->parent, head->key);
	for (i = 0; i < 2; i++)
	{
		kbintree_debug_print(head->children[i], depth+1);
	}

}
#endif /* __KBINTREE_DEBUG_PRINT */
#endif /*  __KBINTREE_H */
