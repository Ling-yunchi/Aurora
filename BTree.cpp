#include "BTree.h"

BTreeNode::BTreeNode()
{
	size_ = 0;
	keys_.resize(M * 2 - 1);
	children_.resize(M * 2);
	data_.resize(M * 2 - 1);
}
