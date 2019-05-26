#ifndef _HTTP_PASER_TEST_H_
#define _HTTP_PASER_TEST_H_


#include <iostream>
#include "http_parser/http_parser.h"
#include "boost/range.hpp"
#include <vector>

using namespace std;

namespace http_parser_test
{

class NotDefaultConstructableClass
{
public:
    NotDefaultConstructableClass(int _){}
};

class NOTDestructable
{
public:
    ~NOTDestructable() = delete;
};


void test_traits()
{
    std::vector<string> vstr{};
    //when construct vector, it didn't check if T has default constructor
    std::vector<NotDefaultConstructableClass> v{};
    // v.resize(12);

    cout << std::is_trivially_default_constructible<NotDefaultConstructableClass>::value << endl;
    using const_char = std::add_const<char*>::type;
    const_char c = "asd";
    cout << c << endl;

    cout << typeid(std::remove_const_t<const_char>).name() << endl;

    cout << std::is_destructible<NotDefaultConstructableClass>::value << endl;
    cout << std::is_destructible<NOTDestructable>::value << endl;
    cout << std::is_destructible<void>::value << endl;
    //为什么const char*也是destructible的???
    cout << std::is_destructible<const_char>::value << endl;
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