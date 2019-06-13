#ifndef _DESIGN_PATTERNS_PROTOTYPE_H_
#define _DESIGN_PATTERNS_PROTOTYPE_H_

#include <string>
using namespace std;

namespace design_patterns
{

struct Address{
    string street;
    string city;
};

struct Contact
{
    string name;
    Address* work_address;
    ~Contact()
    {
        delete work_address;
    }
};

int test_prototype()
{
    Contact john{"John Doe", new Address{"123 East Dr", "London"}};
    Contact jane{"Jane Doe", new Address{"123 East Dr", "London"}};
}

}
#endif //_DESIGN_PATTERNS_PROTOTYPE_H_