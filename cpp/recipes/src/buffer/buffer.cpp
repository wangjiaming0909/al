#include "buffer/buffer.h"


buffer_iter:: buffer_iter( buffer* buffer_ptr
    , size_t offset_of_buffer
    , size_t chunk_number
    , size_t offset_of_chunk)
    : buffer_(buffer_ptr)
    , offset_of_buffer_(offset_of_buffer)
    , chunk_number_(chunk_number)
    , offset_of_chunk_(offset_of_chunk)
{

}

buffer_chunk::buffer_chunk(size_t capacity = DEFAULT_CHUNK_SIZE) 
    : capacity_(capacity)
{
    data_ = static_cast<void*>(new char[capacity_]);
    assert(data != nullptr && ("malloc error size:" + capacity_));
    ::memset(data_, 0, capacity_);
}

buffer_chunk::~buffer_chunk()
{
    delete[] data_;
}

buffer_chunk::buffer_chunk(const buffer_chunk& other)
{
    buffer_chunk(other.capacity_);
    ::memcpy(this->data_, other.data_, capacity_);
}

buffer_chunk& buffer_chunk::operator= (const buffer_chunk& other)
{
    
}

buffer::buffer(/* args */)
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

buffer_iter buffer::earch_range(const char* what, size_t len, const Iter* start, const Iter* end)
{

}

buffer_iter buffer::search_eol(const buffer_iter* start, size_t* eol_len_out, buffer_eol_style eol_style)
{

}

int buffer::set_pos(const Iter& pos)
{

}

int buffer::add_pos(size_t position)
{

}

int buffer::peek(size_t len, Iter* start, std::vector<const buffer_iovec*> vec_out)
{

}