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
		std::vector<BPTreeNode> children_;
		std::vector<Data*> data_;
		int size_;
		int leaf_;
		BPTreeNode(bool leaf);
	};

	class BPTree {
	public:

	};
}
