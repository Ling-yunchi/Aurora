#include "BTree.h"

BTreeNode::BTreeNode(BTreeNode* parent,int t, bool leaf) :
	keys(new int[2 * t - 1]),
	t(t),
	num(0),
	leaf(leaf),
	parent(parent),
	child(new BTreeNode* [2 * t])
{}

BTreeNode* BTreeNode::search(int key)
{
	//找到第一个大于等于传入key的key
	int i = 0;
	while (i<num && key>keys[i]) i++;
	//如果传入key与其相等
	if (keys[i] == key) return this;
	//如果该节点为叶子节点,则不存在需要找的key
	if (leaf == true) return nullptr;
	//去子节点找key
	return child[i]->search(key);
}

void BTreeNode::insert_non_full(int key)
{
	//初始化idx为最后一个存储的key的位置
	int idx = num - 1;
	if (leaf == true) {	//若当前节点为叶子节点
		//找到插入的位置
		while (idx >= 0 && keys[idx] > key) {
			keys[idx + 1] = keys[idx];	//将key后移
			idx--;
		}
		keys[idx + 1] = key;
		num = num + 1;
	}
	else {
		//找到需要插入的子节点的key
		while (idx >= 0 && keys[idx] > key) idx--;
		if (child[idx + 1]->num == 2 * t - 1) {//若子节点已满
			//分裂子节点
			split_child(idx + 1, child[idx + 1]);
			//如果分裂后当前节点
			if (keys[idx + 1] < key) idx++;
		}
		child[idx + 1]->insert_non_full(key);
	}
}

void BTreeNode::split_child(int idx, BTreeNode* y)
{
	//新建节点存储y中(t-1)个keys
	auto z = new BTreeNode(this, y->t, y->leaf);
	z->num = t - 1;
	//复制y的最后(t-1)个key到z
	for (int i = 0; i < t - 1; i++)
		z->keys[i] = y->keys[i + t];
	//复制y的后(t-1)个chile到z
	if (y->leaf == false)
		for (int j = 0; j < t; j++) {
			z->child[j] = y->child[j + t];
		}
	//减少y的key的个数
	y->num = t - 1;
	//后移当前节点的child
	for (int i = num; i >= idx + 1; i--)
		child[i + 1] = child[i];
	//将新建节点作为当前节点的子节点
	child[idx + 1] = z;

	for (int i = num - 1; i >= idx; i--)
		keys[i + 1] = keys[i];

	keys[idx] = y->keys[t - 1];

	num = num + 1;
}

BTreeNode* BTree::search(int k)
{
	return root == nullptr ? nullptr : root->search(k);
}

void BTree::insert(int key)
{
	if (root == nullptr) {
		root = new BTreeNode(nullptr, t, true);
		root->keys[0] = key;
		root->num = 1;
	}
	else {
		if (root->num == 2 * t - 1) {	//根节点满,拆分为子节点
			//创建新的根节点
			auto new_root = new BTreeNode(nullptr, t, false);
			new_root->child[0] = root;
			new_root->split_child(0, root);

			//找到插入的节点
			int i = new_root->keys[0] < key ? 1 : 0;
			new_root->child[i]->insert_non_full(key);

			root = new_root;
		}
		else
			root->insert_non_full(key);
	}
}
