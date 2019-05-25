#ifndef _HTTP_PASER_TEST_H_
#define _HTTP_PASER_TEST_H_


#include <iostream>
#include "http_parser/http_parser.h"
using namespace std;

namespace http_paser_test
{

void test()
{
#ifdef __gnu_linux__
    struct http_parser_url url;
    const char *site = "http://www.github.com/wangjiaming0909";
    http_parser_parse_url(site, strlen(site), 0, &url);
#endif //__gnu_linux__
    cout << 123 << endl;
}

}


#endif // _HTTP_PASER_TEST_H_