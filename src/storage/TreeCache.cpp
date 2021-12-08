#include "TreeCache.h"

template <typename T, int TSize>
TreeCache<T, TSize>::TreeCache(int size, std::function<int(int)>& id_to_page_id, std::string filename)
	: Cache(size, id_to_page_id, filename), max_free_node_(default_max_free_node_)
{}

template <typename T, int TSize>
TreeCache<T, TSize>::TreeCache(int size, std::function<int(int)>& id_to_page_id, std::string filename,
	int max_free_node)
	: Cache(size, id_to_page_id, filename), max_free_node_(max_free_node)
{}
