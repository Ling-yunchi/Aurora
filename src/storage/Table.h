#pragma once
#include <string>
#include <vector>

#include "Cache.hpp"
#include "SBPTree.h"
#include "serializable.h"

class Row;

class Table :public serializable<40 + 4 + 40 * 10 + (4 + 4 + 4 + bptree::max_free_node * 4)>
{
	std::string name_;					//�40���ַ�
	int column_num_ = 0;				//���10���ֶ�
	std::vector<std::string> columns_;	//ÿ���ֶ����40���ַ�
	bptree::SBPTree index_;				//����
	Cache<Row, 4 + 10 * 255> row_cache_;//������
	Table(std::string name, int column_num, std::vector<std::string>& columns);
	explicit Table(std::string filename);
	char* serialize() override;
	void unserialize(char* buf) override;
};

class Row :public serializable<4 + 10 * 255> {
public:
	int size_;
	std::vector<std::string> data_;		//ÿ����¼���255�ַ�
	Row(int size);
	char* serialize() override;
	void unserialize(char* buf) override;
};
