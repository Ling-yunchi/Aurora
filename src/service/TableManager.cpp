//#include "TableManager.h"
//#include "../storage/serializable.h"
//TableManager::TableManager() {
//	if (_access(sys_path.c_str(), 0)) {
//		std::ofstream of(sys_path);
//		char buf[4];
//		*reinterpret_cast<int*>(buf) = 0;
//		of.write(buf, 4);
//	}
//
//	std::ifstream ifs(sys_path);
//
//	auto buf = new char[storage_size_];
//	ifs.read(buf, storage_size_);
//	serializable<0>::unserialize_int(table_num_, buf);
//	serializable<0>::unserialize_vector(tables_, table_num_, buf);
//	delete[] buf;
//
//	ifs.close();
//}
//
//TableManager::~TableManager() {
//	delete table_;
//	delete index_;
//	delete data_cache_;
//
//	std::ofstream ofs(sys_path);
//
//	auto buf = new char[storage_size_];
//	auto tmp_buf = buf;
//	serializable<0>::serialize_int(table_num_, tmp_buf);
//	serializable<0>::serialize_vector(tables_, table_num_, tmp_buf);
//	ofs.write(buf, storage_size_);
//	delete[] buf;
//
//	ofs.close();
//}
//
//void TableManager::show_tables() {
//	for (int i = 0; i < table_num_; i++)
//		std::cout << tables_[i] << std::endl;
//}
//
//void TableManager::add_table(std::string name, int column_num, std::vector<std::string>& columns) {
//	tables_.push_back(name);
//
//	Table table(name, column_num, columns);
//	std::fstream fs;
//	fs.open(data_path + name + "_i.dat", std::ios::in | std::ios::out | std::ios::binary);
//	bptree::SBPTree idx(name + "_idx.dat");
//	auto buf = idx.serialize();
//	fs.write(buf, idx.get_storage_size());
//	fs.close();
//	delete[] buf;
//
//	Cache<Row, 2554> data_cache_(100, name + "_data.dat");
//}
//
//const Table* TableManager::get_now_table() {
//	return table_;
//}
//
//
//void TableManager::choose_table(std::string table_name) {
//	if (table_name == now_table_) return;
//	now_table_ = table_name;
//
//	std::fstream fs;
//	fs.open(data_path + table_name + "_i.dat", std::ios::in | std::ios::out | std::ios::binary);
//	auto buf = index_->serialize();
//	fs.write(buf, index_->get_storage_size());
//	fs.close();
//	delete[] buf;
//
//	delete table_;
//	delete index_;
//	delete data_cache_;
//
//	table_ = new Table();
//	index_ = new bptree::SBPTree(table_name + "_idx.dat");
//	data_cache_ = new Cache<Row, 2554>(100, table_name + "_idx.dat");
//	fs.open(data_path + table_name + "_i.dat", std::ios::in | std::ios::out | std::ios::binary);
//	buf = new char[index_->get_storage_size()];
//	fs.read(buf, index_->get_storage_size());
//	index_->unserialize(buf);
//	delete[] buf;
//
//	std::ifstream ifs(table_name + ".dat");
//	buf = new char[table_->get_storage_size()];
//	ifs.read(buf, table_->get_storage_size());
//	table_->unserialize(buf);
//	delete[] buf;
//	ifs.close();
//}
//
//std::vector<std::string>& TableManager::get_columns() {
//	return table_->columns_;
//}
//
//Row& TableManager::search_table(int key) {
//	auto data_id = index_->search(key);
//	return data_cache_->get_item(data_id);
//}
//
//void TableManager::insert_table(Row& row) {
//	data_cache_->insert_item(table_->auto_increase_id, row);
//	index_->insert(table_->auto_increase_id, table_->auto_increase_id);
//	table_->auto_increase_id++;
//}
//
//void TableManager::update_table(int key, Row& row) {
//	data_cache_->insert_item(key, row);
//	data_cache_->set_dirty(key);
//}
//
//void TableManager::delete_table(int key) {
//	index_->remove(key);
//}
//
//bool TableManager::has_table(std::string table_name) {
//	for (int i = 0; i < table_num_; i++) {
//		if (table_name == tables_[i])
//			return true;
//	}
//	return false;
//}
