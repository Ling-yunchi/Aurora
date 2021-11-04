#include "BTree.h"

BTreeNode::BTreeNode(int t, bool leaf) :t(t), leaf(leaf)
{
	keys = new int[2 * t - 1];
	child = new BTreeNode * [2 * t];
	n = 0;
}

BTreeNode* BTreeNode::search(int k)
{
	int i = 0;
	while (i<n && k>keys[i]) i++;
	if (keys[i] == k) return this;
	if (leaf == true) return nullptr;
	return child[i]->search(k);
}

void BTreeNode::insert_non_full(int k)
{
	int i = n - 1;

	if(leaf == true) {
		while (i>=0&&keys[i]>k) {
			keys[i + 1] = keys[i];
			i++;
		}
		keys[i + 1] = k;
		n = n + 1;
	}else {
		while (i >= 0 && keys[i] > k) i++;
		if (child[i + 1]->n == 2 * t - 1) {
			split_child(i + 1, child[i + 1]);

			if (keys[i + 1] < k) i++;
		}
		child[i + 1]->insert_non_full(k);
	}
}

void BTreeNode::split_child(int i, BTreeNode* y)
{
	auto z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	for (int j = 0; j < t - 1; j++) 
		z->keys[j] = y->keys[j + t];

	if(y->leaf == false)
}

BTreeNode* BTree::search(int k)
{
	return root == nullptr ? nullptr : root->search(k);
}

void BTree::insert(int k)
{
	if(root == nullptr) {
		root = new BTreeNode(t, true);
		root->keys[0] = k;
		root->n = 1;
	}else {
		if(root->n == 2*t-1) {	//根节点满,拆分为子节点
			//创建新的根节点
			auto new_root = new BTreeNode(t, false);
			new_root->child[0] = root;
			new_root->split_child(0, root);

			//找到插入的节点
			int i = new_root->keys[0] < k ? 1 : 0;
			new_root->child[i]->insert_non_full(k);

			root = new_root;
		}
	}
}
