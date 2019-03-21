#include "buffer/buffer.h"
#include <cassert>

namespace buffer_test{

void test_construct_buffer(){
    buffer buf{};
    assert(buf.total_len() == 0);
    buf.append(1);
    assert(buf.total_len() == 4);
}

void run_tests(){
    test_construct_buffer();
}
}