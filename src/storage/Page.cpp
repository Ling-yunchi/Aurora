#include "Page.h"


template <typename T, int size>
bool page<T, size>::is_dirty() const {
	return dirty_;
}

template <typename T, int size>
void page<T, size>::serialize(std::ostream& os) {
	auto buffer = new char[this->storage_size_];

}

template <typename T, int size>
void page<T, size>::unserialize(std::istream& is) {

}
