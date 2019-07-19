#include <gtest/gtest.h>
#include <folly/container/BitIterator.h>
#include <bitset>
#include "utils/timer.h"
#include <iostream>

TEST(bititerator, performance_with_std_bitset)
{
    static const int WHEEL_SIZE = 1 << 16;
    size_t pos = 32700;

    {
        utils::timer _{"using folly findFirstSet"};
        std::array<std::size_t, (WHEEL_SIZE / sizeof(std::size_t)) / 8> bitmap;
        bitmap.fill(0);

        auto beg = folly::makeBitIterator(bitmap.begin());
        auto end = folly::makeBitIterator(bitmap.end());

        *(beg + 32700) = true;

        auto it = folly::findFirstSet(beg, end);
        std::cout << "folly: " << (it - beg) << std::endl;
        ASSERT_EQ(it - beg, pos);
    }


    {
        utils::timer _{"using std::bitset"};
        std::bitset<WHEEL_SIZE> bits;
        bits.reset();
        bits.set(32700);

        auto it = bits._Find_first();
        std::cout << "std: " << it << std::endl;;
        ASSERT_EQ(it, pos);
    }

}