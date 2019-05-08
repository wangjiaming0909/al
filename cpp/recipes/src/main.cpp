#include <iostream>
#include "buffer/test.h"
#include "bytebuf/test.h"
#include "tests/tests.h"
#include "guid_map/test.h"
#include "design_patterns/SOLID_Open_Close.h"
#include "design_patterns/maybemonad.h"
#include "design_patterns/thread_safe_singleton.h"
#include "easylogging/test_easylogging.h"

using namespace std;

INITIALIZE_EASYLOGGINGPP

int main()
{
    // bytebuf_test::run_tests();
//     buffer_test::run_tests();
    // test_bind();

//    test_vsnprintf("%s", c_63);
//    test_vsnprintf("%s", c_64);

    // guid_map_test::run_tests();

//    design_patterns::OC_test();
    // design_patterns::Monad_test();
    // design_patterns::thread_safe_singleton_test();
    easylogging::test();

    cout << "Hello World!" << endl;
    return 0;
}
