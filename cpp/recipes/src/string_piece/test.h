#ifndef _STRING_PIECE_TEST_H_
#define _STRING_PIECE_TEST_H_

#include "string_piece/string_piece.h"
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

namespace string_piece_test
{

void test_sub_string()
{
    std::string str = "1234567890";
    string_piece::const_string_piece sp{str};

    auto subString = sp.sub_string(0, 10);
    assert(subString.begin() == &*str.begin());
    assert(subString.end() == &*str.end());
    assert(subString.size() == str.length());
}

void test_sub_string2()
{
    const char* data = "123456789";
    char* ptr = static_cast<char*>(::calloc(10, 1));
    ::memcpy(ptr, data, 6);
    string_piece::mutable_string_piece str2{ptr, ptr + 5};

    auto subString = str2.sub_string(0, 5);
    assert(subString.begin() == ptr);
    assert(subString.end() == ptr + 5);

    ::free(ptr);
}

void test_find()
{
    string_piece::const_string_piece text = "https://github.com/wangjiaming0909";
    string_piece::const_string_piece pattern = "://";
    auto pos = text.find(pattern);
    cout << "pos: " << pos << endl;
    assert(pos == 5);
    string_piece::const_string_piece pattern1 = "https:";
    pos = text.find(pattern1);
    assert(pos == 0);
}

void test()
{
    string_piece::mutable_string_piece str{};
    const char* data = "123456789";
    char* ptr = static_cast<char*>(::calloc(10, 1));
    ::memcpy(ptr, data, 6);
    string_piece::mutable_string_piece str2{ptr, ptr + 5};

    auto index = str2.find('2');
    cout << "index: " << index << endl;
    index = str2.find('-');
    cout << "index: " << index << endl;

    std::string strstr = "123456";
    //mutable_string_piece do not have this constructor
    // string_piece::mutable_string_piece str3{strstr};

    string_piece::const_string_piece str4{strstr};
    string_piece::mutable_string_piece str5{ptr};

    ::free(ptr);

    // std::vector<int> v{1,2,3};
    // string_piece::Range<int *> int1{&*v.begin()};

    cout << 123 << endl;

    test_sub_string();
    test_sub_string2();
    test_find();
}

}

#endif // _STRING_PIECE_TEST_H_