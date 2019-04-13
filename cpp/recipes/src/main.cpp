#include <iostream>
#include "buffer/test.h"
#include "bytebuf/test.h"
#include "tests/tests.h"

using namespace std;

int main()
{
    // bytebuf_test::run_tests();
    // buffer_test::run_tests();
    // test_bind();

    test_vsnprintf("%s", c_63);
    test_vsnprintf("%s", c_64);

    cout << "Hello World!" << endl;
    return 0;
}
