#include "BPTree.h"
#include <array>
#include "../log/Logger.h"

bptree::BPTreeNode::BPTreeNode(int id, bool leaf) :id_(id), leaf_(leaf)
{
	if (!leaf_) {
		keys_.resize(max);
		children_.resize(max + 1);
	}
	else {
		keys_.resize(max);
		data_.resize(max);
		children_.resize(2);	//此处保存prev与next
	}
}

//实现b+tree插入内部操作
//在parent中找到位置并插入child
void bptree::BPTree::insert_internal(int key, BPTreeNode* cursor, BPTreeNode* child) {
	if (cursor->size_ < max) {
		int idx = 0;
		while (idx < cursor->size_ && key > cursor->keys_[idx]) idx++;

		//遍历游标节点并后移它的key,children
		for (int i = cursor->size_; i > idx; i--) {
			cursor->keys_[i] = cursor->keys_[i - 1];
			cursor->children_[i + 1] = cursor->children_[i];
		}

		cursor->keys_[idx] = key;
		cursor->children_[idx + 1] = child;
		cursor->size_++;
	}
	else {
		//分裂节点
		int virtual_key[max + 1];
		BPTreeNode* virtual_children[max + 2];

		for (int i = 0; i < max; i++) {
			virtual_key[i] = cursor->keys_[i];
			virtual_children[i] = cursor->children_[i];
		}
		virtual_children[max] = cursor->children_[max];

		int idx = 0;
		while (idx < max && key > virtual_key[idx]) idx++;

		for (int i = max; i > idx; i--) {
			virtual_key[i] = virtual_key[i - 1];
			virtual_children[i + 1] = virtual_children[i];
		}
		virtual_key[idx] = key;
		virtual_children[idx + 1] = child;

		auto new_internal = new BPTreeNode(increment_id_++, false);

		cursor->size_ = (max + 1) >> 1;
		new_internal->size_ = max - cursor->size_;

		for (int i = 0, j = cursor->size_ + 1; i < new_internal->size_; i++, j++) {
			new_internal->keys_[i] = virtual_key[j];
			new_internal->children_[i] = virtual_children[j];
		}
		new_internal->children_[new_internal->size_] = virtual_children[max + 1];

		//若cursor为根节点
		if (cursor == root_) {
			auto new_root = new BPTreeNode(increment_id_++, false);

			//更新剩下的节点
			new_root->keys_[0] = cursor->keys_[cursor->size_];	//qwq
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

void bptree::BPTree::remove_internal(int key, BPTreeNode* cursor, BPTreeNode* child) {
	if(cursor == root_) {
		if(cursor->size_ == 1) {
			
		}
	}
}

//找到非叶子节点child的parent
bptree::BPTreeNode* bptree::BPTree::find_parent(BPTreeNode* cursor, BPTreeNode* child) {
	if (cursor->leaf_ || cursor->children_[0]->leaf_) return nullptr;

	BPTreeNode* parent = nullptr;
	for (int i = 0; i < cursor->size_ + 1; i++) {
		if (cursor->children_[i] == child)
			return cursor;
		else {
			parent = find_parent(cursor->children_[i], child);
			if (parent) return parent;
		}
	}
	return parent;
}


Data* bptree::BPTree::search(int key) {
	if (!root_) {
		logger.error("tree is empty!");
		return nullptr;
	}
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
				if (i == cursor->size_ - 1) {
					cursor = cursor->children_[i + 1];
					break;
				}
			}
		}

		//遍历叶子节点
		for (int i = 0; i < cursor->size_; i++) {
			//升序排列,若节点key比要找key大时直接跳出循环
			if (key < cursor->keys_[i]) break;
			if (cursor->keys_[i] == key) {
				logger.info("find value " + std::to_string(cursor->data_[i]->data_));
				return cursor->data_[i];
			}
		}
		logger.info("not find key: " + std::to_string(key));
		return nullptr;
	}
}

void bptree::BPTree::insert(int key, Data* data) {
	if (!root_) {
		root_ = new BPTreeNode(increment_id_++, true);
		root_->keys_[0] = key;
		root_->data_[0] = data;
		root_->children_[0] = root_->children_[1] = nullptr;
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
				if (i == cursor->size_ - 1) {
					cursor = cursor->children_[i + 1];
					break;
				}
			}
		}

		if (cursor->size_ < max) {
			int idx = 0;
			while (key > cursor->keys_[idx] && idx < cursor->size_) idx++;
			if (idx != cursor->size_ && cursor->keys_[idx] == key) {
				//TODO 重构重复代码
				//已存在相同关键字
				logger.error("key " + std::to_string(key) + " is already exist!");
			}
			else {
				for (int i = cursor->size_; i > idx; i--) {
					cursor->keys_[i] = cursor->keys_[i - 1];
					cursor->data_[i] = cursor->data_[i - 1];
				}

				cursor->keys_[idx] = key;
				cursor->data_[idx] = data;
				cursor->size_++;
			}
		}
		else {
			int idx = 0;
			//找到插入位置
			while (idx < max && key > cursor->keys_[idx]) idx++;
			if (idx != cursor->size_ && cursor->keys_[idx] == key) {
				//已存在相同关键字
				logger.error("key " + std::to_string(key) + " is already exist!");
			}
			else {
				//创建一个虚拟节点插入新节点
				int virtual_key[max + 1];
				Data* virtual_data[max + 1];

				for (int i = 0; i < max; i++) {
					virtual_key[i] = cursor->keys_[i];
					virtual_data[i] = cursor->data_[i];
				}

				//后移记录
				for (int i = max; i > idx; i--) {
					virtual_key[i] = virtual_key[i - 1];
					virtual_data[i] = virtual_data[i - 1];
				}
				//插入新记录
				virtual_key[idx] = key;
				virtual_data[idx] = data;

				//创建右兄弟叶子节点
				auto* new_leaf = new BPTreeNode(increment_id_++, true);

				cursor->size_ = (max + 1) >> 1;
				new_leaf->size_ = max + 1 - cursor->size_;

				new_leaf->children_[1] = cursor->children_[1];
				cursor->children_[1] = new_leaf;	//后继
				new_leaf->children_[0] = cursor;	//前驱

				for (int i = 0; i < cursor->size_; i++) {
					cursor->keys_[i] = virtual_key[i];
					cursor->data_[i] = virtual_data[i];
				}
				for (int i = 0, j = cursor->size_; i < new_leaf->size_; i++, j++) {
					new_leaf->keys_[i] = virtual_key[j];
					new_leaf->data_[i] = virtual_data[j];
				}

				//若cursor为根节点
				if (cursor == root_) {
					//建立新的根节点
					auto new_root = new BPTreeNode(increment_id_++, false);

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
}

void bptree::BPTree::remove(int key) {
	if(!root_) {
		logger.error("can not remove from empty tree!");
	}else {
		
	}
}

void bptree::BPTree::display(BPTreeNode* cursor) {
	if (cursor) {
		logger << ">> node: " << std::to_string(cursor->id_) << ", leaf:" << (cursor->leaf_ ? "true" : "false") << "\n";
		logger << "keys: ";
		for (int i = 0; i < cursor->size_; i++) {
			logger << std::to_string(cursor->keys_[i]) << " ";
		}
		logger << "\n";
		if (!cursor->leaf_) {
			logger << "children: ";
			for (int i = 0; i <= cursor->size_; i++) {
				logger << std::to_string(cursor->children_[i]->id_) << " ";
			}
		}
		else {
			logger << "values: ";
			for (int i = 0; i < cursor->size_; i++) {
				logger << std::to_string(cursor->data_[i]->data_) << " ";
			}
			logger << "prev: " << (cursor->children_[0] ? std::to_string(cursor->children_[0]->id_) : "null") << " ,next: " << (cursor->children_[1] ? std::to_string(cursor->children_[1]->id_) : "null");
		}
		logger << "\n";
		if (!cursor->leaf_) {
			for (int i = 0; i < cursor->size_ + 1; i++) {
				display(cursor->children_[i]);
			}
		}
	}
}

bptree::BPTreeNode* bptree::BPTree::get_root() {
	return root_;
}
