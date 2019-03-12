#include "bytebuf/bytebuf.h"
#include <exception>
#include "boost/assert.hpp"

bytebuf::bytebuf(unsigned int capacity) 
    : capacity_(capacity > MAX_CAPACITY ? MAX_CAPACITY : capacity)
    , writeIndex_(0)
    , readIndex_(0)
    , buf_(capacity_)
{
}

//copy the readable bytes to the front
void bytebuf::discardReadBytes()
{
    unsigned int sizeOfTmpBuf = this->readableBytes();
    if(sizeOfTmpBuf <= 0) return;
    char tmpBuf[sizeOfTmpBuf] = {};
    memcpy(tmpBuf, &buf_[readIndex_], sizeOfTmpBuf);
    memcpy(&buf_[0], tmpBuf, sizeOfTmpBuf);
    readIndex_ = 0;
    writeIndex_ = sizeOfTmpBuf;
}

void bytebuf::clear()
{
    readIndex_ = 0;
    writeIndex_ = 0;
}

void bytebuf::readN(void* dest, unsigned int n)
{
    this->peekN(dest, n);
    readIndex_ += n;
}

void bytebuf::peekN(void* dest, unsigned int n)
{
    if(dest == 0) throw std::invalid_argument("Dest is a nullptr...");
    if(n > this->readableBytes()) throw std::out_of_range("Not enough bytes to read...");
    memcpy(dest, &buf_[readIndex_], n);
}

void bytebuf::writeN(const void* source, unsigned int n)
{
    if(this->writableBytes() < n) throw std::out_of_range("Not enough bytes to write...");
    memcpy(&buf_[writeIndex_], source, n);
    writeIndex_ += n;
}