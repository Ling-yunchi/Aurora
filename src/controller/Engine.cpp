#include "Engine.h"

#include <chrono>
#include <iostream>
#include "../view/help.h"
#include "../storage/Table.h"


Engine::Engine()
{
	if (_access("test_i.dat", 0)) {
		bptree::SBPTree init("test_idx.dat");
		std::ofstream of("test_i.dat");
		auto buf = init.serialize();
		of.write(buf, init.get_storage_size());
		delete[] buf;
		of.close();
	}

	if (_access("test.dat", 0)) {
		vector<string> columns = { "name","sex","email" };
		Table table("test", 3, columns);
		std::ofstream of("test.dat");
		auto buf = table.serialize();
		of.write(buf, table.get_storage_size());
		delete[] buf;
		of.close();
	}

	table_ = new Table();

	fstream fis;
	fis.open("test.dat", std::ios::in | std::ios::out | std::ios::binary);

	auto buf_ = new char[table_->get_storage_size()];
	fis.read(buf_, table_->get_storage_size());
	table_->unserialize(buf_);
	delete[] buf_;

	index_ = new bptree::SBPTree("test_idx.dat");
	row_cache_ = new Cache<Row, 4 + 10 * 255>(1000, "test_data.dat");

	fstream fs;
	fs.open("test_i.dat", std::ios::in | std::ios::out | std::ios::binary);

	auto buf = new char[index_->get_storage_size()];
	fs.read(buf, index_->get_storage_size());
	index_->unserialize(buf);
	delete[] buf;
}

Engine::~Engine()
{
	std::ofstream of("test_i.dat");
	auto buf = index_->serialize();
	of.write(buf, index_->get_storage_size());
	delete[] buf;
	of.close();

	of.open("test.dat");
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
		case update_: update_f(); break;
		case delete_: delete_f(); break;
		case test_: test_f(); break;
		default:break;
		}
	}

}

void Engine::print_prompt()
{
	cout << "aurora > ";
}

void Engine::help() const {
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
	cout << "input row id: ";
	int id = -1;
	cin >> id;
	cin.get();
	if (id != -1) {
		int data_id = index_->search(id);
		if (data_id == 0)
			cout << "don't find data" << endl;
		else {
			auto& row = row_cache_->get_item(data_id);
			auto& data = row.data_;
			cout << "row id: " << data_id << ", data: " << data[0] << " " << data[1] << " " << data[2] << endl;
		}
	}
}

void Engine::update_f() {
	cout << "input row id: ";
	int id = -1;
	cin >> id;
	cin.get();
	if (id != -1) {
		int data_id = index_->search(id);
		auto& row = row_cache_->get_item(data_id);
		auto& data = row.data_;
		if (data_id == 0)
			cout << "don't find data" << endl;
		else {
			cout << "row id: " << data_id << ", data: " << data[0] << " " << data[1] << " " << data[2] << endl;
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
			row_cache_->insert_item(id, a_row);
			cout << "update success" << endl;
		}
	}
}
void Engine::delete_f() {
	cout << "input row id: ";
	int id = -1;
	cin >> id;
	cin.get();
	if (id != -1) {
		int data_id = index_->search(id);
		auto& row = row_cache_->get_item(data_id);
		auto& data = row.data_;
		if (data_id == 0)
			cout << "don't find data" << endl;
		else {
			cout << "row id: " << data_id << ", data: " << data[0] << " " << data[1] << " " << data[2] << endl;
			index_->remove(data_id);
			cout << "delete success" << endl;
		}
	}
}

void Engine::test_f() {
	cout
		<< "start : to clear the data and insert {n} data (n>0)" << endl
		<< "        format: id:{i} data: test_{i}, i % 2 ? male : female, {i}@test.com" << endl
		<< "clear : to clear all data" << endl;
	print_prompt();
	string in;
	getline(cin, in);
	auto token = split(in, ' ');
	if (token[0] == "start") {
		clear();
		int n = -1;
		cout << "input n: ";
		cin >> n;
		cin.get();
		if (n < 0) {
			cout << "invalid n" << endl;
			return;
		}
		auto start = chrono::system_clock::now();
		Row row(3);
		for (int i = 1; i <= n; i++) {
			printf("inserting row %d : %2.2f%%", i, (double)i / n * 100);
			row.data_ = { ("test_" + to_string(i)),i % 2 ? "male" : "female",(to_string(i) + "@test.com") };
			index_->insert(table_->auto_increase_id, table_->auto_increase_id);
			row_cache_->insert_item(table_->auto_increase_id, row);
			table_->auto_increase_id++;
			printf("\r");
		}
		auto end = chrono::system_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
		auto cost = double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den;
		cout << "\ninsert finished at " + to_string(cost) + " seconds";
		cout << "\ninsert " << n << " row complete" << endl;
	}
	else if (token[0] == "clear") {
		clear();
		cout << "clear complete" << endl;
	}
	else {
		cout << "invalid input" << endl;
	}
}

void Engine::clear() {
	delete table_;
	delete index_;
	delete row_cache_;

	remove("test.dat");
	remove("test_i.dat");
	remove("test_idx.dat");
	remove("test_data.dat");

	if (_access("test_i.dat", 0)) {
		bptree::SBPTree init("test_idx.dat");
		std::ofstream of("test_i.dat");
		auto buf = init.serialize();
		of.write(buf, init.get_storage_size());
		delete[] buf;
		of.close();
	}

	if (_access("test.dat", 0)) {
		vector<string> columns = { "name","sex","email" };
		Table table("test", 3, columns);
		std::ofstream of("test.dat");
		auto buf = table.serialize();
		of.write(buf, table.get_storage_size());
		delete[] buf;
		of.close();
	}

	table_ = new Table();

	fstream fis;
	fis.open("test.dat", std::ios::in | std::ios::out | std::ios::binary);

	auto buf_ = new char[table_->get_storage_size()];
	fis.read(buf_, table_->get_storage_size());
	table_->unserialize(buf_);
	delete[] buf_;

	index_ = new bptree::SBPTree("test_idx.dat");
	row_cache_ = new Cache<Row, 4 + 10 * 255>(1000, "test_data.dat");

	fstream fs;
	fs.open("test_i.dat", std::ios::in | std::ios::out | std::ios::binary);

	auto buf = new char[index_->get_storage_size()];
	fs.read(buf, index_->get_storage_size());
	index_->unserialize(buf);
	delete[] buf;
}

