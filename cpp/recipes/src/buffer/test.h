#include "buffer/buffer.h"
#include <cassert>
#include <string> 

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
    assert(buf.total_len() == 0);
    buf.append(1);
    assert(buf.total_len() == 4);
    buf.append(2);
    assert(buf.total_len() == 8);
    buf.append("abcd");
    assert(buf.total_len() == 8 + sizeof("abcd"));
    size_t total_length = buf.total_len();
    std::string s{"abcd"};
    buf.append(s);
    assert(buf.total_len() == (total_length + sizeof(s)));//37
    auto dummy = dummy_class();
    buf.append(dummy);
    assert(buf.total_len() == (37 + sizeof(dummy_class)));

    buffer buf2 = buf;
    assert(buf2.total_len() == (37 + sizeof(dummy_class)));
}

void test_buffer_begin_end()
{

}

void run_tests(){
    test_construct_and_append_buffer();
}
}