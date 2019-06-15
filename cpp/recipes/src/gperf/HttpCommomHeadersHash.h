#ifndef _HTTP_COMMOM_HEADERS_HASH_H_
#define _HTTP_COMMOM_HEADERS_HASH_H_
#include <cstdint>
#include <cstring>
#include "HttpHeaderCode.h"

namespace http
{
struct HttpHeader
{
    const char* headerName;
    HttpHeaderCode headerCode;
};

#define TOTAL_KEYWORDS 93
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 37
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 148
/* maximum key range = 146, duplicates = 0 */

class HttpCommomHeaderInternal
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static struct HttpHeader *isValidHttpHeader (const char *str, size_t len);
};
}

#endif //_HTTP_COMMOM_HEADERS_HASH_H_