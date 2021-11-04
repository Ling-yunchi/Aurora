#pragma once
#include <vector>

class BTreeNode
{
	int* keys;
	int t;
	BTreeNode** child;
	int n;
	bool leaf;
public:
	BTreeNode(int t, bool leaf);

	BTreeNode* search(int k);

	void insert_non_full(int k);

	void split_child(int i, BTreeNode* y);

	friend class BTree;
};

class BTree
{
	BTreeNode* root;
	int t;
public:
	BTree(int t) :root(nullptr), t(t){}
	BTreeNode* search(int k);
	void insert(int k);
};
