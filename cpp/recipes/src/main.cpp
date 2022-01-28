#include <boost/smart_ptr/shared_ptr.hpp>
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
#include <sstream>
#include "mysql_gtid//mysql_gtid.h"

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
  using namespace gtid;
  boost::shared_ptr<SidMap> m(new SidMap());
  boost::shared_ptr<SidMap> m2(new SidMap());

  Uuid uuid;
  const char* u1 = "aaaaaaaa-f26c-11ea-954d-e454e8b0c44f:100-111, \nbbbbbbbb-f26c-11ea-954d-e454e8b0c44a:100-200";
  const char* u2 = "aaaaaaaa-f26c-11ea-954d-e454e8b0c44f:1-22,  bbbbbbbb-f26c-11ea-954d-e454e8b0c44a:202-300";
  const char* u3 = "aaaaaaaa-f26c-11ea-954d-e454e8b0c44f:23-90, bbbbbbbb-f26c-11ea-954d-e454e8b0c44a:1-99:201:301-400";
  const char* u4 = "aaaaaaaa-f26c-11ea-954d-e454e8b0c44f:90-120";
  const char* u5 = "aaaaaaaa-f26c-11ea-954d-e454e8b0c44f:100-111";
  const char* u6 = "cccccccc-f26c-11ea-954d-e454e8b0c44f:100-111";
  //uuid.parse(u, strlen(u));
  //m->add_sid(uuid);

  GtidSet gtid_set(m);
  GtidSet gtid_set2(m2);
  gtid_set2.add_gtid_text(u5);


  gtid_set.add_gtid_text(u1);
  cout << gtid_set.to_string() << endl;
  gtid_set.add_gtid_text(u2);
  cout << gtid_set.to_string() << endl;
  gtid_set.add_gtid_text(u3);
  cout << gtid_set.to_string() << endl;
  gtid_set.add_gtid_text(u4);
  cout << gtid_set.to_string() << endl;
  gtid_set.add_gtid_text(u5);
  cout << gtid_set.to_string() << endl;

  Sid sid;
  sid.parse(u6, Sid::TEXT_LENGTH);
  Sid sid2;
  sid2.parse(u4, Sid::TEXT_LENGTH);
  Sid sid3;
  sid3.parse("bbbbbbbb-f26c-11ea-954d-e454e8b0c44a", Sid::TEXT_LENGTH);

  cout << "gtid_set2: " << gtid_set2.to_string() << endl;
  gtid_set.remove_gtid_set(gtid_set2);
  cout << gtid_set.to_string() << endl;
  //gtid_set.add_gtid_text(u6);
  cout << gtid_set.to_string() << endl;
  cout << "gno count for sid: " << sid.to_string() << ": " << gtid_set.get_gno_count(sid) << endl;;
  cout << "gno count for sid: " << sid2.to_string() << ": " << gtid_set.get_gno_count(sid2) << endl;;
  cout << "gno count for sid: " << sid3.to_string() << ": " << gtid_set.get_gno_count(sid3) << endl;;
  GtidSet t(boost::shared_ptr<SidMap>(new SidMap));
  t.add_gtid_text(u6);
  gtid_set.remove_gtid_set(t);
  cout << gtid_set.to_string() << endl;
  boost::shared_ptr<SidMap> m3(new SidMap);
  GtidSet gtid_set3(m3);
  gtid_set.clear();
  gtid_set2.clear();
  gtid_set3.clear();
  return 0;
  while (1) {
    string cmd;
    string s;
    cout << "-----------------------------\n";
    s.clear();
    cin >> cmd;
    string tmp;
    cin.peek();
    while (cin.rdbuf()->in_avail() > 0) {
      cin >> tmp;
      s.append(tmp);
    }
    if (s == "q") {
      break;;
    } else if (GtidSet::is_valid(s.c_str())) {
      cout  << "before: " << gtid_set3.to_string() << endl;
      const char* c = s.c_str();
      if (cmd == "a") {
        gtid_set3.add_gtid_text(c);
      } else if (cmd == "d"){
        GtidSet tmp_set(boost::shared_ptr<SidMap>(new SidMap));
        tmp_set.add_gtid_text(c);
        gtid_set3.remove_gtid_set(tmp_set);
      }
      cout  << "after: " << gtid_set3.to_string() << endl;
    } else {
      cout << "invalid gtid";
    }
  }
  return 0;
}
