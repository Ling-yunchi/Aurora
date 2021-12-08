#include "Table.h"
Table::Table(std::string filename) :
	columns_(10),
	index_(filename),
	row_cache_(100, filename) {}

char* Table::serialize() {
	auto buf = new char[this->storage_size_];
	auto tmp_buf = buf;
	serialize_string(name_, tmp_buf);
	serialize_int(column_num_, tmp_buf);
	serialize_vector(columns_, column_num_, tmp_buf);
	return buf;
}

void Table::unserialize(char* buf) {
	unserialize_string(name_, buf);
	unserialize_int(column_num_, buf);
	unserialize_vector(columns_, column_num_, buf);
}
char* Row::serialize() {
	auto buf = new char[this->storage_size_];
	auto tmp_buf = buf;
	serialize_int(size_, tmp_buf);
	serialize_vector(data_, size_, tmp_buf);
	return buf;
}
void Row::unserialize(char* buf) {
	unserialize_int(size_, buf);
	unserialize_vector(data_, size_, buf);
}
