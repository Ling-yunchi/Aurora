#include "Cache.h"


template <typename T, int TSize>
Cache<T, TSize>::Cache(int size, std::function<int(int)> id_to_page_id) :max_page_num_(size), id_to_page_id_(id_to_page_id) {}