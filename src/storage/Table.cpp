#include "Table.h"

#include <utility>

Table::Table(std::string name, int column_num, std::vector<std::string>& columns) :
	name_(std::move(name)),
	column_num_(column_num),
	columns_(columns) {

}

Table::Table() :columns_(10) {}

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

Row::Row() :data_(10) {}
Row::Row(int size) : size_(size), data_(size) {}

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
