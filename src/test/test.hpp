#pragma once
#include <cstdio>
//#include <boost/archive/binary_oarchive.hpp>
#include "../log/Logger.h"
#include "../controller/Engine.h"
#include "../storage/BPTree.h"
#include "../storage/BTree.h"
#include "../storage/Page.hpp"
#include "../storage/serializable.h"
#include "random"
#include "chrono"
//#include "boost/archive/binary_iarchive.hpp"
//#include "boost/archive/binary_oarchive.hpp"
//#include "boost/serialization/access.hpp"
//#include "boost/serialization/vector.hpp"
#include "./storage/Cache.hpp"
#include "../storage/Table.h"

using namespace std;

namespace test
{
	void logger_test() {
		logger.enable_stdout(true);
		logger.info("test info");
		logger.error("test error");
		logger.warn("test warn");
	}

	void btree_test() {
		btree::BTree b_tree;
		for (int i = 0; i < 100000; i++)
			b_tree.insert(i, new Data(i));

		srand(time(nullptr));

		for (int i = 0; i < 1000; i++) {
			int key = rand() % 10000;
			//cout << key << endl;
			b_tree.remove(key);
		}

		//b_tree.ldr();
	}

	void bptree_test() {
		logger.info("-------bptree_test-------");
		//logger.enable_stdout(true);
		bptree::BPTree tree;
		//tree.insert(5,new Data(5));
		//tree.insert(15,new Data(15));
		//tree.insert(25,new Data(25));
		//tree.insert(35,new Data(35));
		//tree.insert(45,new Data(45));
		for (int i = 1; i <= 10000; i++) {
			tree.insert(i, new Data(i));
		}

		//tree.display(tree.get_root());

		srand(time(nullptr));

		for (int i = 1; i <= 1000; i++) {
			int key = rand() % 10000;
			logger.info("remove key: " + std::to_string(key));
			tree.remove(key);
		}
		tree.remove(-1);

		for (int i = 1; i <= 1000; i++) {
			int key = rand() % 10000;
			logger.info("to find key " + std::to_string(key));
			auto data = tree.search(key);
			if (data) logger.info("data " + to_string(data->data_));
		}

		//tree.display(tree.get_root());
		logger.info("-----bptree_test end-----");
	}

	class Page {
		//friend class boost::serialization::access;

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
			out += ", nodes:";
			out += "[";
			for (int i = 0; i < node_ids_.size(); i++) {
				if (i != 0) out += "," + std::to_string(node_ids_[i]);
				else out += std::to_string(node_ids_[i]);
			}
			out += "]}";
			return out;
		}
	private:
		int id_;
		std::vector<int> node_ids_;
	};

	void test_boost_serialize() {
		//Page page;
		//page.set_id(2);
		//std::vector<int> ids(5);
		//ids = { 2,3,4,5 };
		//page.set_node(ids);
		//std::ofstream out("./data.dat", std::ios::app);
		//out.seekp(0, ios::end);
		//cout << out.tellp();
		//boost::archive::binary_oarchive sout(out);
		//sout << page;
		////ifstream fi("./data.dat");
		////fi.seekg(69);
		////cout << fi.tellg() << endl;
		////boost::archive::binary_iarchive sin(fi);
		////Page p;
		////sin >> p;
		////cout << p.to_string();
	}

	class inner_node :public serializable<4> {
	public:
		int id_;
		inner_node() = default;
		inner_node(int id) :id_(id) {}
		char* serialize() override {
			auto buf = new char[this->storage_size_];
			auto tmp_buf = buf;
			//auto int_buf = reinterpret_cast<int*>(buf);
			//*int_buf = id_;
			serialize_int(id_, tmp_buf);
			return buf;
		}
		void unserialize(char* buf) {
			unserialize_int(id_, buf);
			//auto int_buf = reinterpret_cast<int*>(buf);
			//id_ = *int_buf;
		}
	};

	constexpr int node_max = 5;
	class node : public serializable<2 * sizeof(int) + 5 * 2 * sizeof(int)> {
	public:
		int id_;
		vector<int> keys_;
		vector<inner_node> nodes_;
		int size_;
		node() {
			keys_.resize(node_max);
			nodes_.resize(node_max);
		}
		void display() {
			logger << "id: " << to_string(id_) << ", size: " << to_string(size_) << "\n";
			for (int i = 0; i < size_; i++)
				logger << keys_[i] << " ";
			logger << "\n";
			for (int i = 0; i < size_; i++)
				logger << nodes_[i].id_ << " ";
			logger << "\n";
		}
		char* serialize() override {
			auto buf = new char[this->storage_size_];
			auto temp_buf = buf;
			*reinterpret_cast<int*>(temp_buf) = id_;
			temp_buf += 4;
			*reinterpret_cast<int*>(temp_buf) = size_;
			temp_buf += 4;
			serialize_vector(keys_, size_, temp_buf);
			serialize_vector(nodes_, size_, temp_buf);
			return buf;
		}
		void unserialize(char* buf) override {
			auto temp_buf = buf;
			id_ = *reinterpret_cast<int*>(temp_buf);
			temp_buf += 4;
			size_ = *reinterpret_cast<int*>(temp_buf);
			temp_buf += 4;
			unserialize_vector(keys_, size_, temp_buf);
			unserialize_vector(nodes_, size_, temp_buf);
		}
	};

	void test_my_serialize() {
		logger.enable_stdout(true);
		fstream file("./data.dat", ios::in | ios::out | ios::trunc | ios::binary);
		for (int i = 1; i <= 10; i++) {
			node n;
			n.id_ = i;
			n.size_ = 3;
			n.keys_ = { i,i + 1,i + 2 };
			n.nodes_ = { inner_node(i + 1),inner_node(i + 2),inner_node(i + 3) };
			auto buf = n.serialize();
			file.write(buf, n.get_storage_size());
			delete[] buf;
		}

		file << flush;

		node n;
		file.seekg(n.get_storage_size(), ios::beg);
		auto buf = new char[n.get_storage_size()];
		file.read(buf, n.get_storage_size());
		n.unserialize(buf);
		delete[] buf;
		n.display();
	}

	void test_file() {
		fstream f("./qwq.dat", ios::in | ios::out | ios::binary);
		//预分配空间
		f.seekp(1024 * 1024 * 10);
		f.write("end", 3);
	}

	void test_cache() {
		Cache<inner_node, 4> node_cache(10, "node.dat");
		//for (int i = 0; i < 10000; i++) {
		//	node_cache.insert_item(i, inner_node(i));
		//}

		//logger << node_cache.get_item(10).id_ << lg::endl;
		//logger << node_cache.get_item(350).id_ << lg::endl;
		//logger << node_cache.get_item(999).id_ << lg::endl;
		//logger << node_cache.get_item(9999).id_ << lg::endl;

		default_random_engine e;
		uniform_int<int> rand_int(0, 9999);

		for (int i = 0; i < 1000; i++) {
			int key = rand_int(e);
			logger << "get item " << key << lg::endl;
			logger << node_cache.get_item(key).id_ << lg::endl;
		}

	}

	void test_sbptree() {
		logger.enable_stdout(true);
		bptree::SBPTree index_("test_idx.dat");
		for (int i = 1; i <= 10; i++) {
			logger << "----- insert " + to_string(i) + " -----" << lg::endl;
			index_.insert(i, i);
			index_.display(index_.get_root());
		}
		index_.display(index_.get_root());
	}

	void test_table() {
		vector<string> columns = { "c_1","c_2","c_3" };
		Table table("test", 3, columns);
		bptree::SBPTree index_("test_idx.dat");
		Cache<Row, 4 + 10 * 255> row_cache_(1000, "test_data.dat");

		//for (int i = 0; i < 10000; i++) {
		//	index_.insert(i, i);
		//	Row a_row(3);
		//	a_row.data_ = { "data_" + to_string(i),"data_" + to_string(i),"data_" + to_string(i) };
		//	row_cache_.insert_item(i, a_row);
		//}
		//index_.insert(1, 1);
		//Row a_row(3);
		//a_row.data_ = { "data_1","data_2","data_3" };
		//row_cache_.insert_item(1, a_row);

		fstream fs;
		fs.open("test.dat", std::ios::in | std::ios::out | std::ios::binary);

		//if (!fs.is_open()) {
		//	logger.error("cache init failed: file open fail");
		//}

		//auto buf = index_.serialize();
		//fs.write(buf, index_.get_storage_size());
		//fs.close();
		//delete[] buf;

		auto buf = new char[index_.get_storage_size()];
		fs.read(buf, index_.get_storage_size());
		index_.unserialize(buf);
		delete[] buf;

		default_random_engine e;
		uniform_int<int> rand_int(0, 9999);

		for (int i = 0; i < 1000; i++) {
			int key = rand_int(e);
			int data_id = index_.search(key);
			auto& row = row_cache_.get_item(data_id);
			logger << "-----data " + to_string(data_id) + "-----" << lg::endl;
			for (int j = 0; j < row.size_; j++) {
				logger << row.data_[j] << " ";
			}
			logger << lg::endl;
		}
	}

	//void test_table_manager() {
	//	TableManager manager;
	//	vector<string> column = { "qwq1","qwq2","qwq3" };
	//	manager.add_table("test", 3, column);
	//	manager.choose_table("test");
	//	Row row(3);
	//	row.data_ = { "qwq_1","qwq_2","qwq_3" };
	//	manager.insert_table(row);
	//	auto qwq = manager.search_table(1);
	//}

	void test_one_line_out() {
		for (int i = 0; i < 1000000; i++) {
			printf("inserting row %6d : %2.2f%%", i, (double)i / 10000);
			printf("\r");
		}
	}

	inline void test_entry() {
		logger << logger.get_now_time() << " [test] ----------start----------" << lg::endl;
		auto start = chrono::system_clock::now();

		test_one_line_out();

		auto end = chrono::system_clock::now();
		auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
		auto cost = double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den;
		logger << logger.get_now_time() << " [test] test finished at " + to_string(cost) + " seconds" << lg::endl;
		logger << logger.get_now_time() << " [test] ----------end----------" << lg::endl;
	}
}
