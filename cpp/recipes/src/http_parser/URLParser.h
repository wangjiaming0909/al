#ifndef _HTTP_PARSER_H_
#define _HTTP_PARSER_H_

#include <cstdint>

namespace http
{

class URLParser
{
public:
    using const_string_piece_t = const char*;
    using string_piece_t = char*;
public:
    URLParser(){}
    explicit URLParser(const_string_piece_t urlVal) noexcept
    {
        init(urlVal);
    }
    ~URLParser();

    void init(const_string_piece_t urlVal);

    const_string_piece_t url() const { return url_; }
    const_string_piece_t scheme() const { return scheme_; }
    const_string_piece_t host() const {return host_;}
    uint16_t port() const {return port_;}
    const_string_piece_t path() const {return path_;}
    const_string_piece_t query() const {return query_;}
    bool valid() const {return valid_;}

private:
    string_piece_t      url_;
    string_piece_t      scheme_;
    string_piece_t      host_;
    string_piece_t      path_;
    string_piece_t      query_;
    uint16_t            port_{0};
    bool                valid_{false};
    bool                initilized_{false};
};


}

#endif //_HTTP_PARSER_H_