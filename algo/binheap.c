/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Billy Millare
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * 31mar2014
 */

#include <stdio.h>

/**
 * binary heap interface
 */

typedef struct binheap_node {
	struct binheap_node *parent;
	struct binheap_node *leftChild; // right child is leftChild->right
	struct binheap_node *left;
	struct binheap_node *right;
} BINHEAP_NODE;

typedef int (*BINHEAP_COMPARE)(BINHEAP_NODE *node1, BINHEAP_NODE *node2);

typedef struct {
	BINHEAP_NODE *root;
	BINHEAP_NODE *rightmostLeaf;
	BINHEAP_NODE *currentParent;
	BINHEAP_COMPARE compare;
} BINHEAP;

#define BINHEAP_INITIALIZER(compare)	{ NULL, NULL, NULL, (BINHEAP_COMPARE)compare }

static void fixNode(BINHEAP *binheap, BINHEAP_NODE *node, BINHEAP_NODE *origNode)
{
	if(node->parent != NULL && node->parent->leftChild == origNode)
		node->parent->leftChild = node;

	if(node->left != NULL)
		node->left->right = node;

	if(node->right != NULL)
		node->right->left = node;

	if(node->leftChild != NULL)
	{
		node->leftChild->parent = node;

		if(node->leftChild->right != NULL)
			node->leftChild->right->parent = node;
	}
}

static void swapNodes(BINHEAP *binheap, BINHEAP_NODE *node1, BINHEAP_NODE *node2)
{
	BINHEAP_NODE temp = *node1;
	*node1 = *node2;
	*node2 = temp;

	if(node1->parent == node1)
	{
		node1->parent = node2;

		if(node2->leftChild == node2)
			node2->leftChild = node1;
		else
			node2->leftChild->right = node1;
	}
	else if(node1->leftChild == node1)
	{
		node1->leftChild = node2;
		node2->parent = node1;
	}
	else if(node1->leftChild != NULL && node1->leftChild->right == node1)
	{
		node1->leftChild->right = node2;
		node2->parent = node1;
	}

	if(node1->left == node1)
	{
		node1->left = node2;
		node2->right = node1;
	}
	else if(node1->right == node1)
	{
		node1->right = node2;
		node2->left = node1;
	}

	fixNode(binheap, node1, node2);
	fixNode(binheap, node2, node1);

	if(node1 == binheap->root)
		binheap->root = node2;
	else if(node2 == binheap->root)
		binheap->root = node1;

	if(node1 == binheap->rightmostLeaf)
		binheap->rightmostLeaf = node2;
	else if(node2 == binheap->rightmostLeaf)
		binheap->rightmostLeaf = node1;

	if(node1 == binheap->currentParent)
		binheap->currentParent = node2;
	else if(node2 == binheap->currentParent)
		binheap->currentParent = node1;
}

static void bubbleUp(BINHEAP *binheap, BINHEAP_NODE *node)
{
	while(node->parent != NULL && (binheap->compare)(node, node->parent) < 0)
		swapNodes(binheap, node, node->parent);
}

static void bubbleDown(BINHEAP *binheap, BINHEAP_NODE *node)
{
	BINHEAP_NODE *node2;

	while(node->leftChild != NULL)
	{
		node2 = node->leftChild;

		if(node2->right != NULL)
		{
			if((binheap->compare)(node2, node2->right) > 0)
				node2 = node2->right;
		}

		if((binheap->compare)(node, node2) < 1)
			break;

		swapNodes(binheap, node, node2);
	}
}

void binheap_insert(BINHEAP *binheap, BINHEAP_NODE *node)
{
	node->parent = NULL;
	node->leftChild = NULL;
	node->left = NULL;
	node->right = NULL;

	if(binheap->root == NULL)
	{
		binheap->root = node;
		binheap->rightmostLeaf = node;
		binheap->currentParent = node;
	}
	else
	{
		binheap->rightmostLeaf->right = node;
		node->left = binheap->rightmostLeaf;
		node->parent = binheap->currentParent;
		binheap->rightmostLeaf = node;

		if(binheap->currentParent->leftChild == NULL)
			binheap->currentParent->leftChild = node;
		else
			binheap->currentParent = binheap->currentParent->right;

		bubbleUp(binheap, node);
	}
}

BINHEAP_NODE * binheap_delete(BINHEAP *binheap, BINHEAP_NODE *node)
{
	if(binheap->root->right == NULL)
		binheap->root = NULL;
	else
	{
		BINHEAP_NODE *node1 = binheap->rightmostLeaf;

		if(node != binheap->rightmostLeaf)
			swapNodes(binheap, node, binheap->rightmostLeaf);

		if(node->parent->leftChild == node)
			node->parent->leftChild = NULL;
		else
			binheap->currentParent = binheap->currentParent->left;

		node->left->right = NULL;
		binheap->rightmostLeaf = node->left;

		if(binheap->root->right != NULL)
		{
			bubbleUp(binheap, node1);
			bubbleDown(binheap, node1);
		}
	}

	return node;
}

/**
 * test
 */

#include <string.h>
#include <ctype.h>

#define HELP 										\
			"[help]\n"								\
			"  ?         - help\n"					\
			"  i 'key'   - insert key\n"			\
			"  d 'key'   - delete key\n"			\
			"  r         - get root key\n" 			\
			"  f         - flush heap\n"			\
			"  p         - print heap\n"			\
			"  m         - switch to min-heap\n"	\
			"  M         - switch to max-heap\n"	\
			"\n"									\
			"  max-key=%d\n"						\
			"  min-key=0\n"							\
			"\n"

typedef struct {
	BINHEAP_NODE node;
	int key;
	int inHeap;
} DATA;

static int nodeCount = 0;

static void printChar(int ch, int items)
{
	int i;

	for(i = 0; i < items; i++)
		printf("%c", ch);
}

static int maxCompare(DATA *data1, DATA *data2)
{
	if(data1->key > data2->key)
		return -1;

	return (data1->key < data2->key) ? 1 : 0;
}

static int minCompare(DATA *data1, DATA *data2)
{
	if(data1->key < data2->key)
		return -1;

	return (data1->key > data2->key) ? 1 : 0;
}

static void printBinheap(BINHEAP *binheap)
{
	int depth = 0;
	int bits = nodeCount;
	int i;

	if(bits == 0)
		return;

	for(depth = sizeof(bits) * 8; bits > -1; --depth, bits <<= 1);

	if(depth > 0)
	{
		BINHEAP_NODE *node = binheap->root;
		unsigned int pos;
		int cnt;
		int items;


		for(i = 1; i <= depth && node != NULL; i++)
		{
			printf("[%3d]   ", i);

			pos = (0x1 << (depth - i)) * 3;
			printChar(' ', pos - 3);
			pos = pos * 2 - 3;
			cnt = 0;
			items = (0x1 << (i - 1));

			while(1)
			{
				printf("%03d", ((DATA *)node)->key);

				if((node = node->right) == NULL || --items == 0)
					break;

				printChar(((++cnt % 2) == 0) ? ' ' : '-', pos);
			}

			printf("\n");
		}
	}
}

int main(void)
{
	BINHEAP binheap = BINHEAP_INITIALIZER(maxCompare);
	const int items = 100;
	DATA data[items];
	int i;
	DATA *node;
	int isMaxHeap = 1;

	for(i = 0; i < items; i++)
	{
		data[i].key = i;
		data[i].inHeap = 0;
	}

	char cmd[256] = "?";
	char *token;
	int key;

	while(1)
	{
		switch(cmd[0])
		{
		case '?':
			printf(HELP, items - 1);
			break;

		case 'i':
		case 'd':
			token = strtok(&cmd[2], " \t");

			while(token != NULL)
			{
				key = -1;
				sscanf(token, "%d", &key);

				do
				{
					if(key < 0 || key >= items)
					{
						printf("[%d] invalid\n", key);
						break;
					}

					if(cmd[0] == 'i')
					{
						if(data[key].inHeap)
						{
							printf("[%d] duplicate\n", key);
							break;
						}

						binheap_insert(&binheap, (BINHEAP_NODE *)&data[key]);
						data[key].inHeap = 1;
						++nodeCount;
						printf("[%d] inserted\n", key);
					}
					else
					{
						if(!data[key].inHeap)
						{
							printf("[%d] missing\n", key);
							break;
						}

						binheap_delete(&binheap, (BINHEAP_NODE *)&data[key]);
						data[key].inHeap = 0;
						--nodeCount;
						printf("[%d] deleted\n", key);
					}
				}
				while(0);

				token = strtok(NULL, " \t");
			}

			break;

		case 'r':
			if(binheap.root != NULL)
			{
				node = (DATA *)binheap_delete(&binheap, binheap.root);
				--nodeCount;
				printf("[%d] root pulled\n", node->key);
			}

			break;

		case 'f':
			printf("flushing %s-heap...\n", isMaxHeap ? "max" : "min");

			while(binheap.root != NULL)
			{
				node = (DATA *)binheap_delete(&binheap, binheap.root);
				node->inHeap = 0;
				nodeCount = 0;
				printf("%d\n", node->key);
			}

			break;

		case 'p':
			printf("heap: %d nodes, %s-heap\n", nodeCount, isMaxHeap ? "max" : "min");
			printBinheap(&binheap);
			break;

		case 'm':
			if(!isMaxHeap)
				break;

		case 'M':
			if(cmd[0] == 'M' && isMaxHeap)
				break;

			while(binheap.root != NULL)
			{
				node = (DATA *)binheap_delete(&binheap, binheap.root);
				node->inHeap = 0;
			}

			nodeCount = 0;
			isMaxHeap = !isMaxHeap;
			binheap.compare = isMaxHeap ? (BINHEAP_COMPARE)maxCompare : (BINHEAP_COMPARE)minCompare;
			printf("switched to %s-heap\n", isMaxHeap ? "max" : "min");
			break;

		default:
			break;
		}

		do
		{
			printf(">>> ");
			fgets(cmd, sizeof(cmd) - 1, stdin);
		}
		while(!isspace(cmd[1]));
	}

	return 0;
}
