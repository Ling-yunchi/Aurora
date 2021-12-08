#pragma once
#include <vector>

#include "Data.h"

namespace bptree {
	constexpr int max_node_num = 10;

	class BPTree;
	class BPTreeNode {
		friend BPTree;
		int id_;
		std::vector<int> keys_;
		std::vector<BPTreeNode*> children_;
		std::vector<Data*> data_;
		int size_ = 0;
		int leaf_;
	public:
		BPTreeNode(int id, bool leaf);
	};

	class BPTree {
		BPTreeNode* root_ = nullptr;
		int increment_id_ = 1;
		void insert_internal(int key, BPTreeNode* cursor, BPTreeNode* child);
		void remove_internal(int key, BPTreeNode* cursor, BPTreeNode* child);
		BPTreeNode* find_parent(BPTreeNode* cursor, BPTreeNode* child);
	public:
		BPTree() = default;
		Data* search(int key);
		void insert(int key, Data* data);
		void remove(int key);
		void display(BPTreeNode* cursor);
		BPTreeNode* get_root();
	};
}
