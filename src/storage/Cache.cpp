#include "Cache.h"

//#include "../log/Logger.h"
//
//template <typename T, int TSize>
//Cache<T, TSize>::Cache(int max_page_num, std::string filename) :
//	max_page_num_(max_page_num)
//{
//	disk.open(filename, std::ios::binary | std::ios::in | std::ios::out);
//	if (!disk.is_open()) {
//		logger.error("cache init failed: file open fail");
//	}
//
//	char buf[4];
//	disk.read(buf, 4);
//	serializable::unserialize_int(page_storage_size_, buf);
//
//	//��ȡҳ�Ĵ洢��С
//	page<T, TSize> tmp;
//	page_storage_size_ = tmp.get_storage_size();
//
//	//ò�Ʋ���Ԥ����ռ䣬ֱ��д����qwq
//	//page<T, TSize> p;
//	//disk.seekp(default_pre_allocate_file_size * p.get_storage_size());
//	//disk.write("\0", 1);
//}
//
//template <typename T, int TSize>
//T& Cache<T, TSize>::get_item(int item_id) {
//	int page_id = id_to_page_id(item_id);
//	if (!pages_.count(page_id))
//		read_page_from_disk(page_id);
//	else {
//		//���Ѿ��ڻ����е�page��̭���ȶȽ������
//		page_keys_.remove(page_id);
//		page_keys_.emplace_front(page_id);
//	}
//	int item_idx = id_to_item_idx(item_id);
//	return pages_.at(page_id).get_item(item_idx);
//}
//
//template <typename T, int TSize>
//void Cache<T, TSize>::insert_item(int item_id, const T& item) {
//	int page_id = id_to_page_id(item_id);
//	if (page_id < disk_page_size_) {
//		//д����ڵ�page
//		if (!pages_.count(page_id))
//			read_page_from_disk(page_id);
//		pages_.at(page_id).set_item(id_to_item_idx(item_id), item);
//	}
//	else {
//		//������page
//		page<T, TSize> new_page(disk_page_size_);
//
//		new_page.set_item(id_to_item_idx(item_id), item);
//
//		page_keys_.emplace_front(new_page.get_id());
//		pages_.insert(std::make_pair(new_page.get_id(), new_page));
//
//		disk_page_size_++;
//
//		if (pages_.size() > max_page_num_)
//			obsolescence();
//	}
//}
//
//template <typename T, int TSize>
//void Cache<T, TSize>::flush() {
//	disk.seekp(0, std::ios::beg);
//	char buf[4];
//	*reinterpret_cast<int*>(buf) = disk_page_size_;
//	disk.write(buf, 4);
//
//	while (!pages_.empty()) {
//		obsolescence();
//	}
//}
//
//template <typename T, int TSize>
//void Cache<T, TSize>::read_page_from_disk(int page_id) {
//	if (page_id >= disk_page_size_) {
//		logger.error("read unwritten page!");
//		return;
//	}
//
//	auto buf = new char[page_storage_size_];
//	disk.seekg(page_id_to_disk_idx(page_id));
//	disk.read(buf, page_storage_size_);
//	page<T, TSize> tmp;
//	tmp.unserialize(buf);
//
//	page_keys_.emplace_front(page_id);
//	pages_.insert(std::make_pair(page_id, tmp));
//	logger.info("add page " + std::to_string(page_id) + " to cache");
//
//	//������󻺴�������̭
//	if (pages_.size() > max_page_num_)
//		obsolescence();
//}
//
//template <typename T, int TSize>
//void Cache<T, TSize>::write_page_to_disk(page<T, TSize>& page) {
//	if (page.get_id() > page_storage_size_) {
//		logger.info("write a new page " + std::to_string(page.get_id()));
//		disk_page_size_++;
//	}
//	else
//		logger.info("rewrite a exist page " + std::to_string(page.get_id()));
//
//	auto buf = page.serialize();
//	disk.seekp(page_id_to_disk_idx(page.get_id()));
//	disk.write(buf, page_storage_size_);
//}
//
//template <typename T, int TSize>
//void Cache<T, TSize>::obsolescence() {
//	//�Ƴ���̭���޶���ߵ�page������������page
//	auto die_page_id = page_keys_.back();
//	auto& die_page = pages_.at(die_page_id);
//
//	//���ڲ����ݱ��޸ģ�д�����
//	if (die_page.is_dirty()) {
//		write_page_to_disk(die_page);
//	}
//
//	pages_.erase(die_page_id);
//	page_keys_.pop_back();
//	logger.info("delete page " + std::to_string(die_page_id) + " from cache");
//}
//
//template <typename T, int TSize>
//int Cache<T, TSize>::id_to_page_id(int item_id) {
//	return item_id / page_max_item;
//}
//
//template <typename T, int TSize>
//int Cache<T, TSize>::id_to_item_idx(int item_id) {
//	return item_id % page_max_item;
//}
//
//template <typename T, int TSize>
//int Cache<T, TSize>::page_id_to_disk_idx(int page_id) const {
//	return 4 + page_id * page_storage_size_;
//}
