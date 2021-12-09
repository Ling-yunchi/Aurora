#pragma once
#include <string>
#include <vector>

#include "Cache.hpp"
#include "SBPTree.h"
#include "serializable.h"

class Row;

class Table :public serializable<40 + 4 + 40 * 10 + (4 + 4 + 4 + bptree::max_free_node * 4)>
{
	std::string name_;					//最长40个字符
	int column_num_ = 0;				//最多10个字段
	std::vector<std::string> columns_;	//每个字段名最长40个字符
public:
	Table(std::string name, int column_num, std::vector<std::string>& columns);
	explicit Table();
	char* serialize() override;
	void unserialize(char* buf) override;
};

class Row :public serializable<4 + 10 * 255> {
public:
	int size_;
	std::vector<std::string> data_;		//每个记录最多255字符
	Row();
	explicit Row(int size);
	char* serialize() override;
	void unserialize(char* buf) override;
};
