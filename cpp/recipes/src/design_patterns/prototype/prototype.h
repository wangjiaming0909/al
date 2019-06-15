#ifndef _DESIGN_PATTERNS_PROTOTYPE_H_
#define _DESIGN_PATTERNS_PROTOTYPE_H_

#include <string>
#include <iostream>
#include "boost/serialization/serialization.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
using namespace std;

namespace design_patterns
{

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
    ~Contact()
    {
        delete work_address;
    }

    friend ostream& operator<< (ostream& os, const Contact& obj)
    {
        return os << "name: " << obj.name
                  << " work_address: " << *obj.work_address;
    }
};

int test_prototype()
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

    // auto clone = [](Contact c)
    // {
    //     ostringstream oss;
    //     boost::archive::text_oarchive oa(oss);
    //     // oa << c;
    // };
    return 0;
}
}
#endif //_DESIGN_PATTERNS_PROTOTYPE_H_