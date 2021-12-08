#pragma once
#include "Cache.h"

constexpr int default_max_free_node_ = 100;

template<typename T, int TSize>
class TreeCache : public Cache<T, TSize>
{
	int max_free_node_;
	std::vector<int> free_node_;

	TreeCache(int size, std::function<int(int)>& id_to_page_id, std::string filename);
	TreeCache(int size, std::function<int(int)>& id_to_page_id, std::string filename, int max_free_node);

	friend Cache<T, TSize>;
};



