#pragma once
#include <boost\enable_shared_from_this.hpp>
#include <string>

using namespace boost;

class class_without_default_constructor {
public:
	class_without_default_constructor(int a) : a_(a){}
private:
	int a_;
};

class share_from_me : public enable_shared_from_this<share_from_me>{
public:
	friend std::ostream& operator<<(std::ostream& os, const share_from_me& me) {
		os << me.a_;
		return os;
	}
	share_from_me(int a) : a_(a), c_(a){}
	shared_ptr<share_from_me> f() {
		return this->shared_from_this();
	}
private:
	int a_;
	std::string str_;
	class_without_default_constructor c_;
};


class parent {
public:
	parent() { std::cout << "parent" << std::endl; }
	~parent() { std::cout << "~parent" << std::endl; }
};

std::ostream& operator<<(std::ostream& os, const parent& p) {
	os << "parent";
	return os;
}

class son : public parent {
public:
	son() { std::cout << "son" << std::endl; }
	~son() { std::cout << "~son" << std::endl; }
	friend std::ostream& operator<<(std::ostream& os, const son& s) {
		os << "son";
		return os;
	}
};
