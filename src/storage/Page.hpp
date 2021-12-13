#pragma once
#include <iostream>
#include <vector>

#include "serializable.h"

constexpr int page_max_item = 100;

template<typename T, int TSize>
class page : public serializable<8 + TSize * page_max_item> {
	int id_;
	bool dirty_;	//不用序列化，读出为false，修改后为true
	int size_;
	std::vector<T> items_;
public:
	void dirty();
	bool is_dirty() const;
	page();
	explicit page(int id);
	int get_id() const;
	T& get_item(int id);
	void set_item(int idx, T& item);
	char* serialize() override;
	void unserialize(char* buffer) override;
};

template <typename T, int TSize>
void page<T, TSize>::dirty() {
	dirty_ = true;
}

template <typename T, int TSize>
bool page<T, TSize>::is_dirty() const { return dirty_; }

template <typename T, int TSize>
page<T, TSize>::page() :dirty_(false), id_(-1), size_(0) {
	items_.resize(page_max_item);
}

template <typename T, int TSize>
page<T, TSize>::page(int id) : id_(id), size_(0), dirty_(false) {
	items_.resize(page_max_item);
}

template <typename T, int TSize>
int page<T, TSize>::get_id() const { return id_; }

template <typename T, int TSize>
T& page<T, TSize>::get_item(int idx) {
	return items_[idx];
}

template <typename T, int TSize>
void page<T, TSize>::set_item(int idx, T& item) {
	items_[idx] = item;
	dirty_ = true;
	if (idx >= size_)
		size_ = idx + 1;
}

template <typename T, int TSize>
char* page<T, TSize>::serialize() {
	auto buf = new char[this->storage_size_];
	auto tmp_buf = buf;
	serializable<TSize>::serialize_int(id_, tmp_buf);
	serializable<TSize>::serialize_int(size_, tmp_buf);
	serializable<TSize>::serialize_vector(items_, size_, tmp_buf);
	return buf;
}

template <typename T, int TSize>
void page<T, TSize>::unserialize(char* buffer) {
	dirty_ = false;
	serializable<TSize>::unserialize_int(id_, buffer);
	serializable<TSize>::unserialize_int(size_, buffer);
	serializable<TSize>::unserialize_vector(items_, size_, buffer);
}
