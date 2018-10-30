#pragma once
#include <boost\enable_shared_from_this.hpp>

using namespace boost;

class share_from_me : public enable_shared_from_this<share_from_me>{
public:
	friend std::ostream& operator<<(std::ostream& os, const share_from_me& me) {
		os << me.a_;
		return os;
	}
	share_from_me(int a) : a_(a) {}
private:
	int a_;
};