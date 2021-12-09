#include "SBPTree.h"

bptree::SBPTreeNode::SBPTreeNode() :
	keys_(max_node_num),
	children_(max_node_num + 1),
	data_(max_node_num) {}

bptree::SBPTreeNode::SBPTreeNode(int id, bool leaf)
	: id_(id), leaf_(leaf),
	keys_(max_node_num),
	children_(max_node_num + 1),
	data_(max_node_num) {
	//if (!leaf_) {
	//	keys_.resize(max_node_num);
	//	children_.resize(max_node_num + 1);
	//}
	//else {
	//	keys_.resize(max_node_num);
	//	data_.resize(max_node_num);
	//	children_.resize(2);	//此处保存prev与next
	//}
}

char* bptree::SBPTreeNode::serialize() {
	auto buf = new char[this->storage_size_];
	auto tmp_buf = buf;
	serialize_int(id_, tmp_buf);
	serialize_int(size_, tmp_buf);
	*tmp_buf++ = leaf_;
	serialize_vector(keys_, size_, tmp_buf);
	serialize_vector(children_, size_ + 1, tmp_buf);
	serialize_vector(data_, size_, tmp_buf);
	return buf;
}
void bptree::SBPTreeNode::unserialize(char* buf) {
	unserialize_int(id_, buf);
	unserialize_int(size_, buf);
	leaf_ = *buf++;
	unserialize_vector(keys_, size_, buf);
	unserialize_vector(children_, size_ + 1, buf);
	unserialize_vector(data_, size_, buf);
}

int bptree::SBPTree::get_available_id() {
	if (free_node_num_ > 0) {
		int tmp = free_node_.back();
		free_node_.pop_back();
		free_node_num_--;
		return tmp;
	}
	else
		return increment_id_++;
}

void bptree::SBPTree::add_available_id(int id) {
	if (free_node_num_ < max_free_node) {
		free_node_.push_back(id);
		free_node_num_++;
	}
	else {
		logger.warn("free node is full, id " + std::to_string(id) + " is deprecated");
	}
}

bptree::SBPTreeNode& bptree::SBPTree::node(int node_id) {
	return node_cache_.get_item(node_id);
}

void bptree::SBPTree::insert_internal(int key, int cursor, int child) {
	if (node(cursor).size_ < max_node_num) {
		int idx = 0;
		while (idx < node(cursor).size_ && key > node(cursor).keys_[idx]) idx++;

		//遍历游标节点并后移它的key,children
		for (int i = node(cursor).size_; i > idx; i--) {
			node(cursor).keys_[i] = node(cursor).keys_[i - 1];
			node(cursor).children_[i + 1] = node(cursor).children_[i];
		}

		node(cursor).keys_[idx] = key;
		node(cursor).children_[idx + 1] = child;
		node(cursor).size_++;
		node_cache_.set_dirty(cursor);
	}
	else {
		//分裂节点
		int virtual_key[max_node_num + 1];
		int virtual_children[max_node_num + 2];

		for (int i = 0; i < max_node_num; i++) {
			virtual_key[i] = node(cursor).keys_[i];
			virtual_children[i] = node(cursor).children_[i];
		}
		virtual_children[max_node_num] = node(cursor).children_[max_node_num];

		int idx = 0;
		while (idx < max_node_num && key > virtual_key[idx]) idx++;

		for (int i = max_node_num; i > idx; i--) {
			virtual_key[i] = virtual_key[i - 1];
			virtual_children[i + 1] = virtual_children[i];
		}
		virtual_key[idx] = key;
		virtual_children[idx + 1] = child;

		SBPTreeNode new_internal(get_available_id(), false);

		node(cursor).size_ = (max_node_num + 1) >> 1;
		new_internal.size_ = max_node_num - node(cursor).size_;

		for (int i = 0, j = node(cursor).size_ + 1; i < new_internal.size_; i++, j++) {
			new_internal.keys_[i] = virtual_key[j];
			new_internal.children_[i] = virtual_children[j];
		}
		new_internal.children_[new_internal.size_] = virtual_children[max_node_num + 1];

		node_cache_.insert_item(new_internal.id_, new_internal);
		node_cache_.set_dirty(cursor);

		//若cursor为根节点
		if (cursor == root_) {
			SBPTreeNode new_root(get_available_id(), false);

			//更新剩下的节点
			new_root.keys_[0] = node(cursor).keys_[node(cursor).size_];	//qwq
			new_root.children_[0] = cursor;
			new_root.children_[1] = new_internal.id_;
			new_root.size_ = 1;
			root_ = new_root.id_;

			node_cache_.insert_item(root_, new_root);
		}
		else {
			//递归调用insert_internal
			insert_internal(node(cursor).keys_[node(cursor).size_], find_parent(root_, cursor), new_internal.id_);
		}
	}
}

void bptree::SBPTree::remove_internal(int key, int cursor, int child) {
	if (cursor == root_ && node(cursor).size_ == 1) {
		if (node(cursor).children_[1] == child)
			root_ = node(cursor).children_[0];
		else if (node(cursor).children_[0] == child)
			root_ = node(cursor).children_[1];
		//delete child;
		//delete cursor;

		//将删除的节点id加入可用节点id
		add_available_id(child);
		add_available_id(cursor);
		return;
	}

	int idx = 0;
	while (idx < node(cursor).size_ && node(cursor).keys_[idx] != key) idx++;

	for (int i = idx; i < node(cursor).size_; i++) {
		node(cursor).keys_[i] = node(cursor).keys_[i + 1];
		node(cursor).children_[i + 1] = node(cursor).children_[i + 2];
	}
	node(cursor).size_--;

	node_cache_.set_dirty(cursor);
	//delete child;
	add_available_id(child);

	if (node(cursor).size_ >= ((max_node_num + 1) >> 1) - 1) return;
	if (cursor == root_) return;

	int parent = find_parent(root_, cursor);
	int l_s = -1, r_s = -1;
	for (idx = 0; idx < node(parent).size_ + 1; idx++) {
		if (node(parent).children_[idx] == cursor) {
			l_s = idx - 1;
			r_s = idx + 1;
			break;
		}
	}

	//向左右兄弟借节点
	if (l_s >= 0) {
		//有左兄弟
		auto& left_sibling = node(node(parent).children_[l_s]);
		if (left_sibling.size_ >= (max_node_num + 1) >> 1) {
			for (int i = node(cursor).size_; i > 0; i--) {
				node(cursor).keys_[i] = node(cursor).keys_[i - 1];
				node(cursor).children_[i + 1] = node(cursor).children_[i];
			}
			node(cursor).children_[1] = node(cursor).children_[0];

			node(cursor).keys_[0] = node(parent).keys_[l_s];
			node(cursor).children_[0] = left_sibling.children_[left_sibling.size_];
			node(cursor).size_++;

			node(parent).keys_[l_s] = left_sibling.keys_[left_sibling.size_ - 1];
			left_sibling.size_--;

			node_cache_.set_dirty(left_sibling.id_);
			node_cache_.set_dirty(cursor);
			return;
		}
	}
	if (r_s <= node(parent).size_) {
		//有右兄弟
		auto& right_sibling = node(node(parent).children_[r_s]);
		if (right_sibling.size_ >= (max_node_num + 1) >> 1) {
			node(cursor).keys_[node(cursor).size_] = node(parent).keys_[idx];
			node(parent).keys_[idx] = right_sibling.keys_[0];
			node(cursor).children_[node(cursor).size_ + 1] = right_sibling.children_[0];
			node(cursor).size_++;

			for (int i = 1; i < right_sibling.size_; i++) {
				right_sibling.keys_[i - 1] = right_sibling.keys_[i];
				right_sibling.children_[i - 1] = right_sibling.children_[i];
			}
			right_sibling.children_[right_sibling.size_ - 1] = right_sibling.children_[right_sibling.size_];
			right_sibling.size_--;

			node_cache_.set_dirty(right_sibling.id_);
			node_cache_.set_dirty(cursor);
			return;
		}
	}

	//与左右兄弟合并
	if (l_s >= 0) {
		//将自身合并到左兄弟
		auto& left_sibling = node(node(parent).children_[l_s]);
		left_sibling.keys_[left_sibling.size_] = node(parent).keys_[l_s];

		for (int i = left_sibling.size_ + 1, j = 0; j < node(cursor).size_; i++, j++) {
			left_sibling.keys_[i] = node(cursor).keys_[j];
			left_sibling.children_[i] = node(cursor).children_[j];
		}
		left_sibling.children_[left_sibling.size_ + node(cursor).size_ + 1] =
			node(cursor).children_[node(cursor).size_];

		left_sibling.size_ += node(cursor).size_ + 1;
		node(cursor).size_ = 0;

		node_cache_.set_dirty(left_sibling.id_);
		node_cache_.set_dirty(cursor);

		remove_internal(node(parent).keys_[l_s], parent, cursor);
	}
	else if (node(cursor).children_[1]) {
		//将右兄弟合并到自身
		auto& right_sibling = node(node(parent).children_[r_s]);
		node(cursor).keys_[node(cursor).size_] = node(parent).keys_[r_s - 1];

		for (int i = node(cursor).size_ + 1, j = 0; j < right_sibling.size_; i++, j++) {
			node(cursor).keys_[i] = right_sibling.keys_[j];
			node(cursor).children_[i] = right_sibling.children_[j];
		}
		node(cursor).children_[node(cursor).size_ + right_sibling.size_ + 1] =
			right_sibling.children_[right_sibling.size_];

		node(cursor).size_ += right_sibling.size_ + 1;
		right_sibling.size_ = 0;

		node_cache_.set_dirty(right_sibling.id_);
		node_cache_.set_dirty(cursor);

		remove_internal(node(parent).keys_[r_s - 1], parent, right_sibling.id_);
	}
}

int bptree::SBPTree::find_parent(int cursor, int child) {
	if (node(cursor).leaf_ || node(node(cursor).children_[0]).leaf_) return 0;

	int parent = 0;
	for (int i = 0; i < node(cursor).size_ + 1; i++) {
		if (node(cursor).children_[i] == child)
			return cursor;
		else {
			parent = find_parent(node(cursor).children_[i], child);
			if (parent) return parent;
		}
	}
	return parent;
}

bptree::SBPTree::SBPTree(std::string filename) :node_cache_(1000, filename) {}

char* bptree::SBPTree::serialize() {
	auto buf = new char[this->storage_size_];
	auto tmp_buf = buf;
	serialize_int(root_, tmp_buf);
	serialize_int(increment_id_, tmp_buf);
	serialize_int(free_node_num_, tmp_buf);
	serialize_vector(free_node_, free_node_num_, tmp_buf);
	return buf;
}
void bptree::SBPTree::unserialize(char* buf) {
	unserialize_int(root_, buf);
	unserialize_int(increment_id_, buf);
	unserialize_int(free_node_num_, buf);
	unserialize_vector(free_node_, free_node_num_, buf);
}

int bptree::SBPTree::search(int key) {
	if (!root_) {
		logger.error("tree is empty!");
		return 0;
	}
	else {
		//遍历查找
		int cursor = root_;

		//直到找到叶子节点
		while (!node(cursor).leaf_) {
			for (int i = 0; i < node(cursor).size_; i++) {
				if (key < node(cursor).keys_[i]) {
					cursor = node(cursor).children_[i];
					break;
				}
				if (i == node(cursor).size_ - 1) {
					cursor = node(cursor).children_[i + 1];
					break;
				}
			}
		}

		//遍历叶子节点
		for (int i = 0; i < node(cursor).size_; i++) {
			//升序排列,若节点key比要找key大时直接跳出循环
			if (key < node(cursor).keys_[i]) break;
			if (node(cursor).keys_[i] == key) {
				logger.info("find data " + std::to_string(node(cursor).data_[i]));
				return node(cursor).data_[i];
			}
		}
		logger.info("not find key: " + std::to_string(key));
		return 0;
	}
}

void bptree::SBPTree::insert(int key, int data) {
	if (!root_) {
		root_ = increment_id_++;
		SBPTreeNode node(root_, true);
		node.keys_[0] = key;
		node.data_[0] = data;
		logger.info("insert key " + std::to_string(key));
		node.children_[0] = node.children_[1] = 0;
		node.size_++;
		node_cache_.insert_item(root_, node);
	}
	else {
		int cursor = root_;
		int parent = 0;

		//让游标指向叶子节点
		while (!node(cursor).leaf_) {
			parent = cursor;
			for (int i = 0; i < node(cursor).size_; i++) {
				if (key < node(cursor).keys_[i]) {
					cursor = node(cursor).children_[i];
					break;
				}
				if (i == node(cursor).size_ - 1) {
					cursor = node(cursor).children_[i + 1];
					break;
				}
			}
		}

		if (node(cursor).size_ < max_node_num) {
			int idx = 0;
			while (key > node(cursor).keys_[idx] && idx < node(cursor).size_) idx++;
			if (idx != node(cursor).size_ && node(cursor).keys_[idx] == key) {
				//已存在相同关键字
				logger.error("key " + std::to_string(key) + " is already exist!");
			}
			else {
				for (int i = node(cursor).size_; i > idx; i--) {
					node(cursor).keys_[i] = node(cursor).keys_[i - 1];
					node(cursor).data_[i] = node(cursor).data_[i - 1];
				}

				node(cursor).keys_[idx] = key;
				node(cursor).data_[idx] = data;
				logger.info("insert key " + std::to_string(key));
				node(cursor).size_++;
				node_cache_.set_dirty(cursor);
			}
		}
		else {
			int idx = 0;
			//找到插入位置
			while (idx < max_node_num && key > node(cursor).keys_[idx]) idx++;
			if (idx != node(cursor).size_ && node(cursor).keys_[idx] == key) {
				//已存在相同关键字
				logger.error("key " + std::to_string(key) + " is already exist!");
			}
			else {
				//创建一个虚拟节点插入新节点
				int virtual_key[max_node_num + 1];
				int virtual_data[max_node_num + 1];

				for (int i = 0; i < max_node_num; i++) {
					virtual_key[i] = node(cursor).keys_[i];
					virtual_data[i] = node(cursor).data_[i];
				}

				//后移记录
				for (int i = max_node_num; i > idx; i--) {
					virtual_key[i] = virtual_key[i - 1];
					virtual_data[i] = virtual_data[i - 1];
				}
				//插入新记录
				virtual_key[idx] = key;
				virtual_data[idx] = data;
				logger.info("insert key " + std::to_string(key));

				//创建右兄弟叶子节点
				SBPTreeNode new_leaf(get_available_id(), true);

				node(cursor).size_ = (max_node_num + 1) >> 1;
				new_leaf.size_ = max_node_num + 1 - node(cursor).size_;

				new_leaf.children_[1] = node(cursor).children_[1];
				node(cursor).children_[1] = new_leaf.id_;	//后继
				new_leaf.children_[0] = cursor;	//前驱

				for (int i = 0; i < node(cursor).size_; i++) {
					node(cursor).keys_[i] = virtual_key[i];
					node(cursor).data_[i] = virtual_data[i];
				}
				for (int i = 0, j = node(cursor).size_; i < new_leaf.size_; i++, j++) {
					new_leaf.keys_[i] = virtual_key[j];
					new_leaf.data_[i] = virtual_data[j];
				}

				node_cache_.insert_item(new_leaf.id_, new_leaf);
				node_cache_.set_dirty(cursor);

				//若cursor为根节点
				if (cursor == root_) {
					//建立新的根节点
					SBPTreeNode new_root(get_available_id(), false);

					//更新剩下的节点
					new_root.keys_[0] = new_leaf.keys_[0];
					new_root.children_[0] = cursor;
					new_root.children_[1] = new_leaf.id_;
					new_root.size_ = 1;
					root_ = new_root.id_;
					node_cache_.insert_item(new_root.id_, new_root);
				}
				else {
					//递归调用insert_internal
					insert_internal(new_leaf.keys_[0], parent, new_leaf.id_);
				}
			}
		}
	}
}

void bptree::SBPTree::remove(int key) {
	if (!root_) {
		logger.error("can not remove from empty tree!");
	}
	else {
		int cursor = root_;
		int parent = 0;
		int l_s = -1, r_s = -1;
		while (!node(cursor).leaf_) {
			for (int i = 0; i < node(cursor).size_; i++) {
				parent = cursor;
				l_s = i - 1;
				r_s = i + 1;
				if (key < node(cursor).keys_[i]) {
					cursor = node(cursor).children_[i];
					break;
				}
				if (i == node(cursor).size_ - 1) {
					l_s = i;
					r_s = i + 2;
					cursor = node(cursor).children_[i + 1];
					break;
				}
			}
		}

		bool found = false;
		int idx;
		for (idx = 0; idx < node(cursor).size_; idx++) {
			if (node(cursor).keys_[idx] == key) {
				found = true;
				break;
			}
		}

		if (!found) {
			logger.warn("remove key " + std::to_string(key) + " not found");
			return;
		}

		logger.info("remove key " + std::to_string(key));
		//delete node(cursor).data_[idx];
		//删除后索引查不到，但是data_cache还保存记录
		node(cursor).size_--;
		for (int i = idx; i < node(cursor).size_; i++) {
			node(cursor).keys_[i] = node(cursor).keys_[i + 1];
			node(cursor).data_[i] = node(cursor).data_[i + 1];
		}

		node_cache_.set_dirty(cursor);

		//若删除的是叶子节点的第一个节点，则需要更新parent的key值
		if (l_s >= 0 && parent)
			node(parent).keys_[l_s] = node(cursor).keys_[0];

		if (cursor == root_) {
			if (node(cursor).size_ == 0) {
				logger.info("bptree died");
				add_available_id(root_);
				root_ = 0;
			}
			return;
		}

		//不需要借节点
		if (node(cursor).size_ >= (max_node_num + 1) >> 1) return;

		//向左右兄弟借节点
		if (l_s >= 0) {
			//有左兄弟
			auto& left_sibling = node(node(cursor).children_[0]);
			if (left_sibling.size_ >= ((max_node_num + 1) >> 1) + 1) {
				for (int i = node(cursor).size_; i > 0; i--) {
					node(cursor).keys_[i] = node(cursor).keys_[i - 1];
					node(cursor).data_[i] = node(cursor).data_[i - 1];
				}
				node(cursor).size_++;
				node(cursor).keys_[0] = left_sibling.keys_[left_sibling.size_ - 1];
				left_sibling.size_--;
				node(parent).keys_[l_s] = node(cursor).keys_[0];

				node_cache_.set_dirty(left_sibling.id_);
				node_cache_.set_dirty(cursor);
				return;
			}
		}
		if (r_s < node(parent).size_) {
			//有右兄弟
			auto& right_sibling = node(node(cursor).children_[1]);
			if (right_sibling.size_ >= ((max_node_num + 1) >> 1) + 1) {
				node(cursor).keys_[node(cursor).size_] = right_sibling.keys_[0];
				node(cursor).data_[node(cursor).size_] = right_sibling.data_[0];
				node(cursor).size_++;

				for (int i = 0; i < right_sibling.size_; i++) {
					right_sibling.keys_[i] = right_sibling.keys_[i + 1];
					right_sibling.data_[i] = right_sibling.data_[i + 1];
				}
				right_sibling.size_--;
				node(parent).keys_[r_s - 1] = right_sibling.keys_[0];

				node_cache_.set_dirty(right_sibling.id_);
				node_cache_.set_dirty(cursor);
				return;
			}
		}

		//与左右兄弟合并
		if (l_s >= 0) {
			//将自身合并到左兄弟
			auto& left_sibling = node(node(cursor).children_[0]);
			for (int i = left_sibling.size_, j = 0; j < node(cursor).size_; i++, j++) {
				left_sibling.keys_[i] = node(cursor).keys_[j];
				left_sibling.data_[i] = node(cursor).data_[j];
			}
			left_sibling.size_ += node(cursor).size_;

			//连接叶子节点链表
			left_sibling.children_[1] = node(cursor).children_[1];
			if (left_sibling.children_[1])
				node(left_sibling.children_[1]).children_[0] = left_sibling.id_;

			node_cache_.set_dirty(left_sibling.id_);
			node_cache_.set_dirty(cursor);

			remove_internal(node(parent).keys_[l_s], parent, cursor);
			//delete cursor;
		}
		else if (r_s < node(parent).size_) {
			//将右兄弟合并到自身
			auto& right_sibling = node(node(cursor).children_[1]);
			for (int i = node(cursor).size_, j = 0; j < right_sibling.size_; i++, j++) {
				node(cursor).keys_[i] = right_sibling.keys_[j];
				node(cursor).data_[i] = right_sibling.data_[j];
			}
			node(cursor).size_ += right_sibling.size_;

			node(cursor).children_[1] = right_sibling.children_[1];
			if (right_sibling.children_[1])
				node(node(cursor).children_[1]).children_[0] = cursor;

			remove_internal(node(parent).keys_[r_s - 1], parent, right_sibling.id_);
			//delete right_sibling;
		}
	}
}

void bptree::SBPTree::display(int cursor) {
	if (cursor) {
		logger << ">>\nnode: " << std::to_string(node(cursor).id_) << ", leaf: " << (node(cursor).leaf_ ? "true" : "false") << "\n";
		logger << "keys: ";
		for (int i = 0; i < node(cursor).size_; i++) {
			logger << std::to_string(node(cursor).keys_[i]) << " ";
		}
		logger << "\n";
		if (!node(cursor).leaf_) {
			logger << "children: ";
			for (int i = 0; i <= node(cursor).size_; i++) {
				logger << std::to_string(node(node(cursor).children_[i]).id_) << " ";
			}
		}
		else {
			logger << "values: ";
			for (int i = 0; i < node(cursor).size_; i++) {
				logger << std::to_string(node(cursor).data_[i]) << " ";
			}
			logger << "\nprev: " << std::to_string(node(cursor).children_[0]) << " ,next: " << std::to_string(node(cursor).children_[1]);
		}
		logger << "\n";
		if (!node(cursor).leaf_) {
			for (int i = 0; i < node(cursor).size_ + 1; i++) {
				display(node(cursor).children_[i]);
			}
		}
	}
}

int bptree::SBPTree::get_root() {
	return root_;
}
