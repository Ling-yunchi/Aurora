#pragma once
#include <boost/archive/binary_oarchive.hpp>
#include "../storage/BTree.h"
#include "../parser/Engine.h"
#include "boost/serialization/access.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include "../storage/BPTree.h"
#include "../log/Logger.h"
#include "../storage/serializable.h"
#include "../storage/Page.h"

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
		Page page;
		page.set_id(2);
		std::vector<int> ids(5);
		ids = { 2,3,4,5 };
		page.set_node(ids);
		std::ofstream out("./data.txt", std::ios::app);
		out.seekp(0, ios::end);
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

	constexpr int node_max = 5;
	class node : public serializable {
	public:
		int id_;
		vector<int> keys_;
		vector<int> data_ids_;
		int size_;
		node() :serializable(2 * sizeof(int) + 5 * 2 * sizeof(int)) {

		}
		void display() {
			logger << "id: " << to_string(id_) << ", size: " << to_string(size_) << "\n";
			for (int i = 0; i < size_; i++)
				logger << keys_[i] << " ";
			logger << "\n";
			for (int i = 0; i < size_; i++)
				logger << data_ids_[i] << " ";
			logger << "\n";
		}
		//void serialize(ostream& os) override {
		//	os << id_ << " " << size_ << " ";
		//	for (int i = 0; i < size_; i++)
		//		os << keys_[i] << " ";
		//	for (int i = 0; i < size_; i++)
		//		os << data_ids_[i] << " ";
		//}
		//void unserialize(istream& is) override {
		//	is >> id_;
		//	is >> size_;
		//	keys_.resize(size_);
		//	for (int i = 0; i < size_; i++)
		//		is >> keys_[i];
		//	data_ids_.resize(size_);
		//	for (int i = 0; i < size_; i++)
		//		is >> data_ids_[i];
		//}
		void serialize(ostream& os) override {
			auto buffer = new char[storage_size_];
			auto int_buffer = reinterpret_cast<int*>(buffer);
			*int_buffer++ = id_;
			*int_buffer++ = size_;
			//for (int i = 0; i < node_max; i++)
			//	if (i < size_) *int_buffer++ = keys_[i];
			//	else *int_buffer++ = 0;
			//for (int i = 0; i < node_max; i++)
			//	if (i < size_) *int_buffer++ = data_ids_[i];
			//	else *int_buffer++ = 0;
			serialize_vector(keys_, node_max, int_buffer);
			serialize_vector(data_ids_, node_max, int_buffer);
			os.write(buffer, storage_size_);
		}
		void unserialize(istream& is) override {
			auto buffer = new char[storage_size_];
			is.read(buffer, storage_size_);
			auto int_buffer = reinterpret_cast<int*>(buffer);
			id_ = *int_buffer++;
			size_ = *int_buffer++;
			unserialize_vector(keys_, node_max, int_buffer);
			unserialize_vector(data_ids_, node_max, int_buffer);
		}
	};

	void test_my_serialize() {
		logger.enable_stdout(true);
		fstream file("./data.txt", ios::in | ios::out | ios::trunc | ios::binary);
		for (int i = 1; i <= 10; i++) {
			node n;
			n.id_ = i;
			n.size_ = 3;
			n.keys_ = { i,i + 1,i + 2 };
			n.data_ids_ = { i,i + 1,i + 2 };
			n.serialize(file);
		}

		file << flush;

		file.seekg(0, ios::beg);
		node n;
		n.unserialize(file);
		n.display();
	}

	void test_page() {
		page<int> a;
	}

	inline void test_entry() {
		test_page();
	}
}
