#ifndef _GPERF_TEST_H_
#define _GPERF_TEST_H_

#include <cassert>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <cstdlib>

#include "gperf/HttpCommomHeaders.h"

using namespace std;
namespace gperf
{

void test_all()
{
    const char* fileName = "../src/gperf/headers.txt";
    ifstream ifs{fileName, ifs.in};
    if(!ifs.is_open())
    {
        cout << "can't open file: " << fileName << endl;
        return;
    }

    char* line = static_cast<char*>(::calloc(64, 1));

    int count = 0;
    while(true)
    {
        if(ifs.peek() == EOF) break;

        ifs.getline(line, 64);
        auto ret = Perfect_Hash::isValidHttpHeader(line, strlen(line));
        assert(ret != nullptr);
        assert(strcmp(line, ret->headerName) == 0);
        cout << ret->headerName << " OK..." << endl;
        memset(line, 0, 64);
        count++;
    }
    cout << "count: " << count << endl;
}

void test_gperf()
{
    const char* a1 = "HTTP_HEADER_COLON_AUTHORITY";
    auto ret = Perfect_Hash::isValidHttpHeader(a1, strlen(a1));
    // cout << "header name: " 
    //     << ret->headerName 
    //     << " header code: " 
    //     << static_cast<uint8_t>(ret->headerCode) << endl;
    assert(ret != 0);
    assert(strcmp(ret->headerName, a1) == 0);
    assert(ret->headerCode == HttpHeaderCode::HTTP_HEADER_COLON_AUTHORITY);

    a1 = "";
    ret = Perfect_Hash::isValidHttpHeader(a1, strlen(a1));
    assert(ret == 0);

    a1 = "HTTP_HEADER_CONTENT_ENCODING";
    ret = Perfect_Hash::isValidHttpHeader(a1, strlen(a1));
    assert(ret != 0);
    assert(strcmp(ret->headerName, a1) == 0);
    assert(ret->headerCode == HttpHeaderCode::HTTP_HEADER_CONTENT_ENCODING);

    a1 = "HTTP_HEADER_NONE";
    ret = Perfect_Hash::isValidHttpHeader(a1, strlen(a1));
    assert(ret != 0);
    assert(strcmp(ret->headerName, a1) == 0);
    assert(ret->headerCode == HttpHeaderCode::HTTP_HEADER_NONE);
}

}
#endif // _GPERF_TEST_H_
