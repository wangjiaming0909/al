#include "buffer/buffer.h"


buffer_iter::buffer_iter( buffer* buffer_ptr
    , buffer_chain* chunk
    , size_t offset_of_buffer
    , size_t chunk_number
    , size_t offset_of_chunk)
    : buffer_(buffer_ptr)
    , chunk_(chunk)
    , offset_of_buffer_(offset_of_buffer)
    , chunk_number_(chunk_number)
    , offset_of_chunk_(offset_of_chunk)
{

}

buffer_chain::buffer_chain(size_t capacity) : buffer_(0), next_(0)
{
    if(capacity <= 0) 
    {
        LOG(WARNING) << "capacity_ is nonpositive";
        capacity_ = DEFAULT_CHUNK_SIZE;
    }
    else
    {
        capacity_ = this->calculate_actual_capacity(capacity);
    }

    buffer_ = static_cast<void*>(new char[capacity_]);
    assert(buffer_ != nullptr && ("new operator error size:" + capacity_));
}

buffer_chain::~buffer_chain()
{
    delete[] buffer_;
}

buffer_chain::buffer_chain(const buffer_chain& other)
{
    this->capacity_ = other.capacity_;
    this->buffer_ = static_cast<void*>(new char[capacity_]);
    assert(this->buffer_ != nullptr && ("new operator error size: " + this->capacity_));

    ::memcpy(this->buffer_, other.buffer_, this->capacity_);
    this->next_ = other.next_;
}

buffer_chain& buffer_chain::operator= (const buffer_chain& other)
{
    if(this->capacity_ < other.capacity_)
    {
        delete[] this->buffer_;
        this->capacity_ = other.capacity_;
        this->buffer_ = static_cast<void*>(new char[capacity_]);
    }

    ::memcpy(this->buffer_, other.buffer_, other.capacity_);
    this->next_ = other.next_;
}

size_t buffer_chain::calculate_actual_capacity(size_t given_capacity)
{
    size_t to_alloc = 0;
    if(given_capacity < MAXIMUM_CHUNK_SIZE / 2)
    {
        to_alloc = DEFAULT_CHUNK_SIZE;
        while(to_alloc < given_capacity){
            to_alloc <<= 1;
        }
    }
    else
    {
        to_alloc = given_capacity;
    }
    return to_alloc;
}

buffer::buffer()
{
}

buffer::~buffer()
{
}

buffer::buffer(const buffer& other, size_t dataLen)
{

}

buffer::buffer(const buffer& other, size_t dataLen, Iter* start)
{

}

buffer& buffer::operator=(const buffer& other)
{

}

size_t buffer::buffer_length()
{

}

size_t buffer::first_chunk_length()
{

}


int buffer::append(const buffer& other, size_t dataLen)
{

}
int buffer::append(const buffer& other, size_t dataLen, Iter* start)
{

}
int buffer::append_printf(const char* fmt, ...)
{

}
int buffer::append_vprintf(const char* fmt, va_list ap)
{

}

int buffer::prepend(const buffer& other, size_t dataLen)
{

}

int buffer::prepend(const buffer& other, size_t dataLen, Iter* start)
{

}

int buffer::expand(size_t dataLen)
{

}

unsigned char* buffer::pullup(size_t size)
{

}

int buffer::remove(/*out*/void* data, size_t dataLen)
{

}

int buffer::drain(size_t len)
{

}

int buffer::copy_out_from(void* data, size_t dataLen, Iter* start)
{

}

char* buffer::read_line(size_t *n_read_out, buffer_eol_style eol_style)
{

}

buffer_iter buffer::search(const char* what, size_t len, const Iter* start)
{

}

buffer_iter buffer::search_range(const char* what, size_t len, const Iter* start, const Iter* end)
{

}

buffer_iter buffer::search_eol(const buffer_iter* start, size_t* eol_len_out, buffer_eol_style eol_style)
{

}

int buffer::peek(size_t len, Iter* start, std::vector<const buffer_iovec*> vec_out)
{

}