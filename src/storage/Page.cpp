#include "Page.h"

//template <typename T, int TSize>
//bool page<T, TSize>::is_dirty() const { return dirty_; }
//
//template <typename T, int TSize>
//page<T, TSize>::page() :id_(-1) {
//	items_.resize(page_max_item);
//}
//
//template <typename T, int TSize>
//page<T, TSize>::page(int id) : id_(id) {
//	items_.resize(page_max_item);
//}
//
//template <typename T, int TSize>
//int page<T, TSize>::get_id() const { return id_; }
//
//template <typename T, int TSize>
//T& page<T, TSize>::get_item(int idx) {
//	return items_[idx];
//}
//
//template <typename T, int TSize>
//void page<T, TSize>::set_item(int idx, T& item) {
//	items_[idx] = item;
//	dirty_ = true;
//}
//
//template <typename T, int TSize>
//char* page<T, TSize>::serialize() {
//	auto buf = new char[TSize];
//	auto tmp_buf = buf;
//	serializable<TSize>::serialize_int(id_, tmp_buf);
//	serializable<TSize>::serialize_int(size_, tmp_buf);
//	serializable<TSize>::serialize_vector(items_, size_, tmp_buf);
//	return buf;
//}
//
//template <typename T, int TSize>
//void page<T, TSize>::unserialize(char* buffer) {
//	dirty_ = false;
//	serializable<TSize>::unserialize(id_, buffer);
//	serializable<TSize>::unserialize(size_, buffer);
//	serializable<TSize>::unserialize_vector(items_, size_, buffer);
//}
