#ifndef _HTTP_HEADER_H_
#define _HTTP_HEADER_H_

#include <vector>
#include <string>
#include "http/HttpCommomHeaders.h"
#include "string_piece/string_piece.h"

/*
    HttpHeaders store http headers inside, 
    it have 
        1, header codes vector, 
        2, header names pointers
            if it's commom header, it points to the entry of commom header table
            if it's not the commom headers, dynamic memory is used
        3, header values vector

    how to find the the pos with name or code?
        first get code if you have name,
        then using memchr to find the code in codes_.data(), cause code only have 8 bit
        and the complexity is said to be n/16
 */

namespace http
{
class HttpHeaders
{
    template <typename T>
    using vector_t = std::vector<T>;
    using mutable_string_piece = string_piece::mutable_string_piece;
    using const_string_piece = string_piece::const_string_piece;
public:
    HttpHeaders();
    HttpHeaders(const HttpHeaders& other);
    HttpHeaders(HttpHeaders&& other);
    ~HttpHeaders();
    HttpHeaders& operator=(const HttpHeaders& other);
    HttpHeaders& operator=(HttpHeaders&& other);

public:
    size_t size() const {return codes_.size() - codes_deleted_;}
    void add(const_string_piece headerName, const_string_piece headerValue);
    void add(const std::string& headerName, const std::string& headerValue);
    void add(const std::string& headerName, std::string&& headerValue);
    void add(HttpHeaderCode code, std::string&& headerValue);

    bool remove(const std::string& headerName);
    bool remove(const_string_piece headerName);
    bool remove(HttpHeaderCode code);


private:
    void disposeHeaderNames();
    void clearAll();

private:
    static const size_t INIT_VECTOR_RESERVE_SIZE; 

private:
    vector_t<HttpHeaderCode>            codes_;
    vector_t<const char*>               headerNames_;
    vector_t<std::string>               headerValues_;
    size_t                              codes_deleted_;

#ifdef TESTING
public:
    const vector_t<HttpHeaderCode>& getCodes() const {return codes_;}
    const vector_t<const char*>& getHeaderNames() const {return headerNames_;}
    const vector_t<std::string>& getHeaderValues() const {return headerValues_;}
    size_t getCodesDeleted() const {return codes_deleted_;}

    static size_t getInitVectorReserveSize() {return INIT_VECTOR_RESERVE_SIZE;}
#endif //TESTING
};


}
#endif // _HTTP_HEADER_H_