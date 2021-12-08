#pragma once
#include "Cache.hpp"

constexpr int default_max_free_node_ = 100;

template<typename T, int TSize>
class TreeCache : public Cache<T, TSize>
{
	int max_free_node_;
	std::vector<int> free_node_;

	TreeCache(int size, std::string filename, int max_free_node);
	bool has_free_node();
	void add_free_node(int i);
	int get_free_node();

	int page_id_to_disk_idx(int page_id) const;

	friend Cache<T, TSize>;
};

template <typename T, int TSize>
TreeCache<T, TSize>::TreeCache(int size, std::string filename, int max_free_node)
	:Cache(size, filename), max_free_node_(max_free_node) {}



