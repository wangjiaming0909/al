#include <iostream>
// #include "buffer/test.h"
// #include "bytebuf/test.h"
#include "tests/tests.h"
// #include "guid_map/test.h"
// #include "design_patterns/basic/SOLID_Open_Close.h"
// #include "design_patterns/basic/maybemonad.h"
// #include "design_patterns/basic/thread_safe_singleton.h"
// // #include "easylogging/test_easylogging.h"
// #include "http_parser/test.h"
// #include "string_piece/test.h"
// #include "substring_search/kmp_test.h"
// #include "design_patterns/builder/builder.h"
// #include "design_patterns/builder/groovy_builder.h"
// #include "design_patterns/factory/factory_method.h"
// #include "design_patterns/chain_of_responsibility/pointer_chain.h"
// #include "design_patterns/chain_of_responsibility/broke_chain.h"
// #include "design_patterns/prototype/prototype.h"
// #include "gperf/test.h"

#include "al/merge.h"
#include <mysql/mysql.h>
#include <boost/format.hpp>
#include <cstdint>
#include <exception>

using namespace std;

// INITIALIZE_EASYLOGGINGPP
//

int test_mysql()
{
  static MYSQL* mysql = NULL;
  mysql = mysql_init(mysql);

  mysql_real_connect(mysql, "127.0.0.1", "root", "", "test", 3307, NULL, 0);

  mysql_close(mysql);
}


int test_boost_format()
{
  std::cout << UINT64_MAX << std::endl;
  std::cout << (boost::format("%lu %s %s") % UINT64_MAX % "asd").str() << std::endl;
}

int main()
{
//    bytebuf_test::run_tests();
//    buffer_test::run_tests();
// //    test_bind();

// //    test_vsnprintf("%s", c_63);
// //    test_vsnprintf("%s", c_64);

//    guid_map_test::run_tests();

//    design_patterns::OC_test();
//    design_patterns::Monad_test();
//     design_patterns::thread_safe_singleton_test();
//     // easylogging::test();

//    test_shared_ptr();
//    test_variant();

//    http_parser_test::test();
//    http_parser_test::test_traits();
//    http_parser_test::test_URLParser();

//    string_piece_test::test();
//    kmp_test::test_kmp();
//    kmp_test::test_kmp_search();
//    kmp_test::test_kmp_search_without_0_end();

//    design_patterns::test_build();
//    design_patterns::test_groovy_builder();
//    design_patterns::test_factory_method();
//    design_patterns::test_pointer_chain();
//    design_patterns::test_broke_chain();
//    design_patterns::test_prototype();

//    test_boost_signals2();

//    gperf::test_gperf();
//    gperf::test_all();

    // test_emplace_back();
    // test_emplace_back1();
    // test_emplace_back2();

    // test_intrusive();
    // intrusive_benchmark2();
    // intrusive_using_base_hook_test();
// test_nestedClass();

 //al::test_mergeMultiSortedArray();
    //test_HHWheelTimer();

//test_boost_format();
cout << "Hello World!" << endl;
try {
  std::string s = 0;
} catch (std::exception& e) {
  cout << "got exception " << e.what() << endl;
}
return 0;
}
