#pragma once
#include <string>
#include <vector>
using namespace std;

enum operation
{
	//	tables_,
	help_,
	//	create_,
	test_,
	insert_,
	select_,
	update_,
	delete_
};

class LexicalAnalysis
{
	string str;					//要分析的字符串
	operation keyword;			//操作
	vector<string> parameter;	//参数表
	bool legal;				//输入是否合法

	void command_help();
	void create_table();
	void insert_();
	void delete_();
	void select_();
	void update_();
public:
	LexicalAnalysis();
	~LexicalAnalysis() = default;

	operation& get_keyword() { return keyword; }
	vector<string> get_parameter() const { return parameter; }
	void set_str(const string& str) { this->str = str; }
	bool is_legal() { return legal; }
	void syntax_error();

	void analysis();
};

vector<string> split(const string& str, const char c);