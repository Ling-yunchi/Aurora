#pragma once

class BTreeNode
{
	int* keys;
	int t;
	BTreeNode** child;
	int num;
	bool leaf;
public:
	BTreeNode(int t, bool leaf);

	BTreeNode* search(int key);

	int find_key(int key);

	void insert_non_full(int key);

	void split_child(int idx, BTreeNode* y);

	void remove(int key);

	void remove_from_leaf(int idx);

	void remove_from_non_leaf(int idx);

	int get_pred(int idx);

	int get_succ(int idx);

	void fill(int idx);

	void borrow_from_prev(int idx);
	
	void borrow_from_next(int idx);

	void merge(int idx);
	
	friend class BTree;
};

class BTree
{
	BTreeNode* root;
	int t;
public:
	BTree(int t) :root(nullptr), t(t){}
	
	BTreeNode* search(int k);

	void insert(int key);

	void remove(int key);
};
