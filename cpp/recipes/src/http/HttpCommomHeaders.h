#ifndef _HTTP_COMMOM_HEADER_H_
#define _HTTP_COMMOM_HEADER_H_
#include "gperf/HttpCommomHeadersHash.h"

#include <string>

namespace http
{
class HttpCommomHeaders
{
public:
    static HttpHeaderCode getHeaderCode(const char* headerName, size_t len);

    static HttpHeaderCode getHeaderCode(const std::string& headerName);
};

}
#endif // _HTTP_COMMOM_HEADER_H_