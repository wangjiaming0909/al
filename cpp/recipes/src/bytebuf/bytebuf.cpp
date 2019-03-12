#include "bytebuf.h"
#include <exception>

bytebuf::bytebuf(unsigned int capacity) 
    : capacity_(capacity > MAX_CAPACITY ? MAX_CAPACITY : capacity)
    , writeIndex_(0)
    , readIndex_(0)
    , buf_(capacity_)
{
}

template <typename T>
void bytebuf::read(T* dest)
{
    this->peek(dest);
    readIndex_ += sizeof(T);
}

template <typename T> 
void bytebuf::peek(T* dest)
{
    if(dest == 0) throw std::invalid_argument("Dest is a nullptr...");
    if(this->readablebytes() < sizeof(T)) throw std::out_of_range("Not enough bytes to read...");

    memcpy(dest, &buf_[readIndex_], sizeof(T));
}

template <typename T>
void bytebuf::write(const T& source)
{
    if(this->writableBytes() < sizeof(T)) throw std::out_of_range("Not enough bytes to write...");
    memcpy(&buf_[writeIndex_], &source, sizeof(T));
    writeIndex_ += sizeof(T);
}

void bytebuf::discardReadBytes()
{

}

void bytebuf::clear()
{

}