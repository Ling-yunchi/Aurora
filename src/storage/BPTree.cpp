#include "BPTree.h"
#include "../log/Logger.h"

bptree::BPTreeNode::BPTreeNode(bool leaf) : leaf_(leaf)
{
	if (!leaf_) {
		keys_.resize(max);
		children_.resize(max + 1);
	}
	else {
		keys_.resize(max);
		data_.resize(max);
	}
}

//实现b+tree插入内部操作
//在cursor中找到位置并插入child
void bptree::BPTree::insert_internal(int key, BPTreeNode* cursor, BPTreeNode* child) {
	if (cursor->size_ < max) {
		int idx = 0;
		while (key > cursor->keys_[idx] && idx < cursor->size_) idx++;

		//遍历游标节点并前移它的key,children
		for (int i = cursor->size_; i > idx; i--) {
			cursor->keys_[i] = cursor->keys_[i - 1];
			cursor->children_[i + 1] = cursor->children_[i];
		}

		cursor->keys_[idx] = key;
		cursor->children_[idx] = child;
		cursor->size_++;
	}
	else {
		//分裂节点
		auto new_internal = new BPTreeNode(false);
		int virtual_key[max + 1];
		BPTreeNode* virtual_children[max + 2];

		for (int i = 0; i < max; i++) {
			virtual_key[i] = cursor->keys_[i];
		}
		for (int i = 0; i < max + 1; i++) {
			virtual_children[i] = cursor->children_[i];
		}

		int idx = 0;
		while (key > virtual_key[idx] && idx < max) idx++;

		for (int i = max + 1; i > idx; i--) {
			virtual_key[i] = virtual_key[i - 1];
		}
		virtual_key[idx] = key;

		for (int i = max + 2; i > idx + 1; i++) {
			virtual_children[i] = virtual_children[i - 1];
		}
		virtual_children[idx + 1] = child;

		cursor->size_ = (max + 1) >> 1;
		new_internal->size_ = max - cursor->size_;

		for (int i = 0, j = cursor->size_ + 1; i < new_internal->size_; i++, j++) {
			new_internal->keys_[i] = virtual_key[j];
		}
		for (int i = 0, j = cursor->size_ + 1; i < new_internal->size_ + 1; i++, j++) {
			new_internal->children_[i] = virtual_children[j];
		}


		//若cursor为根节点
		if (cursor == root_) {
			auto new_root = new BPTreeNode(false);

			//更新剩下的节点
			new_root->keys_[0] = cursor->keys_[0];
			new_root->children_[0] = cursor;
			new_root->children_[1] = new_internal;
			new_root->size_ = 1;
			root_ = new_root;
		}
		else {
			//递归调用insert_internal
			insert_internal(cursor->keys_[cursor->size_], find_parent(root_, cursor), new_internal);
		}
	}
}

//找到child的parent
bptree::BPTreeNode* bptree::BPTree::find_parent(BPTreeNode* cursor, BPTreeNode* child) {
	if (cursor->leaf_ || cursor->children_[0]->leaf_) return nullptr;

	BPTreeNode* parent = nullptr;
	for(int i = 0;i<cursor->size_+1;i++) {
		if(cursor->children_[i] == child) 
			return cursor;
		else {
			parent = find_parent(cursor->children_[i], child);
			if (parent) return parent;
		}
	}
	return parent;
}


Data* bptree::BPTree::search(int key) {
	if (!root_)
		logger.error("tree is empty!");
	else {
		//遍历查找
		BPTreeNode* cursor = root_;

		//直到找到叶子节点
		while (!cursor->leaf_) {
			for (int i = 0; i < cursor->size_; i++) {
				if (key < cursor->keys_[i]) {
					cursor = cursor->children_[i];
					break;
				}
				if (i == cursor->size_ - 1)
					cursor = cursor->children_[i + 1];
			}
		}

		//遍历叶子节点
		for (int i = 0; i < cursor->size_; i++) {
			//升序排列,若节点key比要找key大时直接跳出循环
			if (key < cursor->keys_[i]) break;
			if (cursor->keys_[i] == key) {
				logger.info("find value" + std::to_string(cursor->data_[i]->data_));
				return cursor->data_[i];
			}
		}
		logger.info("not find key: " + std::to_string(key));
		return nullptr;
	}
}

void bptree::BPTree::insert(int key, Data* data) {
	if (!root_) {
		root_ = new BPTreeNode(true);
		root_->keys_[0] = key;
		root_->data_[0] = data;
		root_->size_++;
	}
	else {
		BPTreeNode* cursor = root_;
		BPTreeNode* parent = nullptr;

		//让游标指向叶子节点
		while (!cursor->leaf_) {
			parent = cursor;
			for (int i = 0; i < cursor->size_; i++) {
				if (key < cursor->keys_[i]) {
					cursor = cursor->children_[i];
					break;
				}
				if (i == cursor->size_ - 1)
					cursor = cursor->children_[i + 1];
			}
		}

		if (cursor->size_ < max) {
			int idx = 0;
			while (key > cursor->keys_[idx] && idx < cursor->size_) idx++;

			for (int i = cursor->size_; i > idx; i--) {
				cursor->keys_[i] = cursor->keys_[i - 1];
				cursor->data_[i] = cursor->data_[i - 1];
			}

			cursor->keys_[idx] = key;
			cursor->data_[idx] = data;
			cursor->size_++;

			//前后兄弟节点
			//cursor->children_[cursor->size_] = cursor->children_[cursor->size_ - 1];
			//cursor->children_[cursor->size_ - 1] = nullptr;
		}
		else {
			auto* new_leaf = new BPTreeNode(true);

			int virtual_node[max + 1];
			Data* virtual_data[max + 1];

			for (int i = 0; i < max; i++) {
				virtual_node[i] = cursor->keys_[i];
				virtual_data[i] = cursor->data_[i];
			}

			//遍历查找新节点插入位置
			int idx = 0;
			while (key > virtual_node[idx] && idx < max) {
				idx++;
			}

			//更新虚拟节点
			for (int i = max; i > idx; i--) {
				virtual_node[i] = virtual_node[i - 1];
				virtual_data[i] = virtual_data[i - 1];
			}
			virtual_node[idx] = key;
			virtual_data[idx] = data;

			cursor->size_ = (max + 1) >> 1;
			new_leaf->size_ = max + 1 - cursor->size_;

			cursor->children_[cursor->size_] = new_leaf;
			new_leaf->children_[new_leaf->size_] = cursor->children_[max];

			cursor->children_[max] = nullptr;

			//更新虚拟节点
			for (int i = 0; i < cursor->size_; i++) {
				cursor->keys_[i] = virtual_node[i];
				cursor->data_[i] = virtual_data[i];
			}

			//更新new_leaf的key到虚拟节点
			for (int i = 0, j = cursor->size_; i < new_leaf->size_; i++, j++) {
				new_leaf->keys_[i] = virtual_node[j];
				new_leaf->data_[i] = virtual_data[j];
			}

			//若cursor为根节点
			if (cursor == root_) {
				auto new_root = new BPTreeNode(false);

				//更新剩下的节点
				new_root->keys_[0] = new_leaf->keys_[0];
				new_root->children_[0] = cursor;
				new_root->children_[1] = new_leaf;
				new_root->size_ = 1;
				root_ = new_root;
			}
			else {
				//递归调用insert_internal
				insert_internal(new_leaf->keys_[0], parent, new_leaf);
			}
		}
	}
}

void bptree::BPTree::display(BPTreeNode* cursor) {
	if(cursor) {
		for(int i=0;i<cursor->size_;i++) {
			logger << std::to_string(cursor->keys_[i]) << " ";
		}
		logger << "\n";
		if(!cursor->leaf_) {
			for (int i =0;i<cursor->size_+1;i++) {
				display(cursor->children_[i]);
			}
		}
	}
}

bptree::BPTreeNode* bptree::BPTree::get_root() {
	return root_;
}
