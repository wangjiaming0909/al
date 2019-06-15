/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf -DG -K headerName -N isValidHttpHeader -L C++ -Z HttpCommomHeaderInternal -t HttpCommomHeadersHash.h.gperf  */
/* Computed positions: -k'1,8,22,$' */

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

#line 1 "HttpCommomHeadersHash.h.gperf"

#include "HttpCommomHeadersHash.h"

namespace http{

#line 7 "HttpCommomHeadersHash.h.gperf"

inline unsigned int
HttpCommomHeaderInternal::hash (const char *str, size_t len)
{
  static unsigned char asso_values[] =
    {
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149,   0, 149, 149, 149, 149,
      149, 149, 149,  45, 149, 149, 149, 149,  55, 149,
      149, 149, 149, 149, 149,   0, 149,   0,  45,   5,
       60, 149,   0,   5, 149,   5,  40, 120, 149,  90,
        0, 149,   5,  20,  45, 100,  35,  70,  20, 149,
      149, 149, 149, 149, 149, 149, 149,  35, 149,   0,
       35,  10,  75,  35,  25,   0, 149,  10,  30,  30,
        5,  45,   0, 149,   5,   0,   0,  25, 149,   0,
      149,  35,  50, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149, 149, 149, 149, 149,
      149, 149, 149, 149, 149, 149
    };
  unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[static_cast<unsigned char>(str[21])];
      /*FALLTHROUGH*/
      case 21:
      case 20:
      case 19:
      case 18:
      case 17:
      case 16:
      case 15:
      case 14:
      case 13:
      case 12:
      case 11:
      case 10:
      case 9:
      case 8:
        hval += asso_values[static_cast<unsigned char>(str[7])];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
      case 2:
      case 1:
        hval += asso_values[static_cast<unsigned char>(str[0])];
        break;
    }
  return hval + asso_values[static_cast<unsigned char>(str[len - 1])];
}

static struct HttpHeader wordlist[] =
  {
#line 67 "HttpCommomHeadersHash.h.gperf"
    {"P3P", HttpHeaderCode::HTTP_HEADER_P3P},
#line 55 "HttpCommomHeadersHash.h.gperf"
    {"Host", HttpHeaderCode::HTTP_HEADER_HOST},
#line 34 "HttpCommomHeadersHash.h.gperf"
    {"Allow", HttpHeaderCode::HTTP_HEADER_ALLOW},
#line 19 "HttpCommomHeadersHash.h.gperf"
    {"Accept", HttpHeaderCode::HTTP_HEADER_ACCEPT},
#line 35 "HttpCommomHeadersHash.h.gperf"
    {"Alt-Svc", HttpHeaderCode::HTTP_HEADER_ALT_SVC},
#line 51 "HttpCommomHeadersHash.h.gperf"
    {"Expect", HttpHeaderCode::HTTP_HEADER_EXPECT},
#line 52 "HttpCommomHeadersHash.h.gperf"
    {"Expires", HttpHeaderCode::HTTP_HEADER_EXPIRES},
#line 33 "HttpCommomHeadersHash.h.gperf"
    {"Age", HttpHeaderCode::HTTP_HEADER_AGE},
#line 20 "HttpCommomHeadersHash.h.gperf"
    {"Accept-Charset", HttpHeaderCode::HTTP_HEADER_ACCEPT_CHARSET},
#line 38 "HttpCommomHeadersHash.h.gperf"
    {"Connection", HttpHeaderCode::HTTP_HEADER_CONNECTION},
#line 47 "HttpCommomHeadersHash.h.gperf"
    {"Cookie", HttpHeaderCode::HTTP_HEADER_COOKIE},
#line 73 "HttpCommomHeadersHash.h.gperf"
    {"Referer", HttpHeaderCode::HTTP_HEADER_REFERER},
#line 24 "HttpCommomHeadersHash.h.gperf"
    {"Accept-Ranges", HttpHeaderCode::HTTP_HEADER_ACCEPT_RANGES},
#line 72 "HttpCommomHeadersHash.h.gperf"
    {"Range", HttpHeaderCode::HTTP_HEADER_RANGE},
#line 43 "HttpCommomHeadersHash.h.gperf"
    {"Content-Location", HttpHeaderCode::HTTP_HEADER_CONTENT_LOCATION},
#line 46 "HttpCommomHeadersHash.h.gperf"
    {"Content-Type", HttpHeaderCode::HTTP_HEADER_CONTENT_TYPE},
#line 45 "HttpCommomHeadersHash.h.gperf"
    {"Content-Range", HttpHeaderCode::HTTP_HEADER_CONTENT_RANGE},
#line 39 "HttpCommomHeadersHash.h.gperf"
    {"Content-Disposition", HttpHeaderCode::HTTP_HEADER_CONTENT_DISPOSITION},
#line 61 "HttpCommomHeadersHash.h.gperf"
    {"Keep-Alive", HttpHeaderCode::HTTP_HEADER_KEEP_ALIVE},
#line 41 "HttpCommomHeadersHash.h.gperf"
    {"Content-Language", HttpHeaderCode::HTTP_HEADER_CONTENT_LANGUAGE},
#line 26 "HttpCommomHeadersHash.h.gperf"
    {"Access-Control-Allow-Headers", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_ALLOW_HEADERS},
#line 29 "HttpCommomHeadersHash.h.gperf"
    {"Access-Control-Expose-Headers", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_EXPOSE_HEADERS},
#line 31 "HttpCommomHeadersHash.h.gperf"
    {"Access-Control-Request-Headers", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_REQUEST_HEADERS},
#line 78 "HttpCommomHeadersHash.h.gperf"
    {"Server", HttpHeaderCode::HTTP_HEADER_SERVER},
#line 25 "HttpCommomHeadersHash.h.gperf"
    {"Access-Control-Allow-Credentials", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_ALLOW_CREDENTIALS},
#line 59 "HttpCommomHeadersHash.h.gperf"
    {"If-Range", HttpHeaderCode::HTTP_HEADER_IF_RANGE},
#line 100 "HttpCommomHeadersHash.h.gperf"
    {"X-Real-IP", HttpHeaderCode::HTTP_HEADER_X_REAL_IP},
#line 98 "HttpCommomHeadersHash.h.gperf"
    {"X-Frame-Options", HttpHeaderCode::HTTP_HEADER_X_FRAME_OPTIONS},
#line 93 "HttpCommomHeadersHash.h.gperf"
    {"X-Accel-Redirect", HttpHeaderCode::HTTP_HEADER_X_ACCEL_REDIRECT},
#line 74 "HttpCommomHeadersHash.h.gperf"
    {"Refresh", HttpHeaderCode::HTTP_HEADER_REFRESH},
#line 88 "HttpCommomHeadersHash.h.gperf"
    {"VIP", HttpHeaderCode::HTTP_HEADER_VIP},
#line 42 "HttpCommomHeadersHash.h.gperf"
    {"Content-Length", HttpHeaderCode::HTTP_HEADER_CONTENT_LENGTH},
#line 68 "HttpCommomHeadersHash.h.gperf"
    {"Pragma", HttpHeaderCode::HTTP_HEADER_PRAGMA},
#line 30 "HttpCommomHeadersHash.h.gperf"
    {"Access-Control-Max-Age", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_MAX_AGE},
#line 58 "HttpCommomHeadersHash.h.gperf"
    {"If-None-Match", HttpHeaderCode::HTTP_HEADER_IF_NONE_MATCH},
#line 50 "HttpCommomHeadersHash.h.gperf"
    {"ETag", HttpHeaderCode::HTTP_HEADER_ETAG},
#line 96 "HttpCommomHeadersHash.h.gperf"
    {"X-Forwarded-For", HttpHeaderCode::HTTP_HEADER_X_FORWARDED_FOR},
#line 105 "HttpCommomHeadersHash.h.gperf"
    {"X-XSS-Protection", HttpHeaderCode::HTTP_HEADER_X_XSS_PROTECTION},
#line 95 "HttpCommomHeadersHash.h.gperf"
    {"X-Content-Type-Options", HttpHeaderCode::HTTP_HEADER_X_CONTENT_TYPE_OPTIONS},
#line 104 "HttpCommomHeadersHash.h.gperf"
    {"X-Wap-Profile", HttpHeaderCode::HTTP_HEADER_X_WAP_PROFILE},
#line 70 "HttpCommomHeadersHash.h.gperf"
    {"Proxy-Authorization", HttpHeaderCode::HTTP_HEADER_PROXY_AUTHORIZATION},
#line 79 "HttpCommomHeadersHash.h.gperf"
    {"Set-Cookie", HttpHeaderCode::HTTP_HEADER_SET_COOKIE},
#line 40 "HttpCommomHeadersHash.h.gperf"
    {"Content-Encoding", HttpHeaderCode::HTTP_HEADER_CONTENT_ENCODING},
#line 82 "HttpCommomHeadersHash.h.gperf"
    {"TE", HttpHeaderCode::HTTP_HEADER_TE},
#line 69 "HttpCommomHeadersHash.h.gperf"
    {"Proxy-Authenticate", HttpHeaderCode::HTTP_HEADER_PROXY_AUTHENTICATE},
#line 63 "HttpCommomHeadersHash.h.gperf"
    {"Link", HttpHeaderCode::HTTP_HEADER_LINK},
#line 22 "HttpCommomHeadersHash.h.gperf"
    {"Accept-Encoding", HttpHeaderCode::HTTP_HEADER_ACCEPT_ENCODING},
#line 44 "HttpCommomHeadersHash.h.gperf"
    {"Content-MD5", HttpHeaderCode::HTTP_HEADER_CONTENT_MD5},
#line 84 "HttpCommomHeadersHash.h.gperf"
    {"Trailer", HttpHeaderCode::HTTP_HEADER_TRAILER},
#line 64 "HttpCommomHeadersHash.h.gperf"
    {"Location", HttpHeaderCode::HTTP_HEADER_LOCATION},
#line 49 "HttpCommomHeadersHash.h.gperf"
    {"Date", HttpHeaderCode::HTTP_HEADER_DATE},
#line 77 "HttpCommomHeadersHash.h.gperf"
    {"Sec-WebSocket-Accept", HttpHeaderCode::HTTP_HEADER_SEC_WEBSOCKET_ACCEPT},
#line 101 "HttpCommomHeadersHash.h.gperf"
    {"X-Requested-With", HttpHeaderCode::HTTP_HEADER_X_REQUESTED_WITH},
#line 17 "HttpCommomHeadersHash.h.gperf"
    {":Status", HttpHeaderCode::HTTP_HEADER_COLON_STATUS},
#line 56 "HttpCommomHeadersHash.h.gperf"
    {"If-Match", HttpHeaderCode::HTTP_HEADER_IF_MATCH},
#line 32 "HttpCommomHeadersHash.h.gperf"
    {"Access-Control-Request-Method", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_REQUEST_METHOD},
#line 23 "HttpCommomHeadersHash.h.gperf"
    {"Accept-Language", HttpHeaderCode::HTTP_HEADER_ACCEPT_LANGUAGE},
#line 71 "HttpCommomHeadersHash.h.gperf"
    {"Proxy-Connection", HttpHeaderCode::HTTP_HEADER_PROXY_CONNECTION},
#line 102 "HttpCommomHeadersHash.h.gperf"
    {"X-Thrift-Protocol", HttpHeaderCode::HTTP_HEADER_X_THRIFT_PROTOCOL},
#line 36 "HttpCommomHeadersHash.h.gperf"
    {"Authorization", HttpHeaderCode::HTTP_HEADER_AUTHORIZATION},
#line 60 "HttpCommomHeadersHash.h.gperf"
    {"If-Unmodified-Since", HttpHeaderCode::HTTP_HEADER_IF_UNMODIFIED_SINCE},
#line 21 "HttpCommomHeadersHash.h.gperf"
    {"Accept-Datetime", HttpHeaderCode::HTTP_HEADER_ACCEPT_DATETIME},
#line 16 "HttpCommomHeadersHash.h.gperf"
    {":Scheme", HttpHeaderCode::HTTP_HEADER_COLON_SCHEME},
#line 90 "HttpCommomHeadersHash.h.gperf"
    {"Via", HttpHeaderCode::HTTP_HEADER_VIA},
#line 89 "HttpCommomHeadersHash.h.gperf"
    {"Vary", HttpHeaderCode::HTTP_HEADER_VARY},
#line 103 "HttpCommomHeadersHash.h.gperf"
    {"X-UA-Compatible", HttpHeaderCode::HTTP_HEADER_X_UA_COMPATIBLE},
#line 99 "HttpCommomHeadersHash.h.gperf"
    {"X-Powered-By", HttpHeaderCode::HTTP_HEADER_X_POWERED_BY},
#line 54 "HttpCommomHeadersHash.h.gperf"
    {"Front-End-Https", HttpHeaderCode::HTTP_HEADER_FRONT_END_HTTPS},
#line 80 "HttpCommomHeadersHash.h.gperf"
    {"Sec-Token-Binding", HttpHeaderCode::HTTP_HEADER_SEC_TOKEN_BINDING},
#line 83 "HttpCommomHeadersHash.h.gperf"
    {"Timestamp", HttpHeaderCode::HTTP_HEADER_TIMESTAMP},
#line 15 "HttpCommomHeadersHash.h.gperf"
    {":Path", HttpHeaderCode::HTTP_HEADER_COLON_PATH},
#line 97 "HttpCommomHeadersHash.h.gperf"
    {"X-Forwarded-Proto", HttpHeaderCode::HTTP_HEADER_X_FORWARDED_PROTO},
#line 37 "HttpCommomHeadersHash.h.gperf"
    {"Cache-Control", HttpHeaderCode::HTTP_HEADER_CACHE_CONTROL},
#line 76 "HttpCommomHeadersHash.h.gperf"
    {"Sec-WebSocket-Key", HttpHeaderCode::HTTP_HEADER_SEC_WEBSOCKET_KEY},
#line 48 "HttpCommomHeadersHash.h.gperf"
    {"DNT", HttpHeaderCode::HTTP_HEADER_DNT},
#line 53 "HttpCommomHeadersHash.h.gperf"
    {"From", HttpHeaderCode::HTTP_HEADER_FROM},
#line 75 "HttpCommomHeadersHash.h.gperf"
    {"Retry-After", HttpHeaderCode::HTTP_HEADER_RETRY_AFTER},
#line 14 "HttpCommomHeadersHash.h.gperf"
    {":Method", HttpHeaderCode::HTTP_HEADER_COLON_METHOD},
#line 13 "HttpCommomHeadersHash.h.gperf"
    {":Authority", HttpHeaderCode::HTTP_HEADER_COLON_AUTHORITY},
#line 66 "HttpCommomHeadersHash.h.gperf"
    {"Origin", HttpHeaderCode::HTTP_HEADER_ORIGIN},
#line 85 "HttpCommomHeadersHash.h.gperf"
    {"Transfer-Encoding", HttpHeaderCode::HTTP_HEADER_TRANSFER_ENCODING},
#line 57 "HttpCommomHeadersHash.h.gperf"
    {"If-Modified-Since", HttpHeaderCode::HTTP_HEADER_IF_MODIFIED_SINCE},
#line 92 "HttpCommomHeadersHash.h.gperf"
    {"Warning", HttpHeaderCode::HTTP_HEADER_WARNING},
#line 86 "HttpCommomHeadersHash.h.gperf"
    {"Upgrade", HttpHeaderCode::HTTP_HEADER_UPGRADE},
#line 87 "HttpCommomHeadersHash.h.gperf"
    {"User-Agent", HttpHeaderCode::HTTP_HEADER_USER_AGENT},
#line 91 "HttpCommomHeadersHash.h.gperf"
    {"WWW-Authenticate", HttpHeaderCode::HTTP_HEADER_WWW_AUTHENTICATE},
#line 28 "HttpCommomHeadersHash.h.gperf"
    {"Access-Control-Allow-Origin", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN},
#line 62 "HttpCommomHeadersHash.h.gperf"
    {"Last-Modified", HttpHeaderCode::HTTP_HEADER_LAST_MODIFIED},
#line 94 "HttpCommomHeadersHash.h.gperf"
    {"X-Content-Security-Policy-Report-Only", HttpHeaderCode::HTTP_HEADER_X_CONTENT_SECURITY_POLICY_REPORT_ONLY},
#line 81 "HttpCommomHeadersHash.h.gperf"
    {"Strict-Transport-Security", HttpHeaderCode::HTTP_HEADER_STRICT_TRANSPORT_SECURITY},
#line 65 "HttpCommomHeadersHash.h.gperf"
    {"Max-Forwards", HttpHeaderCode::HTTP_HEADER_MAX_FORWARDS},
#line 18 "HttpCommomHeadersHash.h.gperf"
    {":Protocol", HttpHeaderCode::HTTP_HEADER_COLON_PROTOCOL},
#line 27 "HttpCommomHeadersHash.h.gperf"
    {"Access-Control-Allow-Methods", HttpHeaderCode::HTTP_HEADER_ACCESS_CONTROL_ALLOW_METHODS}
  };

static signed char lookup[] =
  {
    -1, -1, -1,  0,  1,  2,  3,  4, -1, -1, -1,  5,  6,  7,
     8,  9, 10, 11, 12, -1, 13, 14, 15, 16, 17, 18, 19, -1,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, -1, 32,
    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
    47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
    61, -1, 62, 63, 64, 65, -1, 66, -1, -1, 67, -1, 68, -1,
    69, 70, -1, 71, 72, -1, -1, -1, 73, 74, 75, -1, 76, 77,
    -1, -1, 78, 79, 80, -1, -1, -1, -1, 81, -1, -1, -1, -1,
    82, -1, -1, -1, -1, 83, -1, -1, 84, 85, 86, 87, -1, -1,
    -1, 88, -1, -1, 89, -1, 90, -1, -1, -1, -1, -1, -1, 91,
    -1, -1, -1, -1, -1, -1, -1, -1, 92
  };

struct HttpHeader *
HttpCommomHeaderInternal::isValidHttpHeader (const char *str, size_t len)
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
#line 106 "HttpCommomHeadersHash.h.gperf"

} //namespace 