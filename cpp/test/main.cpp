﻿#include <iostream>
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <string>
#include <cassert>
#include "shared_from_this_test.h"
#include "my_sp_convetable.h"
#include "util.h"
#include "template.h"

using namespace boost;
using std::string;



template <typename T>
inline void deletion(const T* t, string message) {
	std::cout << *t << " message: " << message << std::endl;
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
	assert(s_ptr.use_count() == 1L);
	assert(w_ptr.expired() == false);
	assert(w_ptr.owner_before(s_ptr) == false);

	//at compile time, we can know that son is the drived class of parent, or compiler will complain: negative subscript
	detail::sp_assert_convertible<share_from_me, enable_shared_from_this<share_from_me>>();
	detail::sp_assert_convertible<son, parent>();
	//typedef char tmp[-1];//negative subscript compile time

	shared_ptr<son> sp_son{ new son() };
	weak_ptr<son> w_ptr_son{ sp_son};
	assert(sp_son.use_count() == 1L);
	weak_ptr<parent> w_ptr_parent{ w_ptr_son };
	assert(w_ptr_parent.lock().get() == w_ptr_son.lock().get());
	//sp_son.reset();//if reset the shared_ptr, all the weak_ptr will points to null
	w_ptr_son.reset(); 
	assert(w_ptr_son.lock() == NULL);//if weak_ptr points to a null, .lock() will return null share_ptr<T>
	shared_ptr<parent> sp_son_from_w_ptr = w_ptr_parent.lock();
	assert(sp_son.use_count() == 2L);
	print(*sp_son_from_w_ptr);
#define reset_of_weak_ptr
	//construct a new shared_ptr, then swap with the weak_ptr
	sp_son_from_w_ptr.reset();//weak_ptr reset
	assert(sp_son.use_count() == 1L);
#define reset_of_shared_ptr
	//when you do reset, shared_ptr only swap the ptr with an empty shared_ptr
	//and the deletion or decrease of the ref_count is done by shared_count
	sp_son.reset();
	assert(sp_son.get() == NULL);
	sp_son.reset(new son(), std::bind(deletion<son>, std::placeholders::_1, "deletion"));
	sp_son.reset();
	print("reset ended...");
	//w_ptr.lock()

	//son o_son;
	//weak_ptr<son> &wk_son = o_son;
}

void test_enable_share_from_this() {
	shared_ptr<share_from_me> p{ new share_from_me(1) };
	shared_ptr<share_from_me> q = p->f();
	assert(p == q);
	assert(p.use_count() == 2L);
}

void test_my_sp_convertable() {
	//compile susccess means that son and parent has the relationship
	char tmp[my_sp_convertable<son, parent>::value ? 1 : -1];
	(void)tmp;
	char tmp2[my_sp_convertable<share_from_me, enable_shared_from_this<share_from_me>>::value ? 1 : -1];
	(void)tmp2;
	//print(typeid(my_sp_convertable<son, parent>::yes).name());
	//char tmp3[my_sp_convertable<int, std::string>::value ? 1 : -1];
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

void test_enable_shared_from_this() {
	//using enable_shared_from_me: 
	//1, first the object must be a heap object not a stack object
	//2, the object must be managed by a shared_ptr first, then invoke the share_from_me(here f) to get the shared_ptr
	//so that the returned the shared_ptr will manage the life cycle of the object, no need to worry that the object will be released some where 
	share_from_me *share_f_me = new share_from_me(1);
	shared_ptr<share_from_me> sp_me{ share_f_me };
	shared_ptr<share_from_me> me_ptr = share_f_me->f();
	assert(me_ptr.use_count() == 2L);
	//now we can reset the origin shared_ptr sp_me
	sp_me.reset();
	assert(me_ptr.use_count() == 1L);
	me_ptr.reset();
	assert(me_ptr.use_count() == 0L);
	//assert(share_f_me == NULL);
}

void test_scoped_enum() {
	enum color{red, green};
	enum class scoped_color{green, red};
	print(red);
	print(green);
	scoped_color g = scoped_color::green;
	scoped_color r = scoped_color::red;

}

class complete_class {
public:
	virtual void print() = 0;
	int a;
};

class incomlete_class;

class test_reference {
public:
	test_reference (string& str);
	~test_reference ();

private:
	string & str_;
};

test_reference ::test_reference (string& str) : str_(str) {

}

test_reference ::~test_reference ()
{
}

long getsize();
const int a = 5;
const int* const int_ptr = &a;
void test_decltype() {
	//getsize didn't defined yet, but it compiles well, and the type of s is long
	decltype(getsize()) s;
	//b is const int, you must initialize it when define it 
	//you can't change the value, because it's a const int
	decltype(a) b = 12;
	//int_ptr has 顶层const, 并且顶层const被保存了
	//so, you have to initialize the int_ptr2 when defined it
	//decltype(int_ptr) int_ptr2;
	decltype(int_ptr) int_ptr2 = 0;
	//底层const也被保存了, 不可以改变指针指向的地址处的值
	//*int_ptr = 2;
	int i = 0;
	int& ref_int = i;
	//int&
	decltype(ref_int) dec_ref_int = i;
	//not a const int&
	//decltype(ref_int) dec_ref_int2 = 0;
	const int& const_ref_int = 0;

	//两个括号，还是引用
	//int&
	decltype((i)) ref_ref_int = i;
	const int& c_ref_int = i;
	//const int&
	decltype(c_ref_int) c_ref_int2 = 0;
	const int c_int = 2;
	//const int&
	decltype((c_int)) c_int2 = 0;
	//const int&
	decltype((c_ref_int)) c_ref_int3 = 0;

	int i_int2 = 10;
	int* int_ptr = &i_int2;
	//解引用返回引用类型
	decltype(*int_ptr) ref_int3 = i_int2;
	//const int*
	const int* const const_int_ptr = &i_int2;
	//解引用返回const int&, 顶层const加不加都一样
	decltype(*const_int_ptr) const_ref_int3 = i_int2;
}

class test_const_ref {
public:
	const int& getdata() const { return data; }
private:
	int data;
};

int main() {
	//template_test::test_template();
	//print(sizeof(complete_class));
	//sizeof(incomlete_class);

	//test_scoped_enum();
	//test_enable_shared_from_this();
	//test_weak_ptr();
}