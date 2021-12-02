#pragma once
#include <vector>

#include "Data.h"

namespace bptree {
	constexpr int max_children = 3;
	constexpr int max_leaf = 3;

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
		void insert_internal(int, BPTreeNode*, BPTreeNode*);
		BPTreeNode* find_parent(BPTreeNode*, BPTreeNode*);
	public:
		BPTree() = default;
		Data* search(int key);
		void insert(int key, Data* data);
		void display(BPTreeNode* node);

	};
}
