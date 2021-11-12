#pragma once
#include <vector>

#include "Data.h"
using namespace std;

const int M = 3;

class BTree;
class BTreeNode
{
	vector<int> keys_;
	vector<BTreeNode*> children_;
	vector<Data*> data_;
	int size_;
	bool leaf_;
public:
	BTreeNode(bool leaf);

	Data* search(int key);

	void insert_non_full(int key, Data* data);

	void split_child(int idx, BTreeNode* child);

	void ldr();
	
	friend BTree;
};

class BTree
{
	BTreeNode* root_;
public:
	BTree() :root_(nullptr) {}

	Data* search(int key);

	void insert(int key, Data* data);

	void ldr();

	void remove(int key);
	
};

