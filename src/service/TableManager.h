//#pragma once
//#include <string>
//#include <vector>
//
//#include "../storage/Table.h"
//#include "../storage/SBPTree.h"
//const std::string sys_path = "./bin/sys.dat";
//const std::string data_path = "./data/";
//const int max_table_num = 100;
//
//class TableManager
//{
//	int table_num_;
//	std::vector<std::string> tables_{ max_table_num };
//	int storage_size_ = 4004;
//	std::string now_table_;
//	Table* table_ = nullptr;
//	bptree::SBPTree* index_ = nullptr;
//	Cache<Row, 4 + 10 * 255>* data_cache_ = nullptr;
//public:
//	TableManager();
//	~TableManager();
//	void show_tables();
//	void add_table(std::string name, int column_num, std::vector<std::string>& columns);
//	const Table* get_now_table();
//	void choose_table(std::string table_name);
//	std::vector<std::string>& get_columns();
//	Row& search_table(int key);
//	void insert_table(Row& row);
//	void update_table(int key, Row& row);
//	void delete_table(int key);
//	bool has_table(std::string table_name);
//};
//
