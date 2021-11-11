#pragma once
#include <vector>

#include "Data.h"
using namespace std;

const int M = 3;

class BTreeNode
{
	int size_;
	vector<int> keys_;
	vector<BTreeNode*> children_;
	vector<Data*> data_;
public:
	BTreeNode();
};

class BTree
{

	
};

