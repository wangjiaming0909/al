#include <iostream>
#include "bytebuf/bytebuf.h"
#include "cassert"
#include <cstring>

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    bytebuf buf{};
    assert(buf.capacity() == bytebuf::DEFAULT_CAPACITY && "capacity should be right");
    buf.write('c');
    assert(buf.readIndex() == 0 && "readIndex should be right");
    assert(buf.writeIndex() == 1);
    assert(buf.readableBytes() == 1);
    assert(buf.writableBytes() == bytebuf::DEFAULT_CAPACITY - 1);

    char c = '\0';
    buf.read(&c);
    assert(c == 'c');
    assert(buf.readIndex() == 1);
    assert(buf.writeIndex() == 1);
    assert(buf.readableBytes() == 0);
    assert(buf.writableBytes() == bytebuf::DEFAULT_CAPACITY - 1);

    buf.write(1);
    assert(buf.readIndex() == 1);
    assert(buf.writeIndex() == 5);
    assert(buf.readableBytes() == 4);
    assert(buf.writableBytes() == bytebuf::DEFAULT_CAPACITY - 5);

    int i = 0;
    buf.read(&i);
    assert(i == 1);
    assert(buf.readIndex() == 5);
    assert(buf.writeIndex() == 5);
    assert(buf.readableBytes() == 0);
    assert(buf.writableBytes() == bytebuf::DEFAULT_CAPACITY - 5);

    buf.write(100.12);
    buf.discardReadBytes();
    assert(buf.readIndex() == 0);
    assert(buf.writeIndex() == 8);
    assert(buf.readableBytes() == 8);
    assert(buf.writableBytes() == bytebuf::DEFAULT_CAPACITY - 8);

    double d = 0;
    buf.peek(&d);
    assert(d == 100.12);
    assert(buf.readIndex() == 0);
    assert(buf.writeIndex() == 8);
    assert(buf.readableBytes() == 8);
    assert(buf.writableBytes() == bytebuf::DEFAULT_CAPACITY - 8);

    const char* str = "HTTP/1.1 / GET\r\nCookie: asdqwdadsasdwdasd";
    unsigned int strLen = strlen(str);
    buf.writeN(str, strLen);
    assert(buf.readIndex() == 0);
    assert(buf.writeIndex() == 8 + strLen);
    assert(buf.readableBytes() == 8 + strLen);
    assert(buf.writableBytes() == bytebuf::DEFAULT_CAPACITY - 8 - strLen);

    int indexOfSlash = -1;
    char target[2] = {'\r', '\n'};
    indexOfSlash = buf.find(target);
    assert(indexOfSlash == 22 && indexOfSlash);

    char target2[4] = {'H', 'T', 'T', 'P'};
    indexOfSlash = buf.find(target2);
    assert(indexOfSlash == 8 && indexOfSlash);

    indexOfSlash = buf.find(100.12);
    assert(indexOfSlash == 0);

    indexOfSlash = buf.find(100);
    assert(indexOfSlash == -1);

    cout << "Hello World!" << endl;
    return 0;
}
