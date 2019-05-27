#ifndef _STRING_PIECE_TEST_H_
#define _STRING_PIECE_TEST_H_

#include "string_piece/string_piece.h"
#include <iostream>
#include <vector>
using namespace std;

namespace string_piece_test
{

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
    string_piece::mutable_string_piece str3{strstr};

    string_piece::const_string_piece str4{strstr};
    string_piece::mutable_string_piece str5{ptr};





    // std::vector<int> v{1,2,3};
    // string_piece::Range<int *> int1{&*v.begin()};

    cout << 123 << endl;
}

}

#endif // _STRING_PIECE_TEST_H_