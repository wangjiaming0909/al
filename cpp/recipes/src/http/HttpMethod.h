#ifndef _HTTP_METHOD_H_
#define _HTTP_METHOD_H_
#include "string_piece/string_piece.h"
#include "boost/optional.hpp"

namespace http
{
#define HTTP_METHOD_GEN(x) \
  x(GET),                  \
  x(POST),                 \
  x(OPTIONS),              \
  x(DELETE),               \
  x(HEAD),                 \
  x(CONNECT),              \
  x(PUT),                  \
  x(TRACE),                \
  x(PATCH),                \
  x(SUB),                  \
  x(PUB),                  \
  x(UNSUB)

#define HTTP_METHOD_ENUM(method) method

enum class HTTPMethod
{
    HTTP_METHOD_GEN(HTTP_METHOD_ENUM)
};

boost::optional<HTTPMethod> stringToMethod(string_piece::const_string_piece methodName);

}
#endif // _HTTP_METHOD_H_