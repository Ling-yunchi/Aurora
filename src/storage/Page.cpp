#include "Page.h"


template <typename T, int size>
bool page<T, size>::is_dirty() const {
	return dirty_;
}

template <typename T, int size>
char* page<T, size>::serialize() {}

template <typename T, int size>
void page<T, size>::unserialize(char* buffer) {}
