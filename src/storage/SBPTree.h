#pragma once
#include "serializable.h"
#include "Cache.hpp"

namespace bptree {
	constexpr int max_node_num = 10;
	constexpr int max_free_node = 100;

	class SBPTree;
	class SBPTreeNode : public serializable<4 + 4 + 1 + (max_node_num * 3 + 1) * 4> {
		friend SBPTree;
		int id_;
		int size_ = 0;
		bool leaf_;
		std::vector<int> keys_;
		std::vector<int> children_;
		std::vector<int> data_;
	public:
		SBPTreeNode(int id, bool leaf);
		char* serialize() override;
		void unserialize(char* buf) override;
	};

	class SBPTree :public serializable<4 + 4 + 4 + max_free_node * 4> {
		int root_ = 0;
		int increment_id_ = 1;
		int free_node_num_ = 0;
		std::vector<int> free_node_;
		Cache<SBPTreeNode, 9 + (max_node_num * 3 + 1) * 4> node_cache_;

		int get_available_id();
		void add_available_id(int id);
		SBPTreeNode& node(int node_id);
		void insert_internal(int key, int cursor, int child);
		void remove_internal(int key, int cursor, int child);
		int find_parent(int cursor, int child);
	public:
		explicit SBPTree(std::string filename);
		char* serialize() override;
		void unserialize(char* buf) override;
		int search(int key);
		void insert(int key, int data);
		void remove(int key);
		void display(int cursor);
		int get_root();
	};
}


