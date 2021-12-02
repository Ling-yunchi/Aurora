#pragma once
#include <vector>

#include "Data.h"
using namespace std;

namespace btree
{
	constexpr int order = 3;

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

		int find_key(int key);

		void insert_non_full(int key, Data* data);

		void remove(int key);

		void remove_from_leaf(int idx);

		void remove_from_non_leaf(int idx);

		void fill(int idx);

		void borrow_from_prev(int idx);

		void borrow_from_next(int idx);

		void merge(int idx);

		void split_child(int idx, BTreeNode* child);

		void ldr();

		void show_node();

		void clear();

		friend BTree;
	};

	class BTree
	{
		BTreeNode* root_;
	public:
		BTree() :root_(nullptr) {}

		~BTree();

		Data* search(int key);

		void insert(int key, Data* data);

		void ldr();

		void remove(int key);

	};
}

