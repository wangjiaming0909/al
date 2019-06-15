#include "http/HttpHeaders.h"

namespace http{

HttpHeaders::HttpHeaders()
    : codes_{}
    , headerNames_{}
    , headerValues_{}
    , codes_deleted_{0}
{
    codes_.reserve(INIT_VECTOR_RESERVE_SIZE);
    headerNames_.reserve(INIT_VECTOR_RESERVE_SIZE);
    headerValues_.reserve(INIT_VECTOR_RESERVE_SIZE);
}

HttpHeaders::HttpHeaders(const HttpHeaders& other)
    : codes_{other.codes_}
    , headerNames_{other.headerNames_}
    , headerValues_{other.headerValues_}
    , codes_deleted_{other.codes_deleted_}
{
    for(size_t i = 0; i < codes_.size(); i++)
    {
        size_t len = strlen(other.headerNames_[i]);
        char* p = static_cast<char*>(::calloc(len + 1, 1));
        ::memcpy(p, other.headerNames_[i], len);
        if(codes_[i] == HttpHeaderCode::HTTP_HEADER_OTHER)
            headerNames_[i] = p;
    }
}

HttpHeaders::HttpHeaders(HttpHeaders&& other)
    : codes_{std::move(other.codes_)}
    , headerNames_{std::move(other.headerNames_)}
    , headerValues_{std::move(other.headerValues_)}
    , codes_deleted_{other.codes_deleted_}
{
    other.clearAll();
}

HttpHeaders& HttpHeaders::operator=(const HttpHeaders& other)
{

}

HttpHeaders& HttpHeaders::operator=(HttpHeaders&& other)
{

}

HttpHeaders::~HttpHeaders()
{
    disposeHeaderNames();
}

void HttpHeaders::add(const_string_piece headerName, const_string_piece headerValue)
{

}

void HttpHeaders::add(const std::string& headerName, const std::string& headerValue)
{

}

void HttpHeaders::add(const std::string& headerName, std::string&& headerValue)
{

}

void HttpHeaders::add(HttpHeaderCode code, std::string&& headerValue)
{

}

bool HttpHeaders::remove(const std::string& headerName)
{

}

bool HttpHeaders::remove(const_string_piece headerName)
{

}

bool HttpHeaders::remove(HttpHeaderCode code)
{

}

void HttpHeaders::clearAll()
{
    disposeHeaderNames();

    codes_.clear();
    headerNames_.clear();
    headerValues_.clear();
    codes_deleted_ = 0;
}

void HttpHeaders::disposeHeaderNames()
{
    for(size_t i = 0; i < codes_.size(); i++)
    {
        if(HttpHeaderCode::HTTP_HEADER_OTHER == codes_[i])
            free(const_cast<void*>(static_cast<const void*>(headerNames_[i])));
    }
}

const size_t HttpHeaders::INIT_VECTOR_RESERVE_SIZE = 16;

}