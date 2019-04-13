#include "buffer/buffer.h"
#include <cassert>
#include <string> 
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

namespace buffer_test{

#define DUMMY_CLASS_SIZE 1024
class dummy_class
{
public:
    dummy_class()
    {
        std::srand(std::time(nullptr));
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

template <unsigned int N>
struct SizableClass_WithData{
    SizableClass_WithData()
    {
        std::srand(std::time(nullptr));
        memset(buffer_, std::rand(), N);
    }
    char buffer_[N];
};


static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> dis(33, 126);

template <unsigned int N>
struct SizableClass_WithChar{
    SizableClass_WithChar()
    {
        for(uint32_t i = 0; i < N; i++)
        {
            buffer_[i] = dis(gen);
        }
    }
    char buffer_[N];
};

void test_buffer_chain_constructor()
{
    buffer buf1{};
    buf1.append(SizableClass<64>());
    const buffer_chain& chain1 = buf1.get_chains().front();
    buffer_chain chain2{chain1};
    assert(chain2.next() == nullptr);
    assert(chain2.size() == 64);
    assert(chain2.get_offset() == 64);
    int64_t ret = chain2.set_misalign(4);
    assert(ret == 4);
    assert(chain2.size() == 64 - 4);

    //copy constructor

    // buffer_chain chain3{chain2, 16, chain2.begin()};
    // assert(chain3.get_misalign() == 4);
    // assert(chain3.size() == 64 - 4);
    //expceted exception
    try {//iter不是chain2的iter
        buffer_chain chain4{chain2, 16, chain1.begin()};
    }catch(std::exception&){}

    try {//iter达到末尾了,没有可以复制的内容了
        buffer_chain chain4{chain2, 16, chain2.end() + 1};
    }catch(std::exception&){}
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
    assert(chains.back().next() == nullptr);

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
    assert(buf5.get_chains().back().next() == nullptr);
    assert(static_cast<double>(first_chain_of_buf5.get_offset()) > (first_chain_of_buf5.chain_capacity() * 7 / 8.0));
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
    assert(next_chain == nullptr);
    assert(first_chain->chain_capacity() == default_size);
    assert(buf1.buffer_length() == default_size - 1);
    assert(buf1.chain_number() == 1);
    assert(buf1.last_chain_with_data() == first_chain);
    buf1.append(1);
    next_chain = first_chain->next();
    assert(buf1.buffer_length() == default_size - 1 + sizeof (int));
    assert(buf1.chain_number() == 2);
    assert(next_chain != nullptr);
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
    assert(next_chain == nullptr);
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
    assert(next_chain == nullptr);
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

    if(p != nullptr)
        free(p);
}

void test_copy_out_from()
{
    auto s1 = SizableClass_WithData<1023>();
    auto s2 = SizableClass_WithData<4>();
    auto s3 = SizableClass_WithData<1010>();
    buffer buf{};
    buf.append(s1);
    buf.append(s2);
    buf.append(s3);

    //第一个chain足够拷贝出来
    buffer buf1 = buf;
    uint32_t size_of_p = 4096;
    char* p = static_cast<char*>(::calloc(size_of_p, 1));
    memset(p, 1, size_of_p);
    //no offset
    buf1.copy_out_from(p, 100, buf1.begin());
    assert(memcmp(p, s1.buffer_, 100) == 0);
    //with offset of 10
    memset(p, 1, size_of_p);
    buf1.copy_out_from(p, 100, buf1.begin() + 10);
    assert(memcmp(p, s1.buffer_ + 10, 100) == 0);

    //用到die第二个chain中的数据
    buffer buf2 = buf;
    memset(p, 1, size_of_p);
    //no offset
    int64_t ret = buf2.copy_out_from(p, 1040, buf2.begin());
    assert(ret > 0);
    assert(memcmp(p, s1.buffer_, 1023) == 0);
    assert(memcmp(p + 1023, s2.buffer_, 4) == 0);
    assert(memcmp(p + 1023 + 4, s3.buffer_, 1040 - 1023 - 4) == 0);
    //with offset

    memset(p, 1, size_of_p);
    assert(buf2.buffer_length() == 1023 + 4 + 1010);
    ret = buf2.copy_out_from(p, 1040, buf2.begin() + 100);
    assert(ret == 1040);
    assert(memcmp(p, s1.buffer_ + 100, 1023 - 100) == 0);
    assert(memcmp(p + 1023 - 100, s2.buffer_, 4) == 0);
    assert(memcmp(p + 1023 - 100 + 4, s3.buffer_, 1040 - (1023 - 100) - 4) == 0);
}

void test_buffer_search_range()
{
    SizableClass_WithChar<1023> s1{};
    SizableClass_WithChar<4> s2{};
    SizableClass_WithChar<1010> s3{};
    buffer buf{};
    buf.append(s1);
    buf.append(s2);
    buf.append(s3);

    uint32_t data_size = 1024;
    uint32_t target_len = 2;
    char* data = static_cast<char*>(::calloc(data_size, 1));
    ::memset(data, 0, data_size);
    ::memcpy(data, s1.buffer_ + 100, target_len);

    auto it = buf.search_range(data, target_len, buf.begin(), buf.end());
    assert(it.chain() == buf.begin().chain());
    assert(it.offset() == 100);

    //cross two chains
    ::memset(data, 0, data_size);
    target_len = 100;
    ::memcpy(data, s1.buffer_ + 1000, 1023 - 1000);
    ::memcpy(data + 23, s2.buffer_, 4);
    ::memcpy(data + 23 + 4, s3.buffer_, 100 - 23 - 4);
    auto it2 = buf.search_range(data, target_len, buf.begin(), buf.end());
    assert(it.chain() == buf.begin().chain());
    assert(it2.offset() == 1000);
}

void test_buffer_memcmp()
{
    buffer buf{};
    SizableClass_WithChar<1023> s1{};
    SizableClass_WithChar<8> s2{};
    SizableClass_WithChar<1010> s3{};
    buf.append(s1);
    buf.append(s2);
    buf.append(s3);

    uint32_t data_size = 4096;
    char* data_to_compare = static_cast<char*>(::calloc(data_size, 1));
    //1023/1024, 1018/1024
    //only compare the first chain
    ::memcpy(data_to_compare, s1.buffer_, 1023);
    bool ret = buf.buffer_memcmp(data_to_compare, 1023, buf.begin());
    assert(ret == true);

    //only compare the first chain but not from first byte
    ::memset(data_to_compare, 0, data_size);
    ::memcpy(data_to_compare, s1.buffer_ + 100, 923);
    ret = buf.buffer_memcmp(data_to_compare, 923, buf.begin() + 100);
    assert(ret == true);

    //compare two chains, from start
    ::memset(data_to_compare, 0, data_size);
    ::memcpy(data_to_compare, s1.buffer_, 1023);
    ::memcpy(data_to_compare + 1023, s2.buffer_, 8);
    ret = buf.buffer_memcmp(data_to_compare, 1023 + 8, buf.begin());
    assert(ret == true);

    //compare all bytes
    ::memset(data_to_compare, 0, data_size);
    ::memcpy(data_to_compare, s1.buffer_, 1023);
    ::memcpy(data_to_compare + 1023, s2.buffer_, 8);
    ::memcpy(data_to_compare + 1023 + 8, s3.buffer_, 1010);
    ret = buf.buffer_memcmp(data_to_compare, 1023 + 8 + 1010, buf.begin());
    assert(ret == true);

    //compare all bytes not from first byte
    ::memset(data_to_compare, 0, data_size);
    ::memcpy(data_to_compare, s1.buffer_ + 64, 1023 - 64);
    ::memcpy(data_to_compare + 1023 - 64, s2.buffer_, 8);
    ::memcpy(data_to_compare + 1023 + 8 - 64, s3.buffer_, 1010);
    ret = buf.buffer_memcmp(data_to_compare, 1023 + 8 + 1010 - 64, buf.begin() + 64);
    assert(ret == true);

    //compare the second chain, from the first byte of the second chain
    ::memset(data_to_compare, 0, data_size);
    ::memcpy(data_to_compare, s2.buffer_, 8);
    ::memcpy(data_to_compare + 8, s3.buffer_, 1010);
    ret = buf.buffer_memcmp(data_to_compare, 8 + 1010, buf.begin() + 1023);
    assert(ret == true);

    //compare the second chain, not from the first byte of the second chain
    ::memset(data_to_compare, 0, data_size);
    ::memcpy(data_to_compare, s2.buffer_ + 2, 6);
    ::memcpy(data_to_compare + 6, s3.buffer_, 1010);
    ret = buf.buffer_memcmp(data_to_compare, 6 + 1010, buf.begin() + (1023 + 2));
    assert(ret == true);
}

void test_buffer_iter(){
    auto it_NUL = buffer_iter::NULL_ITER;
    buffer buf{};
    buf.append(4);
    auto it_begin = buf.begin();
    auto it_end = buf.end();
    assert(!(it_NUL > it_begin));
    assert(!(it_NUL > it_end));
    assert(it_begin < it_end);
    assert((it_begin + sizeof (4)) == it_end);
}

void test_buffer_search_eol()
{
    SizableClass_WithData<1023> s1{};
    SizableClass_WithData<4> s2{};
    SizableClass_WithData<1010> s3{};

    const char* CRLF = "\r\n";
//    const char* LF = "\n";
    ::memcpy(s1.buffer_ + 100, CRLF, 2);

    buffer buf{};
    buf.append(s1);
    buf.append(s2);
    buf.append(s3);

    uint32_t len = 0;
    auto it = buf.search_eol(&len, buffer_eol_style::BUFFER_EOL_CRLF_STRICT, buf.begin());
    assert(it.chain().get_buffer() == buf.get_chains().front().get_buffer());
    assert(it.chain().size() == buf.get_chains().front().size());
    assert(it.offset() == 100);
    assert(len == 2);
}

void test_buffer_read_line()
{
    SizableClass_WithData<1023> s1{};
    SizableClass_WithData<4> s2{};
    SizableClass_WithData<1010> s3{};

    const char* CRLF = "\r\n";
//    const char* LF = "\n";
    uint32_t pos_of_CR = 100;
    ::memcpy(s1.buffer_ + pos_of_CR, CRLF, 2);

    buffer buf{};
    buf.append(s1);
    buf.append(s2);
    buf.append(s3);

    uint32_t data_size = 4096;
    char* data = static_cast<char*>(::calloc(data_size, 1));
    auto read_size = buf.read_line(data, 4096, buffer_eol_style::BUFFER_EOL_CRLF_STRICT);
    assert(read_size == pos_of_CR);

    assert(memcmp(s1.buffer_, data, read_size) == 0);

    free(data);
}

void test_buffer_append_printf()
{
    //the last_chain_with_data have enough space for formatted string
    buffer buf;
    buf.append(SizableClass_WithChar<64>());
    const char* fmt = "%s";
    const char* c1 = "0123456789";

    buf.append_printf(fmt, c1);
    assert(buf.chain_number() == 1);
    assert(buf.buffer_length() == 64 + 10);
    const buffer_chain* last_chain = buf.last_chain_with_data();
    const char* p = static_cast<const char*>(last_chain->get_buffer()) + last_chain->get_misalign() + 64;
    assert(memcmp(p, c1, 10) == 0);
    assert(last_chain->get_offset() == 64 + 10);
    assert(buf.total_len() == 64 + 10);

    //have enough space for the formatted string after expand at the first time
    buffer buf1;
    buf1.append(SizableClass_WithChar<1020>());

    int64_t ret = buf1.append_printf(fmt, c1);
    assert(ret == 10);
    assert(buf1.chain_number() == 2);
    assert(buf1.total_len() == 1020 + 10);
    assert(buf1.last_chain_with_data()->get_offset() == 10);
    last_chain = buf1.last_chain_with_data();
    p = static_cast<const char*>(last_chain->get_buffer()) + last_chain->get_misalign();
    assert(memcmp(p, c1, 10) == 0);

    //the first time, donot have enugh space for formatted string, after expand should have enough space
    buffer buf2;
    buf2.append(SizableClass_WithChar<900>());

    char* c2 = static_cast<char*>(::calloc(256, 1));
    ::memset(c2, 1, 256);
    ret = buf2.append_printf(fmt, c2);
    assert(ret == 256);
    assert(buf2.chain_number() == 2);
    assert(buf2.total_len() == 900 + 256);

}

void run_tests()
{
    test_construct_and_append_buffer();
    test_operator_equal();
    test_append_buffer();
    test_buffer_chain_constructor();
    test_buffer_append_chain();
    test_pullup();
    test_pullup_with_more_chains();
    test_remove();
    test_copy_out_from();
    test_buffer_memcmp();
    test_buffer_search_range();
    test_buffer_iter();
    test_buffer_search_eol();
    test_buffer_read_line();
    test_buffer_append_printf();
}

} //namespace buffer_test
