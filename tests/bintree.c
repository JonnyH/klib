#include "test.h"
#include "klib/kbintree.h"

void random_count(void)
{
	#define NUM_NODES 32
	unsigned int i = 0;
	struct kbintree_t head;
	struct kbintree_t nodes[NUM_NODES] = {0};
	struct kbintree_t *node;
	srand(1);
	kbintree_init_head(&head);
	for (i = 0; i < NUM_NODES; i++)
	{
		nodes[i].key = rand();
		kbintree_insert(&head, &nodes[i]);
	}
	TEST_FAIL_IF(kbintree_size(&head) != NUM_NODES);
}
TEST_FN(random_count);

void random_find(void)
{
	#define NUM_NODES 32
	unsigned int i = 0;
	struct kbintree_t head;
	struct kbintree_t nodes[NUM_NODES];
	struct kbintree_t *node;
	srand(2);
	kbintree_init_head(&head);
	for (i = 0; i < NUM_NODES; i++)
	{
		nodes[i].key = rand();
		kbintree_insert(&head, &nodes[i]);
	}
	for (i = 0; i < NUM_NODES; i++)
	{
		TEST_FAIL_IF(kbintree_find(&head, nodes[i].key) != &nodes[i]);
	}
	for (i = 0; i < NUM_NODES; i++)
	{
		bool unique = true;
		unsigned int j = 0;
		kbintree_key_t key = rand();
		for (j = 0; j < NUM_NODES; j++)
		{
			if (nodes[i].key == key)
			{
				unique = false;
				break;
			}
		}
		if (unique)
			TEST_FAIL_IF(kbintree_find(&head, key) != NULL);
	}
}
TEST_FN(random_find);

void random_delete(void)
{
	#define NUM_NODES 32
	unsigned int i = 0;
	struct kbintree_t head;
	struct kbintree_t nodes[NUM_NODES];
	struct kbintree_t *node;
	srand(3);
	kbintree_init_head(&head);
	for (i = 0; i < NUM_NODES; i++)
	{
		nodes[i].key = rand();
		kbintree_insert(&head, &nodes[i]);
	}
	for (i = 0; i < NUM_NODES; i++)
	{
		TEST_FAIL_IF(kbintree_find(&head, nodes[i].key) != &nodes[i]);
	}
	for (i = 0; i < NUM_NODES; i++)
	{
		unsigned int j = 0;
		kbintree_remove(&nodes[i]);
		for (j = 0; j < NUM_NODES; j++)
		{
			if (j > i)
			{
				TEST_FAIL_IF(kbintree_find(&head, nodes[j].key) != &nodes[j]);
			}
			else
			{
				TEST_FAIL_IF(kbintree_find(&head, nodes[j].key) != NULL);
			}
		}
	}
}
TEST_FN(random_delete);
