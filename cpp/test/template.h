#pragma once
#include "util.h"

namespace template_test {

template <unsigned N, typename T>
unsigned array_size(const T(&arr)[N])
{
	return N;
}

//ptr must be const expression, 
//but for a pointer, the initialization of a const expression must be either nullptr or 0
//no one will use this right?
template <char* ptr>
char dereference(char* p) {
	return *p + *ptr;
}

static char* static_ptr = "asd";
constexpr char* const_ptr = "asd";
const char*const c_ptr_c = "asd";
void test_array_size() {
	long l_arr[10];
	int i_arr[100];
	print(array_size(l_arr));
	print(array_size(i_arr));
	const int a = 12;
	const int* a_ptr = &a;
	char* c_ptr = "ab";
	int b = dereference<nullptr>(c_ptr);
	print(b);
}




void test_template() {
	test_array_size();
}
}
