#include "http/HttpCommomHeaders.h"

namespace http{

HttpHeaderCode HttpCommomHeaders::getHeaderCode(const char* headerName, size_t len)
{
    const HttpHeader* header = HttpCommomHeaderInternal::isValidHttpHeader(headerName, len);
    return header == nullptr ? HttpHeaderCode::HTTP_HEADER_OTHER : header->headerCode;
}

HttpHeaderCode HttpCommomHeaders::getHeaderCode(const std::string& headerName)
{
    return getHeaderCode(headerName.data(), headerName.length());
}


}//http