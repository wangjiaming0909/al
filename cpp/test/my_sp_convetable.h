#pragma once

template <typename son, typename parent>
class my_sp_convertable {
public:
	typedef char(&yes)[1];//yes is a type of reference of char[1]
	typedef char(&no)[2];
	static yes f(parent*);
	static no f(...);//overload any other parameters
	//there is no function call here for f
	//only get the size of the return type in compile time
	//sizeof is a compile time operator
	//so we donot need a definition of function f
	enum _vt { value = sizeof(f(static_cast<son*>(0))) == sizeof(yes)};
};

class assert_sp_convertable {

};