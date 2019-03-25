#include "buffer/buffer.h"
#include <cassert>
#include <string> 
#include <iostream>
using namespace std;

namespace buffer_test{

#define DUMMY_CLASS_SIZE 1024
class dummy_class
{
public:
    dummy_class()
    {
        memset(buffer_, 0, DUMMY_CLASS_SIZE);
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
    char* data = static_cast<char*>(iter_in_buf4.chain().get_buffer());
    int* int_2 = reinterpret_cast<int*>(data);
    assert(*int_2 == 2);
    char* char_2 = reinterpret_cast<char*>(data + 4);
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

void test_buffer_begin_end()
{

}

void run_tests(){
    test_construct_and_append_buffer();
    test_operator_equal();
}
}