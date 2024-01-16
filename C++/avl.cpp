#include <iostream>
#include "avl.h"

struct BstNode
{
	const void *item;
	BstNode *left;
	BstNode *right;
	int height;
};

struct Bst
{
	BstNode *root;
	BstCompare *compare;
};

BstNode *SingleRotateWithLeft(BstNode *ptr);
BstNode *SingleRotateWithRight(BstNode *ptr);
BstNode *DoubleRotateWithLeft(BstNode *ptr);
BstNode *DoubleRotateWithRight(BstNode *ptr);

inline int Max(int i, int j)
{
	return i > j ? i : j;
}

inline int Height(BstNode *p)
{
	return p == NULL ? -1 : p->height;
}

Bst *BstConstruct(BstCompare *compare)
{
	Bst *tree;

	tree = new Bst;
	if (tree == NULL)
	{
		return NULL;
	}
	tree->root = NULL;
	tree->compare = compare;
	return tree;
}

BstNode *Insert(BstNode *subtree, const void *item, BstCompare *compare)
{
	int result;
	BstNode *p;

	if (subtree == NULL)
	{
		// Create and return a one-node tree
		subtree = new BstNode;
		if (subtree == NULL)
		{
			return NULL; // No memory
		}
		subtree->item = item;
		subtree->left = subtree->right = NULL;
		subtree->height = 0;
	}
	else
	{
		result = compare(item, subtree->item);
		if (result < 0)
		{
			if ((p = Insert(subtree->left, item, compare)) == NULL)
			{
				return NULL;
			}
			subtree->left = p;
			if (Height(subtree->left) - Height(subtree->right) == 2)
			{
				if (compare(item, subtree->left->item) < 0)
				{
					subtree = SingleRotateWithLeft(subtree);
				}
				else
				{
					subtree = DoubleRotateWithLeft(subtree);
				}
			}
		}
		else if (result > 0)
		{
			if ((p = Insert(subtree->right, item, compare)) == NULL)
			{
				return NULL;
			}
			subtree->right = p;
			if (Height(subtree->right) - Height(subtree->left) == 2)
			{
				if (compare(item, subtree->right->item) > 0)
				{
					subtree = SingleRotateWithRight(subtree);
				}
				else
				{
					subtree = DoubleRotateWithRight(subtree);
				}
			}
		}
		else
		{
			return NULL; // Found
		}
	}
	subtree->height = Max(Height(subtree->left), Height(subtree->right)) + 1;
	return subtree;
}

bool BstInsert(Bst *tree, const void *item)
{
	BstNode *p;
	if ((p = Insert(tree->root, item, tree->compare)) == NULL)
	{
		return false;
	}
	tree->root = p;
	return true;
}

const void *BstSearch(const Bst *tree, const void *item)
{
	BstNode *current = tree->root;
	int result;

	while (current != NULL)
	{
		result = tree->compare(item, current->item);
		if (result < 0)
		{
			current = current->left;
		}
		else if (result > 0)
		{
			current = current->right;
		}
		else
		{
			return current->item; // Found
		}
	}
	return NULL; // Not found
}

void Traverse(BstNode *ptr, BstPrint *fn, TraverseOrder order)
{
	if (ptr != NULL)
	{
		switch (order)
		{
		case pre:
			fn(ptr->item);
			Traverse(ptr->left, fn, pre);
			Traverse(ptr->right, fn, pre);
			break;
		case in:
			Traverse(ptr->left, fn, in);
			fn(ptr->item);
			Traverse(ptr->right, fn, in);
			break;
		case post:
			Traverse(ptr->left, fn, post);
			Traverse(ptr->right, fn, post);
			fn(ptr->item);
			break;
		default:
			break;
		}
	}
}

void BstTraverse(Bst *tree, BstPrint *fn, TraverseOrder order)
{
	Traverse(tree->root, fn, order);
}

void DeleteSubtree(BstNode *ptr)
{
	// Recursively delete nodes
	if (ptr == NULL)
	{
		return;
	}
	DeleteSubtree(ptr->left);
	DeleteSubtree(ptr->right);
	free(ptr);
}

void BstDestroy(Bst *tree)
{
	DeleteSubtree(tree->root);
	free(tree);
}

BstNode *SingleRotateWithLeft(BstNode *ptr)
{
	BstNode *newparent;

	newparent = ptr->left;
	ptr->left = newparent->right;
	newparent->right = ptr;
	ptr->height = Max(Height(ptr->left), Height(ptr->right)) + 1;
	newparent->height = Max(Height(newparent->left), ptr->height) + 1;
	return newparent;
}

BstNode *SingleRotateWithRight(BstNode *ptr)
{
	BstNode *newparent;

	newparent = ptr->right;
	ptr->right = newparent->left;
	newparent->left = ptr;
	ptr->height = Max(Height(ptr->right), Height(ptr->left)) + 1;
	newparent->height = Max(Height(newparent->right), ptr->height) + 1;
	return newparent;
}

BstNode *DoubleRotateWithLeft(BstNode *ptr)
{
	ptr->left = SingleRotateWithRight(ptr->left);
	return SingleRotateWithLeft(ptr);
}

BstNode *DoubleRotateWithRight(BstNode *ptr)
{
	ptr->right = SingleRotateWithLeft(ptr->right);
	return SingleRotateWithRight(ptr);
}

int Depth(BstNode *ptr)
{
	if (ptr == NULL)
	{
		return 0;
	}
	else
	{
		return 1 + Max(Depth(ptr->left), Depth(ptr->right));
	}
}
