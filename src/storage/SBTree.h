#pragma once
#include <vector>

#include "Cache.hpp"
#include "serializable.h"

namespace btree
{
	constexpr int order = 3;

	class SBTreeNode : public serializable<4 + 4 + 1 + (order * 2 - 1) * 4 * 2 + (order * 2) * 4 * 2> {
		int id_;
		int size_ = 0;
		bool leaf_;
		std::vector<int> keys_;
		std::vector<int> children_;
		std::vector<int> data_;
	public:
		SBTreeNode(int id, bool leaf);

		int get_id();

		int search(int key);

		int find_key(int key);

		void insert_non_full(int key, int data);

		void remove(int key);

		void remove_from_leaf(int idx);

		void remove_from_non_leaf(int idx);

		void fill(int idx);

		void borrow_from_prev(int idx);

		void borrow_from_next(int idx);

		void merge(int idx);

		void split_child(int idx, SBTreeNode* child);

		void ldr();

		void show_node();

		void clear();
	};

	template<typename T, int TSize>
	class SBTree : public serializable<4>
	{
		int root_ = 0;
		Cache<SBTreeNode, 4 + 4 + 1 + (order * 2 - 1) * 4 * 2 + (order * 2) * 4 * 2> node_cache_;
		Cache<T, TSize> data_cache_;
	public:
		SBTree() = default;

		~SBTree();

		int search(int key);

		void insert(int key, int data);

		void ldr();

		void remove(int key);
	};
};




