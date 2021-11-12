#include "BTree.h"

#include <iostream>

BTreeNode::BTreeNode(bool leaf)
{
	keys_.resize(M * 2 - 1);
	children_.resize(M * 2);
	data_.resize(M * 2 - 1);
	size_ = 0;
	leaf_ = leaf;
}

Data* BTreeNode::search(int key)
{
	int idx = 0;
	while (idx < size_ && key > keys_[idx]) idx++;
	if (idx == size_) children_[idx]->search(key);
	if (keys_[idx] == key) return data_[idx];
	if (leaf_) return nullptr;
	return children_[idx]->search(key);
}

void BTreeNode::insert_non_full(int key, Data* data)
{
	int idx = size_ - 1;
	if (leaf_ == true) {
		while (idx >= 0 && keys_[idx] > key) {
			keys_[idx + 1] = keys_[idx];
			data_[idx + 1] = data_[idx];
			idx--;
		}
		keys_[idx + 1] = key;
		data_[idx + 1] = data;
		size_++;
	}
	else {
		while (idx >= 0 && keys_[idx] > key) idx--;
		if (children_[idx + 1]->size_ == M * 2 - 1) {
			split_child(idx + 1, children_[idx + 1]);
			if (keys_[idx + 1] < key) idx++;
		}
		children_[idx + 1]->insert_non_full(key, data);
	}
}

void BTreeNode::split_child(int idx, BTreeNode* child)
{
	auto new_node = new BTreeNode(child->leaf_);
	new_node->size_ = M - 1;

	//拷贝child的后半段数据
	for (int i = 0; i < M - 1; i++) {
		new_node->keys_[i] = child->keys_[i + M];
		new_node->data_[i] = child->data_[i + M];
	}

	//若子节点不为叶子节点,拷贝孩子信息
	if (!child->leaf_)
		for (int i = 0; i < M; i++)
			new_node->children_[i] = child->children_[i + M];

	//减少数量代替删除
	child->size_ = M - 1;

	for (int i = size_ - 1; i >= idx; i--) {
		children_[i + 2] = children_[i + 1];
		keys_[i + 1] = keys_[i];
		data_[i + 1] = data_[i];
	}

	children_[idx + 1] = new_node;
	keys_[idx] = child->keys_[M - 1];
	data_[idx] = child->data_[M - 1];

	size_++;
}

void BTreeNode::ldr()
{
	if (leaf_)
		for (int i = 0; i < size_; i++)
			cout << keys_[i] << ":" << data_[i]->data_ << " ";
	else {
		for (int i = 0; i < size_; i++) {
			children_[i]->ldr();
			cout << keys_[i] << ":" << data_[i]->data_ << " ";
		}
		children_[size_]->ldr();
	}
}

Data* BTree::search(int key)
{
	return root_ == nullptr ? nullptr : root_->search(key);
}

void BTree::insert(int key, Data* data)
{
	if (root_ == nullptr) {
		root_ = new BTreeNode(true);
		root_->keys_[0] = key;
		root_->data_[0] = data;
		root_->size_ = 1;
	}
	else {
		if (root_->size_ == M * 2 - 1) {
			auto new_root = new BTreeNode(false);
			new_root->children_[0] = root_;
			new_root->split_child(0, root_);

			int idx = new_root->keys_[0] < key ? 1 : 0;
			new_root->children_[idx]->insert_non_full(key, data);

			root_ = new_root;
		}
		else
			root_->insert_non_full(key, data);
	}
}

void BTree::ldr()
{
	root_->ldr();
}




