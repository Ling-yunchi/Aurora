#pragma once
#include <vector>

#include "Data.h"

namespace bptree {
	constexpr int max = 3;

	class BPTree;
	class BPTreeNode {
		friend BPTree;
		std::vector<int> keys_;
		std::vector<BPTreeNode*> children_;
		std::vector<Data*> data_;
		int size_ = 0;
		int leaf_;
	public:
		BPTreeNode(bool leaf);
	};

	class BPTree {
		BPTreeNode* root_ = nullptr;
		void insert_internal(int key, BPTreeNode* cursor, BPTreeNode* child);
		BPTreeNode* find_parent(BPTreeNode* cursor, BPTreeNode* child);
	public:
		BPTree() = default;
		Data* search(int key);
		void insert(int key, Data* data);
		void display(BPTreeNode* cursor);
		BPTreeNode* get_root();

	};
}
