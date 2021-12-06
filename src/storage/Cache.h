#pragma once
#include <functional>

#include "Page.h"
#include <map>
#include <queue>

template<typename T, int TSize>
class Cache
{
	int max_page_num_;
	std::function<int(int)> id_to_page_id_;
	std::queue<int> page_keys_;
	std::map<int, page<T, TSize>> page_;
	Cache(int size, std::function<int(int)> id_to_page_id);

};


