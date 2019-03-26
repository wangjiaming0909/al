#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;


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
    auto bind_func1 = std::bind(function1, _1, _2, 2);
    bind_func1(1, "avc");
    string s{"abc"};
    bind_func1(1, std::move(s));

    int j = 0;
    auto bind_func2 = std::bind(function2, _1, _2, j);
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
}