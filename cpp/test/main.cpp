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

void void_void(class_without_default_constructor* p) {
	
}

void test_weak_ptr() {
	//int* p = new int(12);
	//void_void(p);
	shared_ptr<class_without_default_constructor> s_ptr{ new class_without_default_constructor(1) };
	weak_ptr<class_without_default_constructor> w_ptr{ s_ptr };

	//sp_element<T[]>, although the template param of weak_ptr is class_without_default_constructor[],
	//but the element type of it is class_without_default_constructor
	shared_ptr<class_without_default_constructor[]> s_ptr_arr{ new class_without_default_constructor[3]{1,2,3} };//array pointer type
	weak_ptr<class_without_default_constructor[]> w_ptr_arr{ s_ptr_arr };
	//weak_ptr<class_without_default_constructor[]>::element_type element{ 1 };//its class_without_default_constructor type
	assert(typeid(weak_ptr<class_without_default_constructor[]>::element_type) == typeid(class_without_default_constructor));

	//at compile time, we can know that son is the drived class of parent, or compiler will complain: negative subscript
	detail::sp_assert_convertible<share_from_me, enable_shared_from_this<share_from_me>>();
	detail::sp_assert_convertible<son, parent>();
	//typedef char tmp[-1];//negative subscript compile time

	shared_ptr<son> sp_son{ new son() };
	weak_ptr<son> w_ptr_son{ sp_son};
	assert(sp_son.use_count() == 1L);
	weak_ptr<parent> w_ptr_parent{ w_ptr_son };

	assert(s_ptr.use_count() == 1L);
	assert(w_ptr.expired() == false);
	assert(w_ptr.owner_before(s_ptr) == false);
}

void test_enable_share_from_this() {
	shared_ptr<share_from_me> p{ new share_from_me(1) };
	shared_ptr<share_from_me> q = p->f();
	assert(p == q);
	assert(p.use_count() == 2L);
}

int main() {
	test_weak_ptr();
}

void test_shared_ptr() {
	shared_ptr<std::string> str_ptr{ new std::string("123") };
	shared_ptr<std::_String_alloc<std::_String_base_types<char, std::allocator<char>>>> str2_ptr = str_ptr;
	assert(str_ptr.get() == str2_ptr.get());
	assert(!str_ptr.unique());
	assert(str_ptr.use_count() == 2L);
	shared_ptr<std::string> str_ptr_swap{ new std::string("456") };
	str_ptr.swap(str_ptr_swap);
	assert(str_ptr.get() != str2_ptr.get());
	assert(str2_ptr.use_count() == 2L);
	assert(str_ptr.use_count() == 1L);
	assert(str_ptr_swap.get() == str2_ptr.get());
	print(str_ptr);
	print(*str_ptr);
	print(*str_ptr_swap);
}