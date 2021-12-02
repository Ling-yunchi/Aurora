#pragma once
#include "serializable.h"

class page : public serializable {
	bool dirty_;
public:
	void is_dirty();
};
