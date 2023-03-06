#ifndef _DESIGN_PATTERNS_PROTOTYPE_H_
#define _DESIGN_PATTERNS_PROTOTYPE_H_

#include <string>
#include <iostream>
#include "boost/serialization/serialization.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
using namespace std;

namespace design_patterns {

  struct WorkAddress{
    string street;
    string city;
    int suite;

    WorkAddress(const string& street, const string& city, int suite)
      : street(street), city(city), suite(suite){}

    WorkAddress(const WorkAddress& addr)
    {
      street = addr.street;
      city = addr.city;
      suite = addr.suite;
    }

    friend std::ostream& operator<< (std::ostream& os, const WorkAddress& addr)
    {
      return os << "street: " << addr.street
        << " city: " << addr.city
        << " suite: " << addr.suite;
    }
    private:
    friend class boost::serialization::access;
    template <typename Ar>
      void serialize(Ar& ar, unsigned int version) {
        ar & street & city & suite;
      }
  };

  struct Contact
  {
    Contact(const string& name, WorkAddress* workAddress)
      : name(name), work_address(new WorkAddress(*workAddress))
    {
    }

    Contact(const Contact &obj)
      : name(obj.name), work_address(new WorkAddress(*obj.work_address))
    {}

    string name;
    WorkAddress* work_address;
    ~Contact() {
      delete work_address;
    }

    friend ostream& operator<< (ostream& os, const Contact& obj)
    {
      return os << "name: " << obj.name
        << " work_address: " << *obj.work_address;
    }
  };

  struct EmployeeFactory {
    static unique_ptr<Contact> new_main_employee(const string& name, int suite) {
      static Contact employee{"", new WorkAddress{"123 East Dr", "London", 0}};
      auto ret = make_unique<Contact>(employee);
      ret->name = name;
      ret->work_address->suite = suite;
      return ret;
    }
    public:
    static unique_ptr<Contact> new_main_employee2(const string&name, int suite) {
      return new_employee(name, suite, main);
    }
    static unique_ptr<Contact> new_aux_employee(const string& name, int suite) {
      return new_employee(name, suite, aux);
    }
    private:
    static Contact main, aux;
    static unique_ptr<Contact> new_employee(const string& name, int suite, Contact& prototype) {
      auto ret = make_unique<Contact>(prototype);
      ret->name = name;
      ret->work_address->suite = suite;
      return ret;
    }
  };
  Contact EmployeeFactory::main{"", new WorkAddress{"123 easdasd", "main", 0}};
  Contact EmployeeFactory::aux{"", new WorkAddress{"456 easdasd", "aux", 0}};

  inline int test_prototype()
  {
    Contact employee{"", new WorkAddress{"123 East Dr", "London", 0}};
    Contact john{employee};
    john.name = "john";
    john.work_address->suite = 100;

    Contact jane{employee};
    jane.name = "jane";
    jane.work_address->suite = 101;
    cout << john << endl;
    cout << jane << endl;

    auto john2 = EmployeeFactory::new_main_employee("john2", 1);

    auto john3 = EmployeeFactory::new_main_employee2("john3", 101);
    auto john4 = EmployeeFactory::new_aux_employee("john4", 102);

    cout << *john2 << endl << *john3 << endl << *john4<< endl;

    // auto clone = [](Contact c)
    // {
    //     ostringstream oss;
    //     boost::archive::text_oarchive oa(oss);
    //     // oa << c;
    // };
    ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << john3->work_address;
    cout << oss.str() << endl;
    return 0;
  }
}
#endif //_DESIGN_PATTERNS_PROTOTYPE_H_
