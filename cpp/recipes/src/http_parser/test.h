#ifndef _HTTP_PASER_TEST_H_
#define _HTTP_PASER_TEST_H_


#include <iostream>
#include "http_parser/http_parser.h"
#include "boost/range.hpp"

using namespace std;

namespace http_parser_test
{

class NotDefaultConstructableClass
{
public:
    NotDefaultConstructableClass(int _){}
};

void test_traits()
{
    cout << std::is_trivially_default_constructible<NotDefaultConstructableClass>::value << endl;
    using const_char = std::add_const<char*>::type;
    const_char c = "asd";
    cout << c << endl;

    std::remove_const_t<const_char> p = "a";
    cout << p << endl;
}


void test()
{
#ifdef __gnu_linux__
    struct http_parser_url url;
    const char *site = "http://www.github.com/wangjiaming0909";
    http_parser_parse_url(site, strlen(site), 0, &url);

    using it_traits = std::iterator_traits<const char*>;
    it_traits::pointer p = "asd";
    it_traits::difference_type difference = 1;
    cout << typeid(it_traits::iterator_category).name() << endl;
    cout << typeid(it_traits::reference).name() << endl;
    cout << typeid(it_traits::value_type).name() << endl;
    cout << difference << endl;
    cout << p << endl;

    // boost::range_iterator<char> string_piece;
    // boost::range_result_iterator<char>
#endif //__gnu_linux__
    cout << "testing http_parser" << endl;
}

}


#endif // _HTTP_PASER_TEST_H_