#include <gtest/gtest.h>
#include "http/HttpHeaders.h"

using namespace http;

namespace http_test
{

void assertHeadersSize(const HttpHeaders& headers, size_t size)
{
    ASSERT_EQ(headers.size(), size);
}

void assertCodesSize(const HttpHeaders& headers, size_t size, size_t capacity)
{
    ASSERT_EQ(headers.getCodes().size(), size);
    ASSERT_EQ(headers.getCodes().capacity(), capacity);
}

void assertHeaderNameSize(const HttpHeaders& headers, size_t size, size_t capacity)
{
    ASSERT_EQ(headers.getHeaderNames().size(), size);
    ASSERT_EQ(headers.getHeaderNames().capacity(), capacity);
}

void assertHeaderValueSize(const HttpHeaders& headers, size_t size, size_t capacity)
{
    ASSERT_EQ(headers.getHeaderValues().size(), size);
    ASSERT_EQ(headers.getHeaderValues().capacity(), capacity);
}

TEST(http_headers, constructor)
{
    HttpHeaders headers{};

    assertHeadersSize(headers, 0);
    assertCodesSize(headers, 0, HttpHeaders::getInitVectorReserveSize());
    assertHeaderNameSize(headers, 0, HttpHeaders::getInitVectorReserveSize());
    assertHeaderValueSize(headers, 0, HttpHeaders::getInitVectorReserveSize());
    ASSERT_EQ(headers.getCodesDeleted(), 0);
}

TEST(http_headers, copy_construcot)
{
    HttpHeaders headers1{};
    HttpHeaders headers2 = headers1;

    assertHeadersSize(headers1, 0);
    assertCodesSize(headers1, 0, HttpHeaders::getInitVectorReserveSize());
    assertHeaderNameSize(headers1, 0, HttpHeaders::getInitVectorReserveSize());
    assertHeaderValueSize(headers1, 0, HttpHeaders::getInitVectorReserveSize());
    ASSERT_EQ(headers1.getCodesDeleted(), 0);

    assertHeadersSize(headers2, 0);
    assertCodesSize(headers2, 0, 0);
    assertHeaderNameSize(headers2, 0, 0);
    assertHeaderValueSize(headers2, 0, 0);
    ASSERT_EQ(headers2.getCodesDeleted(), 0);
}

}