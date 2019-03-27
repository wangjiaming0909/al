#include "buffer/buffer.h"
#include <cassert>
#include <string> 
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

namespace buffer_test{

#define DUMMY_CLASS_SIZE 1024
class dummy_class
{
public:
    dummy_class()
    {
        std::srand(std::time(0));
        memset(buffer_, std::rand() / (RAND_MAX), DUMMY_CLASS_SIZE);
    }
private:
    char    buffer_[DUMMY_CLASS_SIZE];
};

template <unsigned int N>
struct SizableClass{
    SizableClass()
    {
        memset(buffer_, 0, N);
    }
    char buffer_[N];
};

void test_construct_and_append_buffer(){
    buffer buf{};
    assert(buf.buffer_length() == 0);
    buf.append(1);
    assert(buf.buffer_length() == sizeof(int));
    buf.append(2);
    assert(buf.buffer_length() == sizeof(int) * 2);
    buf.append("abcd");
    assert(buf.buffer_length() == sizeof(int) * 2 + sizeof("abcd"));
    size_t total_length = buf.buffer_length();
    std::string s{"abcd"};
    buf.append(s);
    size_t length = total_length + sizeof(s);
    assert(buf.buffer_length() == length);
    auto dummy = dummy_class();
    buf.append(dummy);
    length += sizeof(dummy_class);
    assert(buf.buffer_length() == length);

    buffer buf2 = buf;
    assert(buf2.buffer_length() == length);

    buffer buf3{};
    buf3.append(dummy);
    assert(buf3.buffer_length() == 1024);
    buf3.append(1);
    assert(buf3.buffer_length() == (1024 + 4));

    //copy constructor
    buffer_iter&& iter = buf.begin();
    auto* it = &(iter + 4);
    buffer buf4{buf, 1024, it};
    assert(buf4.buffer_length() == 1024);
    auto&& iter_in_buf4 = buf4.begin();
    const char* data = static_cast<const char*>(iter_in_buf4.chain().get_buffer());
    const int* int_2 = reinterpret_cast<const int*>(data);
    assert(*int_2 == 2);
    const char* char_2 = reinterpret_cast<const char*>(data + 4);
    char* actual = "abcd";
    for(int i = 0; i < strlen(char_2); i++)
    {
        assert(char_2[i] == actual[i]);
    }


    assert(buf.chain_number() == 1);
    length = buf.buffer_length();
    auto sizable_900 = SizableClass<900>();
    buf.append(sizable_900);
    assert(buf.chain_number() == 1);
    assert(buf.buffer_length() == (length + 900));
    buf.append(SizableClass<100>());
    assert(buf.chain_number() == 2);
    const auto& chains = buf.get_chains();
    assert(chains.front().next() == &chains.back());
    assert(chains.back().next() == 0);

    assert(buf.buffer_length() == (length + 900 + 100));
    buf.append(SizableClass<1024>());
    assert(buf.chain_number() == 2);
    assert(chains.front().next() == &chains.back());
    assert(buf.buffer_length() == (length + 900 + 100 + 1024));

    buffer buf5{buf, buf.buffer_length() - 100, &(buf.begin() + 24)};
    assert(buf5.buffer_length() == (buf.buffer_length() - 100));
    assert(buf5.chain_number() == 2);
    const auto& first_chain_of_buf5 = buf5.get_chains().front();
    assert(first_chain_of_buf5.next() == &buf5.get_chains().back());
    assert(buf5.get_chains().back().next() == 0);
    assert((double)first_chain_of_buf5.get_offset() > (first_chain_of_buf5.chain_capacity() * 7 / 8.0));
}

void test_operator_equal()
{
    buffer buf1{};
    buf1.append(SizableClass<64>());
    assert(buf1.buffer_length() == 64);
    buffer buf2{};
    buf2 = buf1;
    assert(buf2.buffer_length() == 64);
}

void test_append_buffer()
{
    buffer buf1{};
    buf1.append(SizableClass<64>());
    assert(buf1.buffer_length() == 64);
    buffer buf2{};
    buf2.append(SizableClass<64>());
    assert(buf2.buffer_length() == 64);
    //capacity: 1024 and off_: 64 and add 64bytes
    buf2.append(buf1, 64, buf1.begin());//here append will append 64 bytes into the first chain in buf2, won't new a chain and append behind
    assert(buf2.buffer_length() == 64 * 2);
    assert(buf2.get_chains().size() == 1);

    buf1.append(SizableClass<4>());
    assert(buf1.buffer_length() == (64 + 4));

    //capacity: 1024, off_: 128 and add 4 bytes
    size_t length = buf2.buffer_length();
    buf2.append(buf1, 4, buf1.begin() + 64); //the same as the first append
    assert(buf2.buffer_length() == length + 4);
    assert(buf2.get_chains().size() == 1);

    int ret = memcmp(buf1.last_chain_with_data()->get_buffer(), buf2.last_chain_with_data()->get_buffer(), buf2.buffer_length());
    assert(ret == 0);

    buffer buf3{};
    buf3.append(SizableClass<850>());
    length = buf2.buffer_length();
    buf2.append(buf3, buf3.buffer_length(), buf3.begin());
    assert(buf2.buffer_length() == length + 850);

    length = buf2.buffer_length();
    buf2.append(buf1, buf1.buffer_length(), buf1.begin());
    assert(buf2.buffer_length() == length + buf1.buffer_length());
    assert(buf2.get_chains().size() == 2);
}

void test_buffer_begin_end()
{

}

void run_tests(){
    test_construct_and_append_buffer();
    test_operator_equal();
    test_append_buffer();
}
}