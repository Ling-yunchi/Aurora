#include "BTree.h"

#include <iostream>

btree::BTreeNode::BTreeNode(bool leaf)
{
	keys_.resize(order * 2 - 1);
	children_.resize(order * 2);
	data_.resize(order * 2 - 1);
	size_ = 0;
	leaf_ = leaf;
}

Data* btree::BTreeNode::search(int key)
{
	int idx = find_key(key);
	if (idx == size_) children_[idx]->search(key);
	if (keys_[idx] == key) return data_[idx];
	if (leaf_) return nullptr;
	return children_[idx]->search(key);
}

//���ص�һ�����ڵ���key���±�
int btree::BTreeNode::find_key(int key)
{
	int idx = 0;
	while (idx < size_ && keys_[idx] < key) idx++;
	return idx;
}

//����һ��δ���Ľڵ�
void btree::BTreeNode::insert_non_full(int key, Data* data)
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
		if (children_[idx + 1]->size_ == order * 2 - 1) {
			split_child(idx + 1, children_[idx + 1]);
			if (keys_[idx + 1] < key) idx++;
		}
		children_[idx + 1]->insert_non_full(key, data);
	}
}

void btree::BTreeNode::remove(int key)
{
	int idx = find_key(key);
	if (idx < size_ && keys_[idx] == key) {
		if (leaf_) remove_from_leaf(idx);
		else remove_from_non_leaf(idx);
	}
	else {
		if (leaf_) return;	//key������

		bool flag = idx == size_;

		//���Ҫɾ���ĺ��ӽڵ��СС��M,���ú��ӽڵ�
		if (children_[idx]->size_ < order)
			fill(idx);	//size_��ֵ���ܱ��ı�

		if (flag && idx > size_)
			children_[idx - 1]->remove(key);
		else
			children_[idx]->remove(key);
	}
}

void btree::BTreeNode::remove_from_leaf(int idx)
{
	delete data_[idx];
	for (int i = idx + 1; i < size_; i++) {
		keys_[i - 1] = keys_[i];
		data_[i - 1] = data_[i];
		//children_[i - 1] = children_[i];
	}
	size_--;
}

void btree::BTreeNode::remove_from_non_leaf(int idx)
{
	delete data_[idx];
	data_[idx] = nullptr;
	int key = keys_[idx];
	if (children_[idx]->size_ >= order) {
		//��ǰ��
		BTreeNode* pred = children_[idx];
		while (!pred->leaf_)
			pred = pred->children_[pred->size_];

		//��ǰ����key��data��ֵ����ǰ�ڵ�
		keys_[idx] = pred->keys_[pred->size_ - 1];
		data_[idx] = pred->data_[pred->size_ - 1];
		//��ǰ��data��Ϊ��,��ֹremoveʱ���ͷ�
		pred->data_[pred->size_ - 1] = nullptr;
		//ɾ��ǰ��
		children_[idx]->remove(pred->keys_[pred->size_ - 1]);
	}
	else if (children_[idx + 1]->size_ >= order) {
		BTreeNode* succ = children_[idx + 1];
		while (!succ->leaf_)
			succ = succ->children_[0];

		keys_[idx] = succ->keys_[0];
		data_[idx] = succ->data_[0];
		succ->data_[0] = nullptr;
		children_[idx + 1]->remove(succ->keys_[0]);
	}
	else {
		merge(idx);
		children_[idx]->remove(key);
	}
}

//���children[i]�ڵ�ʹ���С����M
void btree::BTreeNode::fill(int idx)
{
	//��Ҫ���Ľڵ��±겻Ϊ0(�����ֵ�)�����ֵܴ�С���ڵ���M,�����ֵܽ�ڵ�
	if (idx != 0 && children_[idx - 1]->size_ >= order)
		borrow_from_prev(idx);
	//�������ֵ������ֵܴ�С���ڵ���M,�����ֵܽ�ڵ�
	else if (idx != size_ && children_[idx + 1]->size_ >= order)
		borrow_from_next(idx);
	else {

		if (idx != size_)	//���ýڵ������ֵ�,���������ֵܺϲ�
			merge(idx);
		else				//���������ֵ���ýڵ�ϲ�
			merge(idx - 1);
	}
}

//�����ֵܽ�ڵ�
void btree::BTreeNode::borrow_from_prev(int idx)
{
	BTreeNode* child = children_[idx];
	BTreeNode* sibling = children_[idx - 1];

	//����child�ڵ�����
	for (int i = child->size_ - 1; i >= 0; i--) {
		child->keys_[i + 1] = child->keys_[i];
		child->data_[i + 1] = child->data_[i];
	}

	if (!child->leaf_)
		for (int i = child->size_; i >= 0; i--)
			child->children_[i + 1] = child->children_[i];

	//���Ʊ��ڵ��Ӧ���ֵܽڵ��key
	child->keys_[0] = keys_[idx - 1];
	child->data_[0] = data_[idx - 1];

	if (!child->leaf_)
		child->children_[0] = sibling->children_[sibling->size_];

	//�����ֵ����ҽڵ�����
	keys_[idx - 1] = sibling->keys_[sibling->size_ - 1];
	data_[idx - 1] = sibling->data_[sibling->size_ - 1];


	child->size_++;
	sibling->size_--;
}

//�����ֵܽ�ڵ�
void btree::BTreeNode::borrow_from_next(int idx)
{
	BTreeNode* child = children_[idx];
	BTreeNode* sibling = children_[idx + 1];

	//���ú��ӽڵ��Ӧ��key�·Ÿ��ú��ӽڵ�
	child->keys_[child->size_] = keys_[idx];
	child->data_[child->size_] = data_[idx];

	if (!child->leaf_)
		child->children_[child->size_ + 1] = sibling->children_[0];

	//�������ֵܽ�Ľڵ���뱾�ڵ��key��
	keys_[idx] = sibling->keys_[0];
	data_[idx] = sibling->data_[0];

	//�������ֵ�
	for (int i = 1; i < sibling->size_; i++) {
		sibling->keys_[i - 1] = sibling->keys_[i];
		sibling->data_[i - 1] = sibling->data_[i];
	}
	if (!sibling->leaf_)
		for (int i = 1; i <= sibling->size_; i++)
			sibling->children_[i - 1] = sibling->children_[i];

	child->size_++;
	sibling->size_--;
}

//�ϲ�children_[idx + 1]��children_[idx],�ϲ���children_[idx + 1]�ᱻ�ͷ�
//children_[idx]�Ĵ�СС��M
void btree::BTreeNode::merge(int idx)
{
	BTreeNode* child = children_[idx];
	BTreeNode* sibling = children_[idx + 1];

	child->keys_[order - 1] = keys_[idx];
	child->data_[order - 1] = data_[idx];

	for (int i = 0; i < sibling->size_; i++) {
		child->keys_[i + order] = sibling->keys_[i];
		child->data_[i + order] = sibling->data_[i];
	}
	if (!child->leaf_)
		for (int i = 0; i <= sibling->size_; i++)
			child->children_[i + order] = sibling->children_[i];

	for (int i = idx + 1; i < size_; i++) {
		keys_[i - 1] = keys_[i];
		data_[i - 1] = data_[i];
	}

	for (int i = idx + 2; i <= size_; i++) {
		children_[i - 1] = children_[i];
	}

	child->size_ += sibling->size_ + 1;
	size_--;

	delete sibling;
}

//���Ѻ��ӽڵ�,childΪchildren[idx]
void btree::BTreeNode::split_child(int idx, BTreeNode* child)
{
	auto new_node = new BTreeNode(child->leaf_);
	new_node->size_ = order - 1;

	//����child�ĺ�������
	for (int i = 0; i < order - 1; i++) {
		new_node->keys_[i] = child->keys_[i + order];
		new_node->data_[i] = child->data_[i + order];
	}

	//���ӽڵ㲻ΪҶ�ӽڵ�,����������Ϣ
	if (!child->leaf_)
		for (int i = 0; i < order; i++)
			new_node->children_[i] = child->children_[i + order];

	//������������ɾ��
	child->size_ = order - 1;

	//���Ʊ��ڵ������
	for (int i = size_ - 1; i >= idx; i--) {
		children_[i + 2] = children_[i + 1];
		keys_[i + 1] = keys_[i];
		data_[i + 1] = data_[i];
	}

	children_[idx + 1] = new_node;
	keys_[idx] = child->keys_[order - 1];
	data_[idx] = child->data_[order - 1];

	size_++;
}

void btree::BTreeNode::ldr()
{
	if (leaf_)
		for (int i = 0; i < size_; i++) {
			//if (keys_[i] != data_[i]->data_)
			cout << keys_[i] << ":" << data_[i]->data_ << " ";
		}
	else {
		for (int i = 0; i < size_; i++) {
			children_[i]->ldr();
			//if (keys_[i] != data_[i]->data_)
			cout << keys_[i] << ":" << data_[i]->data_ << " ";
		}
		children_[size_]->ldr();
	}
}

void btree::BTreeNode::show_node() {
	cout << "----------" << endl;
	for (int i = 0; i < size_; i++)
		cout << keys_[i] << " ";
	cout << endl;
	for (int i = 0; i < size_; i++)
		cout << children_[i] << " ";
	cout << endl;
	for (int i = 0; i < size_; i++)
		cout << data_[i] << " ";
	cout << endl;
	cout << "----------" << endl;
}

void btree::BTreeNode::clear() {
	if (!leaf_)
		for (int i = 0; i <= size_; i++) {
			children_[i]->clear();
			delete children_[i];
		}
	for (int i = 0; i < size_; i++)
		delete data_[i];
}

btree::BTree::~BTree() {
	root_->clear();
	delete root_;
}

Data* btree::BTree::search(int key)
{
	return root_ == nullptr ? nullptr : root_->search(key);
}

void btree::BTree::insert(int key, Data* data)
{
	if (root_ == nullptr) {
		root_ = new BTreeNode(true);
		root_->keys_[0] = key;
		root_->data_[0] = data;
		root_->size_ = 1;
	}
	else {
		if (root_->size_ == order * 2 - 1) {
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

void btree::BTree::ldr()
{
	root_->ldr();
}

void btree::BTree::remove(int key)
{
	if (!root_) return;

	root_->remove(key);

	if (root_->size_ == 0) {
		BTreeNode* tmp = root_;
		if (root_->leaf_)
			root_ = nullptr;
		else
			root_ = root_->children_[0];

		delete tmp;
	}
}