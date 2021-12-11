#include "Engine.h"
#include <iostream>
#include "../view/help.h"
#include "../storage/Table.h"


Engine::Engine()
{
	if (_access("test_i.txt", 0)) {
		bptree::SBPTree init("test_idx.txt");
		std::ofstream of("test_i.txt");
		auto buf = init.serialize();
		of.write(buf, init.get_storage_size());
		delete[] buf;
		of.close();
	}

	if (_access("test.txt", 0)) {
		vector<string> columns = { "name","sex","email" };
		Table table("test", 3, columns);
		std::ofstream of("test.txt");
		auto buf = table.serialize();
		of.write(buf, table.get_storage_size());
		delete[] buf;
		of.close();
	}

	table_ = new Table();

	fstream fis;
	fis.open("test.txt", std::ios::in | std::ios::out | std::ios::binary);

	auto buf_ = new char[table_->get_storage_size()];
	fis.read(buf_, table_->get_storage_size());
	table_->unserialize(buf_);
	delete[] buf_;

	index_ = new bptree::SBPTree("test_idx.txt");
	row_cache_ = new Cache<Row, 4 + 10 * 255>(1000, "test_data.txt");

	fstream fs;
	fs.open("test_i.txt", std::ios::in | std::ios::out | std::ios::binary);

	auto buf = new char[index_->get_storage_size()];
	fs.read(buf, index_->get_storage_size());
	index_->unserialize(buf);
	delete[] buf;
}

Engine::~Engine()
{
	std::ofstream of("test_i.txt");
	auto buf = index_->serialize();
	of.write(buf, index_->get_storage_size());
	delete[] buf;
	of.close();

	of.open("test.txt");
	buf = table_->serialize();
	of.write(buf, table_->get_storage_size());
	delete[] buf;
	of.close();

	delete table_;
	delete index_;
	delete row_cache_;
}

void Engine::analysis(std::string& cmd)
{
	//cout << (cmd.empty() ? "" : cmd + "\n");
	lexicalAnalysis.set_str(cmd);
	lexicalAnalysis.analysis();
	if (lexicalAnalysis.is_legal()) {
		switch (lexicalAnalysis.get_keyword()) {
		case help_: help(); break;
			//case create_: create(); break;
		case insert_: insert_f(); break;
		case select_: select_f(); break;
		case update_: break;
		case delete_: break;
		default:break;
		}
	}

}

void Engine::print_prompt()
{
	cout << "Aurora > ";
}

void Engine::help() {
	view::help();
}

void Engine::insert_f() {
	cout << "input {name} {sex} {email}:" << endl;
	vector<string> data(3);
	for (int i = 0; i < 3; i++) {
		string tmp;
		cin >> tmp;
		data[i] = tmp;
	}
	cin.get();
	Row a_row(3);
	a_row.data_ = data;
	index_->insert(table_->auto_increase_id, table_->auto_increase_id);
	row_cache_->insert_item(table_->auto_increase_id, a_row);
	cout << "row id: " << table_->auto_increase_id << ", data: " << data[0] << " " << data[1] << " " << data[2] << " inserted" << endl;
	table_->auto_increase_id++;
}

void Engine::select_f() {
	cout << "input row id" << endl;
	int id = -1;
	cin >> id;
	cin.get();
	if (id != -1) {
		int data_id = index_->search(id);
		auto& row = row_cache_->get_item(data_id);
		auto& data = row.data_;
		if (data_id == 0)
			cout << "don't find data" << endl;
		else
			cout << "row id: " << data_id << ", data: " << data[0] << " " << data[1] << " " << data[2] << endl;
	}
}

void Engine::update_f() {}
void Engine::delete_f() {}
