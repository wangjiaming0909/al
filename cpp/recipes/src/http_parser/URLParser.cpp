#include "http_parser/URLParser.h"
#include <algorithm>
#include <string>
#include "boost/range.hpp"

namespace http
{

URLParser::~URLParser()
{
}

void URLParser::init(const_string_piece_t urlVal)
{
    std::find(urlVal, urlVal + 10, "://");
}


}
