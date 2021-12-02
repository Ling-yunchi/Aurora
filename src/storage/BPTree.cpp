#include "BPTree.h"
#include "../log/Logger.h"

bptree::BPTreeNode::BPTreeNode(bool leaf) : leaf_(leaf)
{
	if (!leaf_) {
		keys_.resize(max_children);
		children_.resize(max_children + 1);
	}
	else {
		keys_.resize(max_leaf);
		data_.resize(max_leaf);
	}
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

		if (cursor->size_ < max_leaf) {
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
		}else {
			BPTreeNode* new_leaf = new BPTreeNode(true);

			int virtualNode[max_leaf];
			//TODO
		}
	}
}
