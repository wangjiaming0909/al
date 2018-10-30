#include <iostream>
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <string>
#include <cassert>
#include "shared_from_this_test.h"

using namespace boost;

template <typename T>
inline void print(const T &t) {
	std::cout << t << std::endl;
}

int main() {
	share_from_me me{1};
	auto ptr = me.shared_from_this();
	//weak_ptr<share_from_me> wptr = me.weak_from_this();
	print(*ptr);
	print(ptr.use_count());
}

void test_shared_ptr() {
	shared_ptr<std::string> str_ptr{ new std::string("123") };
	shared_ptr<std::_String_alloc<std::_String_base_types<char, std::allocator<char>>>> str2_ptr = str_ptr;
	assert(str_ptr.get() == str2_ptr.get());
	assert(!str_ptr.unique());
	assert(str_ptr.use_count() == 2l);
	shared_ptr<std::string> str_ptr_swap{ new std::string("456") };
	str_ptr.swap(str_ptr_swap);
	assert(str_ptr.get() != str2_ptr.get());
	assert(str2_ptr.use_count() == 2l);
	assert(str_ptr.use_count() == 1l);
	assert(str_ptr_swap.get() == str2_ptr.get());
	print(str_ptr);
	print(*str_ptr);
	print(*str_ptr_swap);
}