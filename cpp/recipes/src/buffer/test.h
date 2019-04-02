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

void test_buffer_chain_constructor()
{
    buffer buf1{};
    buf1.append(SizableClass<64>());
    const buffer_chain& chain1 = buf1.get_chains().front();
    buffer_chain chain2{chain1};
    assert(chain2.next() == 0);
    assert(chain2.size() == 64);
    assert(chain2.get_offset() == 64);
    int ret = chain2.set_misalign(4);
    assert(ret == 4);
    assert(chain2.size() == 64 - 4);

    //copy constructor

    // buffer_chain chain3{chain2, 16, chain2.begin()};
    // assert(chain3.get_misalign() == 4);
    // assert(chain3.size() == 64 - 4);
    //expceted exception
    try {//iter不是chain2的iter
        buffer_chain chain4{chain2, 16, chain1.begin()};
    }catch(std::exception& e){}

    try {//iter达到末尾了,没有可以复制的内容了
        buffer_chain chain4{chain2, 16, chain2.end() + 1};
    }catch(std::exception& e){}
    //TODO 没有测到iter在misalign_之前时的情况, 因为好像不能获得这个iter
    
}

void test_construct_and_append_buffer(){
    buffer buf{};
    assert(buf.buffer_length() == 0);
    buf.append(1);
    assert(buf.buffer_length() == sizeof(int));
    buf.append(2);
    assert(buf.buffer_length() == sizeof(int) * 2);
    buf.append("abcd");
    assert(buf.buffer_length() == sizeof(int) * 2 + sizeof("abcd"));
    uint32_t total_length = buf.buffer_length();
    std::string s{"abcd"};
    buf.append(s);
    uint32_t length = total_length + sizeof(s);
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
    // auto* it = &(iter + 4);
    buffer buf4{buf, 1024, iter + 4};
    assert(buf4.buffer_length() == 1024);
    auto&& iter_in_buf4 = buf4.begin();
    const char* data = static_cast<const char*>(iter_in_buf4.chain().get_buffer());
    const int* int_2 = reinterpret_cast<const int*>(data);
    assert(*int_2 == 2);
    const char* char_2 = reinterpret_cast<const char*>(data + 4);
    const char* actual = "abcd";
    for(uint32_t i = 0; i < strlen(char_2); i++)
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

    buffer buf5{buf, buf.buffer_length() - 100, buf.begin() + 24};
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
    uint32_t length = buf2.buffer_length();
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

void test_buffer_append_chain()
{
    buffer buf1{};
    buf1.append(SizableClass<1023>());
    assert(buf1.buffer_length() == 1023);
    assert(buf1.chain_number() == 1);
    buf1.append(SizableClass<4>());
    assert(buf1.buffer_length() == 1023 + 4);
    assert(buf1.chain_number() == 2);

    const buffer_chain& chain1 = buf1.get_chains().back();
    assert(chain1.size() == 4);
    buf1.append(chain1);
    assert(buf1.buffer_length() == 1023 + 4 + 4);
    assert(buf1.chain_number() == 2);
}

void test_pullup()
{
/*-----------------------------only two chains----------------------------------------*/
    const uint32_t default_size = buffer_chain::DEFAULT_CHAIN_SIZE;
    buffer buf1{};
    buf1.append(SizableClass<default_size - 1>());
    const auto* first_chain = &buf1.get_chains().front();
    const auto* next_chain = first_chain->next();
    assert(next_chain == 0);
    assert(first_chain->chain_capacity() == default_size);
    assert(buf1.buffer_length() == default_size - 1);
    assert(buf1.chain_number() == 1);
    assert(buf1.last_chain_with_data() == first_chain);
    buf1.append(1);
    next_chain = first_chain->next();
    assert(buf1.buffer_length() == default_size - 1 + sizeof (int));
    assert(buf1.chain_number() == 2);
    assert(next_chain != 0);
    assert(next_chain->size() == sizeof (int));
    assert(buf1.last_chain_with_data() == next_chain);

    //the existed data in first chain is enough
    auto* p = buf1.pullup(default_size - 1);
    first_chain = &buf1.get_chains().front();
    next_chain = first_chain->next();
    assert(first_chain->chain_capacity() == default_size);
    assert(buf1.buffer_length() == default_size - 1 + sizeof (int));
    assert(buf1.chain_number() == 2);
    assert(p == first_chain->get_buffer());
    assert(::memcmp(p, first_chain->get_buffer(), first_chain->chain_capacity()) == 0);

    //the existed data in first chain is not enough, but the first chain is big enough
    buffer buf2 = buf1;
    p = buf2.pullup(default_size);
    //第一个chain已经满了
    first_chain = &buf2.get_chains().front();
    next_chain = first_chain->next();
    assert(first_chain->get_misalign() == 0);
    assert(first_chain->size() == first_chain->chain_capacity());
    assert(first_chain->get_offset() == first_chain->chain_capacity());
    //第二个chain的 misalign会向后移动一格
    assert(next_chain->get_misalign() == 1);
    assert(::memcmp(static_cast<const char*>(first_chain->get_buffer()) + default_size - 1, next_chain->get_buffer(), 1 ) == 0);

    //the first chain is not enough, keep the next chain, remain one byte in the next chain
    buffer buf3 = buf1;
    uint32_t size = default_size - 1 + sizeof (int) - 1;
    p = buf3.pullup(size);
    first_chain = &buf3.get_chains().front();
    next_chain = first_chain->next();
    assert(first_chain->chain_capacity() >= size);
    assert(first_chain->size() == size);
    assert(next_chain->size() == 1);
    assert(next_chain->get_misalign() == sizeof (int) - 1);

    //the size is going to pullup equals to the sum of first_chain and the second chain
    //after pullup, the next chain will be deleted
    buffer buf4 = buf1;
    size = default_size - 1 + sizeof(int);
    p = buf4.pullup(size);
    first_chain = &buf4.get_chains().front();
    next_chain = first_chain->next();
    assert(buf4.get_chains().size() == 1);
    assert(first_chain->chain_capacity() >= size);
    assert(first_chain->size() == size);
    assert(next_chain == 0);
/*-----------------------------only two chains----------------------------------------*/
}

void test_pullup_with_more_chains()
{
    buffer buf1{};
    const uint32_t size1 = 1020, size2 = 1010;
    const uint32_t size3_1 = 100, size3_2 = 900;
    const uint32_t size4 = 100;
    buf1.append(SizableClass<size1>());
    assert(buf1.chain_number() == 1);
    buf1.append(1.1);
    assert(buf1.chain_number() == 2);
    buf1.append(SizableClass<size2>());//size2 + sizeof(double)
    assert(buf1.chain_number() == 2);
    buf1.append(SizableClass<size3_1>());
    assert(buf1.chain_number() == 3);
    buf1.append(SizableClass<size3_2>());
    assert(buf1.chain_number() == 3);
    buf1.append(SizableClass<size4>());
    assert(buf1.chain_number() == 4);
    //1020/1024, 1018/1024, 1000/1024, 100/1024

    //the existed data in first chain is enough
    buffer buf2 = buf1;
    const unsigned char* p = buf2.pullup(1019);
    assert(buf1.chain_number() == 4);
    auto* first_chain = &buf2.get_chains().front();
    assert(first_chain->size() == size1);
    assert(first_chain->chain_capacity() == 1024);
    assert(p == first_chain->get_buffer());
    int ret = memcmp(p, first_chain->get_buffer(), first_chain->size());
    assert(ret == 0);

    //the existed data in first chain is not enough, but the first chain is big enough
    buffer buf3 = buf1;
    p = buf3.pullup(1024);
    first_chain = &buf3.get_chains().front();
    auto* next_chain = first_chain->next();
    assert(buf3.chain_number() == 4);
    assert(first_chain->chain_capacity() >= 1024);
    assert(first_chain->size() == 1024);
    assert(next_chain->get_misalign() == 4);
    assert(p == first_chain->get_buffer());
    ret = memcmp(p, first_chain->get_buffer(), first_chain->size());
    assert(ret == 0);

    //the first chain is not enough, keep the next chain, remain some bytes in the second chain
    buffer buf4 = buf1;
    p = buf4.pullup(2020);
    first_chain = &buf4.get_chains().front();
    next_chain = first_chain->next();
    assert(buf4.chain_number() == 4);
    assert(first_chain->chain_capacity() >= 2020);
    assert(first_chain->size() == 2020);
    assert(next_chain->get_misalign() == (2020 - 1020));
    assert(p == first_chain->get_buffer());
    ret = memcmp(p, first_chain->get_buffer(), first_chain->size());
    assert(ret == 0);

    //pullup all data from the first_chain and next_chian
    buffer buf5 = buf1;
    uint32_t size_going_to_pullup = size1 + size2 + sizeof(double);
    p = buf5.pullup(size_going_to_pullup);
    first_chain = &buf5.get_chains().front();
    next_chain = first_chain->next();
    assert(buf5.chain_number() == 3);
    assert(first_chain->chain_capacity() >= size_going_to_pullup);
    assert(first_chain->size() == size_going_to_pullup);
    assert(next_chain->size() == 1000);
    assert(next_chain->get_misalign() == 0);
    assert(p == first_chain->get_buffer());
    ret = memcmp(p, first_chain->get_buffer(), first_chain->size());
    assert(ret == 0);

    //pull all data from first 3 chains
    buffer buf6 = buf1;
    size_going_to_pullup = size1 + size2 + sizeof(double) + size3_1 + size3_2;
    p = buf6.pullup(size_going_to_pullup);
    first_chain = &buf6.get_chains().front();
    next_chain = first_chain->next();
    assert(buf6.chain_number() == 2);
    assert(first_chain->chain_capacity() >= size_going_to_pullup);
    assert(first_chain->size() == size_going_to_pullup);
    assert(next_chain->size() == 100);
    assert(next_chain->get_misalign() == 0);
    assert(p == first_chain->get_buffer());
    ret = ::memcmp(p, first_chain->get_buffer(), first_chain->size());
    assert(ret == 0);

    //pullup all data from the buffer
    buffer buf7 = buf1;
    size_going_to_pullup = size1 + size2 + sizeof(double) + size3_1 + size3_2 + size4;
    p = buf7.pullup(size_going_to_pullup);
    first_chain = &buf7.get_chains().front();
    next_chain = first_chain->next();
    assert(buf7.chain_number() == 1);
    assert(first_chain->chain_capacity() >= size_going_to_pullup);
    assert(first_chain->size() == size_going_to_pullup);
    assert(next_chain == 0);
    assert(p == first_chain->get_buffer());
    ret = ::memcmp(p, first_chain->get_buffer(), first_chain->size());
    assert(ret == 0);
}

void test_remove()
{
    buffer buf{};
    buf.append(SizableClass<1023>());
    buf.append(SizableClass<4>());
    buf.append(SizableClass<1010>());

    //1, the first chain has enough data
    buffer buf1 = buf;
    assert(buf1.chain_number() == 2);
    uint32_t data_len = 4096;
    void* p = ::calloc(data_len, 1);
    memset(p, 1, data_len);
    buf1.remove(p, 10);
    const buffer_chain* first_chain = &buf1.get_chains().front();
    assert(first_chain->get_misalign() == 10);
    for (uint32_t i = 0; i < 10; i++) {
        assert(static_cast<char*>(p)[i] == '\0');
    }
    for (uint32_t i = 10; i < data_len; i++) {
        assert(static_cast<char*>(p)[i] == 1);
    }

    //2, remove the first chain (size == first_chain->size())
    buffer buf2 = buf;
    memset(p, 1, data_len);
    buf2.remove(p, 1023);
    first_chain = &buf2.get_chains().front();//the first chain has changed, 第一个chain被删除了
    assert(first_chain->size() == 1014);
    assert(first_chain->get_misalign() == 0);
    assert(buf2.chain_number() == 1);
    for (uint32_t i = 0; i < 1023;i++) {
        assert(static_cast<char*>(p)[i] == '\0');
    }
    for (uint32_t i = 1023; i < data_len; i++) {
        assert(static_cast<char*>(p)[i] == 1);
    }

    //3, the first chain do not have enough data, the second chain has enough
    buffer buf3 = buf;
    for(uint32_t i = 0; i < 1024; i++)
    {
        assert(static_cast<const char*>(buf3.get_chains().front().get_buffer())[i] == '\0');
        assert(static_cast<const char*>(buf3.get_chains().back().get_buffer())[i] == '\0');
    }
    ::memset(p, 1, data_len);
    buf3.remove(p, 1024);
    first_chain = &buf3.get_chains().front();
    assert(first_chain->size() == 1013);
    assert(first_chain->get_misalign() == 1);
    assert(buf3.chain_number() == 1);
    for(uint32_t i = 0; i < 1024; i++)
    {
        assert(static_cast<char*>(p)[i] == '\0');
    }

    //4, the total_length == data_len
    buffer buf4 = buf;
    ::memset(p, 1, data_len);
    buf4.remove(p, 1023 + 4 + 1010);
    first_chain = &buf.get_chains().front();
    assert(buf4.chain_number() == 0);
    assert(buf4.get_chains().size() == 0);
    assert(buf4.buffer_length() == 0);

    if(p != 0)
        free(p);
}

void run_tests(){
    test_construct_and_append_buffer();
    test_operator_equal();
    test_append_buffer();
    test_buffer_chain_constructor();
    test_buffer_append_chain();
    test_pullup();
    test_pullup_with_more_chains();
    test_remove();
}
}
