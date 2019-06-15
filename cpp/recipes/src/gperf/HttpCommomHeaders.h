/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf -DG -K headerName -N isValidHttpHeader -L C++ -t HttpCommomHeaders.h.gperf  */
/* Computed positions: -k'13,20,34,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "HttpCommomHeaders.h.gperf"

#include <cstdint>
#include <cstring>
#include "HttpHeaderCode.h"
using HttpHeaderCode = http::HttpHeaderCode;
#line 7 "HttpCommomHeaders.h.gperf"
struct HttpHeader
{
    const char* headerName;
    HttpHeaderCode headerCode;
};

#define TOTAL_KEYWORDS 95
#define MIN_WORD_LENGTH 14
#define MAX_WORD_LENGTH 49
#define MIN_HASH_VALUE 19
#define MAX_HASH_VALUE 200
/* maximum key range = 182, duplicates = 0 */

class Perfect_Hash
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static struct HttpHeader *isValidHttpHeader (const char *str, size_t len);
};

inline unsigned int
Perfect_Hash::hash (const char *str, size_t len)
{
  static unsigned char asso_values[] =
    {
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201,  15, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201,   0, 201,   0,  55,  15,
       70,  45,  25,  55, 201, 100,  65,  55,   5,  75,
       25, 201,  45,  15,   5, 100,   5,  35,  15,  20,
        5, 201, 201, 201, 201,  15, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201, 201, 201, 201, 201,
      201, 201, 201, 201, 201, 201
    };
  unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[static_cast<unsigned char>(str[33])];
      /*FALLTHROUGH*/
      case 33:
      case 32:
      case 31:
      case 30:
      case 29:
      case 28:
      case 27:
      case 26:
      case 25:
      case 24:
      case 23:
      case 22:
      case 21:
      case 20:
        hval += asso_values[static_cast<unsigned char>(str[19])];
      /*FALLTHROUGH*/
      case 19:
      case 18:
      case 17:
      case 16:
      case 15:
      case 14:
      case 13:
        hval += asso_values[static_cast<unsigned char>(str[12])];
        break;
    }
  return hval + asso_values[static_cast<unsigned char>(str[len - 1])];
}

static struct HttpHeader wordlist[] =
  {
#line 37 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ALT_SVC", HttpHeaderCode::HTTP_HEADER_ALT_SVC},
#line 92 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_VIA", HttpHeaderCode::HTTP_HEADER_VIA},
#line 21 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCEPT", HttpHeaderCode::HTTP_HEADER_ACCEPT},
#line 35 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_AGE", HttpHeaderCode::HTTP_HEADER_AGE},
#line 22 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCEPT_CHARSET", HttpHeaderCode::HTTP_HEADER_ACCEPT_CHARSET},
#line 49 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_COOKIE", HttpHeaderCode::HTTP_HEADER_COOKIE},
#line 84 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_TE", HttpHeaderCode::HTTP_HEADER_TE},
#line 38 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_AUTHORIZATION", HttpHeaderCode::HTTP_HEADER_AUTHORIZATION},
#line 13 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_NONE", HttpHeaderCode::HTTP_HEADER_NONE},
#line 53 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_EXPECT", HttpHeaderCode::HTTP_HEADER_EXPECT},
#line 18 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_COLON_SCHEME", HttpHeaderCode::HTTP_HEADER_COLON_SCHEME},
#line 91 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_VARY", HttpHeaderCode::HTTP_HEADER_VARY},
#line 70 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_PRAGMA", HttpHeaderCode::HTTP_HEADER_PRAGMA},
#line 19 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_COLON_STATUS", HttpHeaderCode::HTTP_HEADER_COLON_STATUS},
#line 90 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_VIP", HttpHeaderCode::HTTP_HEADER_VIP},
#line 57 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_HOST", HttpHeaderCode::HTTP_HEADER_HOST},
#line 17 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_COLON_PATH", HttpHeaderCode::HTTP_HEADER_COLON_PATH},
#line 45 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_CONTENT_LOCATION", HttpHeaderCode::HTTP_HEADER_CONTENT_LOCATION},
#line 54 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_EXPIRES", HttpHeaderCode::HTTP_HEADER_EXPIRES},
#line 41 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_CONTENT_DISPOSITION", HttpHeaderCode::HTTP_HEADER_CONTENT_DISPOSITION},
#line 36 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ALLOW", HttpHeaderCode::HTTP_HEADER_ALLOW},
#line 46 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_CONTENT_MD5", HttpHeaderCode::HTTP_HEADER_CONTENT_MD5},
#line 48 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_CONTENT_TYPE", HttpHeaderCode::HTTP_HEADER_CONTENT_TYPE},
#line 47 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_CONTENT_RANGE", HttpHeaderCode::HTTP_HEADER_CONTENT_RANGE},
#line 43 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_CONTENT_LANGUAGE", HttpHeaderCode::HTTP_HEADER_CONTENT_LANGUAGE},
#line 27 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCESS_CONTROL_ALLOW_CREDENTIALS", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_ALLOW_CREDENTIALS},
#line 33 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCESS_CONTROL_REQUEST_HEADERS", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_REQUEST_HEADERS},
#line 95 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_ACCEL_REDIRECT", HttpHeaderCode::HTTP_HEADER_X_ACCEL_REDIRECT},
#line 32 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCESS_CONTROL_MAX_AGE", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_MAX_AGE},
#line 69 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_P3P", HttpHeaderCode::HTTP_HEADER_P3P},
#line 44 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_CONTENT_LENGTH", HttpHeaderCode::HTTP_HEADER_CONTENT_LENGTH},
#line 79 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_SEC_WEBSOCKET_ACCEPT", HttpHeaderCode::HTTP_HEADER_SEC_WEBSOCKET_ACCEPT},
#line 86 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_TRAILER", HttpHeaderCode::HTTP_HEADER_TRAILER},
#line 31 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCESS_CONTROL_EXPOSE_HEADERS", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_EXPOSE_HEADERS},
#line 100 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_FRAME_OPTIONS", HttpHeaderCode::HTTP_HEADER_X_FRAME_OPTIONS},
#line 101 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_POWERED_BY", HttpHeaderCode::HTTP_HEADER_X_POWERED_BY},
#line 50 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_DNT", HttpHeaderCode::HTTP_HEADER_DNT},
#line 52 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ETAG", HttpHeaderCode::HTTP_HEADER_ETAG},
#line 74 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_RANGE", HttpHeaderCode::HTTP_HEADER_RANGE},
#line 80 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_SERVER", HttpHeaderCode::HTTP_HEADER_SERVER},
#line 78 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_SEC_WEBSOCKET_KEY", HttpHeaderCode::HTTP_HEADER_SEC_WEBSOCKET_KEY},
#line 28 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCESS_CONTROL_ALLOW_HEADERS", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_ALLOW_HEADERS},
#line 40 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_CONNECTION", HttpHeaderCode::HTTP_HEADER_CONNECTION},
#line 103 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_REQUESTED_WITH", HttpHeaderCode::HTTP_HEADER_X_REQUESTED_WITH},
#line 97 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_CONTENT_TYPE_OPTIONS", HttpHeaderCode::HTTP_HEADER_X_CONTENT_TYPE_OPTIONS},
#line 26 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCEPT_RANGES", HttpHeaderCode::HTTP_HEADER_ACCEPT_RANGES},
#line 51 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_DATE", HttpHeaderCode::HTTP_HEADER_DATE},
#line 24 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCEPT_ENCODING", HttpHeaderCode::HTTP_HEADER_ACCEPT_ENCODING},
#line 42 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_CONTENT_ENCODING", HttpHeaderCode::HTTP_HEADER_CONTENT_ENCODING},
#line 76 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_REFRESH", HttpHeaderCode::HTTP_HEADER_REFRESH},
#line 107 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_XSS_PROTECTION", HttpHeaderCode::HTTP_HEADER_X_XSS_PROTECTION},
#line 16 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_COLON_METHOD", HttpHeaderCode::HTTP_HEADER_COLON_METHOD},
#line 66 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_LOCATION", HttpHeaderCode::HTTP_HEADER_LOCATION},
#line 23 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCEPT_DATETIME", HttpHeaderCode::HTTP_HEADER_ACCEPT_DATETIME},
#line 68 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ORIGIN", HttpHeaderCode::HTTP_HEADER_ORIGIN},
#line 94 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_WARNING", HttpHeaderCode::HTTP_HEADER_WARNING},
#line 106 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_WAP_PROFILE", HttpHeaderCode::HTTP_HEADER_X_WAP_PROFILE},
#line 34 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCESS_CONTROL_REQUEST_METHOD", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_REQUEST_METHOD},
#line 93 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_WWW_AUTHENTICATE", HttpHeaderCode::HTTP_HEADER_WWW_AUTHENTICATE},
#line 82 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_SEC_TOKEN_BINDING", HttpHeaderCode::HTTP_HEADER_SEC_TOKEN_BINDING},
#line 61 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_IF_RANGE", HttpHeaderCode::HTTP_HEADER_IF_RANGE},
#line 85 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_TIMESTAMP", HttpHeaderCode::HTTP_HEADER_TIMESTAMP},
#line 25 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCEPT_LANGUAGE", HttpHeaderCode::HTTP_HEADER_ACCEPT_LANGUAGE},
#line 75 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_REFERER", HttpHeaderCode::HTTP_HEADER_REFERER},
#line 29 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCESS_CONTROL_ALLOW_METHODS", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_ALLOW_METHODS},
#line 105 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_UA_COMPATIBLE", HttpHeaderCode::HTTP_HEADER_X_UA_COMPATIBLE},
#line 104 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_THRIFT_PROTOCOL", HttpHeaderCode::HTTP_HEADER_X_THRIFT_PROTOCOL},
#line 102 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_REAL_IP", HttpHeaderCode::HTTP_HEADER_X_REAL_IP},
#line 56 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_FRONT_END_HTTPS", HttpHeaderCode::HTTP_HEADER_FRONT_END_HTTPS},
#line 30 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN},
#line 60 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_IF_NONE_MATCH", HttpHeaderCode::HTTP_HEADER_IF_NONE_MATCH},
#line 83 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_STRICT_TRANSPORT_SECURITY", HttpHeaderCode::HTTP_HEADER_STRICT_TRANSPORT_SECURITY},
#line 87 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_TRANSFER_ENCODING", HttpHeaderCode::HTTP_HEADER_TRANSFER_ENCODING},
#line 58 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_IF_MATCH", HttpHeaderCode::HTTP_HEADER_IF_MATCH},
#line 67 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_MAX_FORWARDS", HttpHeaderCode::HTTP_HEADER_MAX_FORWARDS},
#line 98 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_FORWARDED_FOR", HttpHeaderCode::HTTP_HEADER_X_FORWARDED_FOR},
#line 73 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_PROXY_CONNECTION", HttpHeaderCode::HTTP_HEADER_PROXY_CONNECTION},
#line 88 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_UPGRADE", HttpHeaderCode::HTTP_HEADER_UPGRADE},
#line 20 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_COLON_PROTOCOL", HttpHeaderCode::HTTP_HEADER_COLON_PROTOCOL},
#line 14 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_OTHER", HttpHeaderCode::HTTP_HEADER_OTHER},
#line 55 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_FROM", HttpHeaderCode::HTTP_HEADER_FROM},
#line 89 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_USER_AGENT", HttpHeaderCode::HTTP_HEADER_USER_AGENT},
#line 15 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_COLON_AUTHORITY", HttpHeaderCode::HTTP_HEADER_COLON_AUTHORITY},
#line 81 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_SET_COOKIE", HttpHeaderCode::HTTP_HEADER_SET_COOKIE},
#line 96 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_CONTENT_SECURITY_POLICY_REPORT_ONLY", HttpHeaderCode::HTTP_HEADER_X_CONTENT_SECURITY_POLICY_REPORT_ONLY},
#line 62 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_IF_UNMODIFIED_SINCE", HttpHeaderCode::HTTP_HEADER_IF_UNMODIFIED_SINCE},
#line 72 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_PROXY_AUTHORIZATION", HttpHeaderCode::HTTP_HEADER_PROXY_AUTHORIZATION},
#line 99 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_X_FORWARDED_PROTO", HttpHeaderCode::HTTP_HEADER_X_FORWARDED_PROTO},
#line 39 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_CACHE_CONTROL", HttpHeaderCode::HTTP_HEADER_CACHE_CONTROL},
#line 59 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_IF_MODIFIED_SINCE", HttpHeaderCode::HTTP_HEADER_IF_MODIFIED_SINCE},
#line 71 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_PROXY_AUTHENTICATE", HttpHeaderCode::HTTP_HEADER_PROXY_AUTHENTICATE},
#line 65 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_LINK", HttpHeaderCode::HTTP_HEADER_LINK},
#line 77 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_RETRY_AFTER", HttpHeaderCode::HTTP_HEADER_RETRY_AFTER},
#line 63 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_KEEP_ALIVE", HttpHeaderCode::HTTP_HEADER_KEEP_ALIVE},
#line 64 "HttpCommomHeaders.h.gperf"
    {"HTTP_HEADER_LAST_MODIFIED", HttpHeaderCode::HTTP_HEADER_LAST_MODIFIED}
  };

static signed char lookup[] =
  {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1,  0,  1, -1, -1,  2, -1, -1, -1, -1,
    -1, -1,  3,  4, -1,  5,  6,  7,  8, -1,  9, 10, -1, 11,
    -1, 12, 13, 14, 15, 16, 17, 18, -1, 19, 20, 21, 22, 23,
    -1, -1, 24, 25, -1, -1, 26, 27, 28, 29, 30, 31, -1, 32,
    -1, 33, 34, -1, 35, 36, 37, 38, 39, 40, 41, -1, 42, 43,
    44, 45, 46, 47, 48, 49, -1, -1, -1, 50, 51, 52, -1, 53,
    54, 55, 56, 57, -1, 58, 59, 60, 61, 62, -1, 63, 64, -1,
    65, -1, 66, -1, 67, 68, -1, 69, 70, -1, 71, -1, 72, 73,
    -1, -1, -1, 74, -1, -1, 75, 76, 77, -1, 78, 79, -1, -1,
    -1, 80, 81, -1, -1, -1, -1, 82, -1, -1, -1, -1, 83, -1,
    84, -1, 85, -1, -1, -1, -1, 86, -1, -1, 87, 88, -1, -1,
    -1, 89, 90, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 91,
    -1, 92, -1, -1, -1, -1, -1, -1, -1, -1, 93, -1, -1, -1,
    -1, -1, -1, -1, 94
  };

struct HttpHeader *
Perfect_Hash::isValidHttpHeader (const char *str, size_t len)
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          int index = lookup[key];

          if (index >= 0)
            {
              const char *s = wordlist[index].headerName;

              if (*str == *s && !strcmp (str + 1, s + 1))
                return &wordlist[index];
            }
        }
    }
  return 0;
}
