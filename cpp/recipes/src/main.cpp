#include <iostream>
#include "buffer/test.h"
#include "bytebuf/test.h"

using namespace std;

int main()
{
    // bytebuf_test::run_tests();
    buffer_test::run_tests();
    cout << "Hello World!" << endl;
    return 0;
}
