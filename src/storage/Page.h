#pragma once
#include <iostream>
#include <vector>

#include "serializable.h"

constexpr int page_max_item = 10;

template<typename T, int size>
class page : public serializable<4 + size * page_max_item> {
	int id_;
	bool dirty_;	//不用序列化，读出为false，修改后为true
	int size_;
	std::vector<T> items_;
public:
	bool is_dirty() const;
	page() {

	}
	void serialize(std::ostream& os) override;
	void unserialize(std::istream& is) override;
};
