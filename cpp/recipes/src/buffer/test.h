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

void test_construct_and_append_buffer(){
    buffer buf{};
    assert(buf.buffer_length() == 0);
    buf.append(1);
    assert(buf.buffer_length() == 4);
    buf.append(2);
    assert(buf.buffer_length() == 8);
    buf.append("abcd");
    assert(buf.buffer_length() == 8 + sizeof("abcd"));
    size_t total_length = buf.buffer_length();
    std::string s{"abcd"};
    buf.append(s);
    assert(buf.buffer_length() == (total_length + sizeof(s)));//37
    auto dummy = dummy_class();
    buf.append(dummy);
    assert(buf.buffer_length() == (37 + sizeof(dummy_class)));

    buffer buf2 = buf;
    assert(buf2.buffer_length() == (37 + sizeof(dummy_class)));

    buffer buf3{};
    buf3.append(dummy);
    assert(buf3.buffer_length() == 1024);
    buf3.append(1);
    assert(buf3.buffer_length() == (1024 + 4));

    buffer_iter iter = buf.begin();
    auto* it = &(iter + 4);
    buffer buf4{buf, 1024, it};
    assert(buf4.buffer_length() == 1024);
    auto iter_in_buf4 = buf4.begin();
    char* data = static_cast<char*>(iter_in_buf4.chain().get_buffer());
    int* int_2 = reinterpret_cast<int*>(data);
    assert(*int_2 == 2);
    char* char_2 = reinterpret_cast<char*>(data + 4);
    char* actual = "abcd";
    for(int i = 0; i < strlen(char_2); i++)
    {
        assert(char_2[i] == actual[i]);
    }
}

void test_buffer_begin_end()
{

}

void run_tests(){
    test_construct_and_append_buffer();
}
}