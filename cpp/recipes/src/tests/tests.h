#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <cstdio>
#include <memory>
#include <cstdarg>
#include "boost/range.hpp"
#include "boost/signals2.hpp"
#include "boost/intrusive/slist.hpp"
#include "boost/intrusive/list.hpp"
#include "utils/timer.h"

#ifdef __GNUC__
#if __GNUC__ >= 7
#include <variant>
#endif //__GNUC__
#endif //__GNUC__ >= 7

using namespace std;
using namespace placeholders;

#define FUNC_NAME std::cout << "------" << __func__ << "------" << std::endl;


int function1(int i, string&& s, int j)
{
    cout << i << s << j << endl;;
    return i;
}

int function2(int& i, string& s, int& j)
{
    cout << i << s << j << endl;;
    i = 1;
    j = 1;
    s = "qqq";
    return 0;
}

int function3(string& s, int i)
{
    cout << s << i << endl;
    s = "qqqqqqqqq";
    return 0;
}

void test_bind()
{
    auto bind_func1 = std::bind(function1, std::placeholders::_1, std::placeholders::_2, 2);
    bind_func1(1, "avc");
    string s{"abc"};
    bind_func1(1, std::move(s));

    int j = 0;
    auto bind_func2 = std::bind(function2, std::placeholders::_1, std::placeholders::_2, j);
    int i = 0;
    cout << j << endl;
    cout << i << s << endl;
    bind_func2(i, s);
    cout << i << s << endl;
    cout << j << endl;

    cout << "S: " << s << endl;
    auto bind_func3 = std::bind(function3, s, 1);
    bind_func3();
    cout << "S: " << s << endl;

    auto bind_func4 = std::bind(function3, std::ref(s), 1);
    bind_func4();
    cout << "S: " << s << endl;
    std::vector<int> v{};
    v.push_back(1);
    v.emplace_back(2);
}

// static const char* c_63 = "012345678901234567890123456789012345678901234567890123456789123";
// static const char* c_64 = "0123456789012345678901234567890123456789012345678901234567890123";

void test_vsnprintf(const char* fmt, ...)
{
    char* const p = (char*)::calloc(64, 1);
    ::memset(p, 1, 63);
    va_list v;
    va_start(v, fmt);
    //当传递的字符串因为比63大而被截断时，返回的本因写入的长度（no '\0'）,因此当传入c_64的时候，返回值其实是64,虽然传递给vsnprintf中第二个参数是63
   ::vsnprintf(p, 63, fmt, v);//虽然传递了63作为参数，其实从fmt中只取了62个字符，第63个字符为'\0'
    va_end(v);

    ::free(p);
}

void test_variant()
{
#ifdef __GNUC__
#if __GNUC__ >= 7
    std::variant<int, double> v;
    v = 1.1;
    auto d = std::get<double>(v);
    // auto i = std::get<int>(v);
    cout << d << endl;
#endif // __GNUC__
#endif // __GNUC__ >= 7
}

void test_shared_ptr()
{
    shared_ptr<string> str = make_shared<string>("123");

    weak_ptr<string> w_str = str;
    cout << w_str.expired() << endl;
    cout << "unique: " << str.unique() << endl;
    str.reset();
    cout << w_str.expired() << endl;

    unique_ptr<string> u_ptr = make_unique<string>("123");
}

void test_boost_range()
{

}

void test_boost_signals2()
{
    using namespace std;
    boost::signals2::signal<void(string a)> s{};

    auto con = s.connect([](string a){cout << a << endl;});

    s("we are family;");
}


class TestClass
{
public: 
    TestClass(int)
    {
        cout << "constructor 2" << endl;
    }
    TestClass()
    {
        cout << "constructor" << endl;
    }
    TestClass(const TestClass&)
    {
        cout << "copy constructor" << endl;
    }
    TestClass(TestClass&&)
    {
        cout << "move constructor" << endl;
    }
    ~TestClass()
    {
        // cout << "destructor" << endl;
    }
};

void test_emplace_back()
{
    FUNC_NAME
    std::vector<TestClass> v{};
    // v.push_back(TestClass{});

    v.emplace_back(TestClass{});
    FUNC_NAME
}

void test_emplace_back1()
{
    FUNC_NAME
    std::vector<TestClass> v{};
    v.push_back(TestClass{});

    // v.emplace_back(TestClass{});
    FUNC_NAME
}

void test_emplace_back2()
{
    FUNC_NAME
    std::vector<TestClass> v{};
    v.reserve(10);
    v.push_back(TestClass{});

    v.emplace_back(TestClass{});
    FUNC_NAME

    v.push_back(1);
    v.emplace_back(1);
}

struct Point : boost::intrusive::slist_base_hook<>
{
    Point(size_t size) : size_(size){}
    size_t size_;
};

void test_instrusive()
{
    using namespace boost::intrusive;
    Point p1{1}, p2{2}, p3{3};

    slist<Point> list{};
    // list.push_back(p1);
    list.push_front(p1);
    list.push_front(p2);
    list.push_front(p3);

    for (auto it = list.cbegin(); it != list.cend(); it++)
    {
        cout << it->size_ << endl;
    }
}

template <typename T>
std::unique_ptr<std::vector<T>> initialize_Points(size_t size)
{
    std::unique_ptr<std::vector<T>> ret = std::make_unique<std::vector<T>>();
    for (size_t i = 0; i < size; i++)
    {
        ret->emplace_back(i);
    }
    return ret;
}

struct Point2
{
    Point2(size_t size) : size_(size){}
    size_t size_;
};

void intrusive_benchmark()
{
    size_t size = 100000;
    using namespace boost::intrusive;

    auto points = initialize_Points<Point>(size);
    auto points2 = initialize_Points<Point2>(size);

    {//if the elements have been constructed, using intrusive is faster than std::list
        utils::timer _{"intrusive list"};
        slist<Point> list{};
        for (size_t i = 0; i < size; i++)
        {
            list.push_front(points->operator[](i));
        }
    }

    {
        utils::timer _{"std::list"};
        std::list<Point2*> list{};
        for (size_t i = 0; i < size; i++)
        {
            list.push_back(&points2->operator[](i));
        }
    }
}

void intrusive_benchmark2()
{
    size_t size = 100000;
    using namespace boost::intrusive;

    {//if adding the time of constructing elements, the std list is faster than intrusive list
        utils::timer _{"intrusive list"};
        auto points = initialize_Points<Point>(size);
        slist<Point> list{};
        for (size_t i = 0; i < size; i++)
        {
            list.push_front(points->operator[](i));
        }
    }

    {
        utils::timer _{"std list"};
        std::list<Point2> list{};
        for (size_t i = 0; i < size; i++)
        {
            list.emplace_front(i);
        }
    }
}

struct Node : public boost::intrusive::list_base_hook<>
{
    Node(size_t size) : size_(size){}
    size_t size_;
};

void intrusive_list()
{
    boost::intrusive::list<Node, boost::intrusive::constant_time_size<false>> list;

    Node n1{1}, n2{2};
    list.push_back(n1);
}

void intrusive_using_base_hook_test()
{
    using namespace boost::intrusive;

    class MyTag{ };
    using TagType = tag<MyTag>;
    using list_hook_type = list_base_hook<TagType, link_mode<safe_link>>;
    struct MyNode : public list_hook_type//void* as pointer type
    {
        MyNode(int size) : size_(size){}
        size_t size_;
    };

    MyNode node1{1}, node2{2};
    assert(node1.is_linked() == false);
    assert(node2.is_linked() == false);

    using list_type = boost::intrusive::list<MyNode, base_hook<list_hook_type>, constant_time_size<true>, size_type<std::size_t>>;

    list_type l{};
    assert(l.constant_time_size == true);

    l.push_back(node1);
    assert(node1.is_linked() == true);
    assert(l.size() == 1);
    l.push_back(node2);
    assert(node2.is_linked() == true);
    assert(l.size() == 2);


    for (auto it = l.cbegin(); it != l.cend(); it++)
    {
        cout << it->size_ << endl;
    }

    l.erase(l.begin());
    assert(node1.is_linked() == false);
    assert(l.size() == 1);
    assert(&l.front() == &node2);
}

void intrusive_using_member_hook_test()
{
    using namespace boost::intrusive;


}
