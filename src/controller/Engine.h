#pragma once
#include <xstring>
#include "../storage/Table.h"

#include "LexicalAnalysis.h"

class Engine
{
	Table* table_;
	bptree::SBPTree* index_;
	Cache<Row, 4 + 10 * 255>* row_cache_;
public:
	Engine();

	~Engine();

	//分析指令
	void analysis(std::string& cmd);
	void print_prompt();

	void help() const;

	//void create();
	void insert_f();
	void select_f();
	void update_f();
	void delete_f();
	void test_f();


private:
	LexicalAnalysis lexicalAnalysis;
	void clear();
};
