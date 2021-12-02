﻿#include <iostream>
#include <string>
#include <fstream>

#include "storage/BTree.h"
#include "parser/Engine.h"
#include "boost/serialization/access.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "log/Logger.h"
using namespace std;

void logger_test() {
	logger.enable_stdout(true);
	logger.info("test info");
	logger.error("test error");
	logger.warn("test warn");
}

void btree_test() {
	BTree b_tree;
	for (int i = 0; i < 100000; i++)
		b_tree.insert(i, new Data(i));

	srand(time(nullptr));

	for (int i = 0; i < 1000; i++) {
		int key = rand() % 10000;
		//cout << key << endl;
		b_tree.remove(key);
	}

	b_tree.ldr();
}

class Page {
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& id_;
		ar& node_ids_;
	}
public:
	Page() {}
	void set_id(int id) { id_ = id; }
	void set_node(vector<int>& v) { node_ids_ = v; }
	string to_string() {
		string out;
		out += "{id:";
		out += std::to_string(id_);
		out+=", nodes:";
		out += "[";
		for (int i = 0; i < node_ids_.size();i++) {
			if (i != 0) out += "," + std::to_string(node_ids_[i]);
			else out += std::to_string(node_ids_[i]);
		}
		out += "]}";
		return out;
	}
private:
	int id_;
	vector<int> node_ids_;
};

void test_serialize() {
	Page page;
	page.set_id(2);
	vector<int> ids(5);
	ids = { 2,3,4,5 };
	page.set_node(ids);
	ofstream out("./data.txt",ios::app);
	out.seekp(0,ios::end);
	cout << out.tellp();
	boost::archive::binary_oarchive sout(out);
	sout << page;
	//ifstream fi("./data.txt");
	//fi.seekg(69);
	//cout << fi.tellg() << endl;
	//boost::archive::binary_iarchive sin(fi);
	//Page p;
	//sin >> p;
	//cout << p.to_string();
}

int main()
{
	test_serialize();
	//string cmd;
	//Engine engine;
	//while (true) {
	//	engine.print_prompt();
	//	getline(cin, cmd);
	//	engine.analysis(cmd);
	//}

	return 0;
}