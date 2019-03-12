#include <iostream>
#include "bytebuf/bytebuf.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    bytebuf buf{};
    cout << buf.capacity() << endl;
    return 0;
}
