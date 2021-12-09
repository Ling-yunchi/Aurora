#pragma once
#include <string>
#include <vector>
using namespace std;

enum operation
{
	create_table_,
	insert_,
	select_,
	update_,
	delete_
};

class LexicalAnalysis
{
	string str;					//Ҫ�������ַ���
	operation keyword;			//����
	vector<string> parameter;	//������
	bool legal;				//�����Ƿ�Ϸ�

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

	void analysis();
};

