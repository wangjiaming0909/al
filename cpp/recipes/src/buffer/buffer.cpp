#include "buffer/buffer.h"


buffer_iter::buffer_iter( buffer* buffer_ptr
    , buffer_chain* chain
    , size_t offset_of_buffer
    , size_t chain_number
    , size_t offset_of_chain)
    : buffer_(buffer_ptr)
    , chain_(chain)
    , offset_of_buffer_(offset_of_buffer)
    , chain_number_(chain_number_)
    , offset_of_chain_(offset_of_chain_)
{

}

//TODO UT
buffer_iter& buffer_iter::operator+(size_t forward_steps)
{
    size_t maximum_steps_can_forward = buffer_->total_len() - this->offset_of_buffer_;
    if(forward_steps > maximum_steps_can_forward)
    {
        chain_ = buffer_->last_chain_with_data();
        offset_of_chain_ = chain_->get_offset();
        offset_of_buffer_ = buffer_->total_len();
        chain_number_ = 0;
        return *this;
    }

    size_t forward_chain_numbers = -1;
    size_t steps_can_forward_in_current_chain = chain_->get_offset() - this->offset_of_chain_;
    assert(steps_can_forward_in_current_chain >= 0 && "offset of a chain should > offset_of_chain in buffer_iter");
    size_t remain_steps = forward_steps;

    //current is not the last chain_with_data
    while(!buffer_->is_last_chain_with_data(chain_) && remain_steps > steps_can_forward_in_current_chain)
    {
        remain_steps -= steps_can_forward_in_current_chain;
        chain_ = chain_->next();
        chain_number_++;
        this->offset_of_chain_ = 0;
    }
    //last chain_with_data or current chain can cover the remain_steps
    //note that current chain could be the original chain(means that chain_ didn't changed at all)
    offset_of_chain_ = this->offset_of_chain_ + remain_steps;
    offset_of_buffer_ += forward_steps;
    return *this;
}

buffer_chain::buffer_chain(size_t capacity) 
    : buffer_(0)
    , next_(0)
    , off_(0)
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
    this->off_ = other.off_;
}

buffer_chain::buffer_chain(const buffer_chain& other, size_t data_len)
{
    if(data_len > other.get_offset())
    {
        buffer_chain{other};
        return;
    }

    buffer_chain{data_len};
    memcpy(this->buffer_, other.buffer(), data_len);
    this->buffer_ = other.buffer_;
    this->next_ = other.next_;
    this->off_ = data_len;
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
    this->off_ = other.off_;
}

int buffer_chain::set_offset(size_t offset)
{
    if(offset < 0 || offset > capacity_) return -1;
    off_ = offset;
    return 0;
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
    : chains_()
    , last_chain_with_data_(0)
    , total_len_(0)
{

}

buffer::buffer(const buffer& other)
{
    this->chains_ = other.chains_;
    this->last_chain_with_data_ = other.last_chain_with_data_;
    this->total_len_ = other.total_len_;
}

buffer::buffer(const buffer& other, size_t data_len) 
{
    if(other.total_len_ == 0 || data_len <= 0) 
    {
        buffer{};
        return;
    }

    //copy all
    if(data_len >= other.total_len())
    {
        buffer{other};
        return;
    }

    //partially copy
    size_t remain_to_copy = data_len;
    const buffer_chain* current_chain = &other.first();

    while(!this->is_last_chain_with_data(current_chain) && current_chain->get_offset() < remain_to_copy)
    {
        assert(current_chain->chain_capacity() == current_chain->get_offset() && "current chain should be full");
        chains_.push_back(*current_chain);
        remain_to_copy -= current_chain->chain_capacity();
        current_chain = current_chain->next();
    }

    //get to the last_chain_with_data or current_chain can cover remain_to_copy
    //even it is the last_chain_with_data, it can also cover the remain_to_copy
    buffer_chain last_chain{*current_chain, remain_to_copy};
    chains_.push_back(last_chain);
    last_chain_with_data_ = &chains_.back();
    total_len_ = data_len;
}

buffer::buffer(const buffer& other, size_t data_len, Iter* start)
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


int buffer::append(const buffer& other, size_t data_len)
{

}
int buffer::append(const buffer& other, size_t data_len, Iter* start)
{

}
int buffer::append_printf(const char* fmt, ...)
{

}
int buffer::append_vprintf(const char* fmt, va_list ap)
{

}

int buffer::prepend(const buffer& other, size_t data_len)
{

}

int buffer::prepend(const buffer& other, size_t data_len, Iter* start)
{

}

int buffer::expand(size_t data_len)
{

}

unsigned char* buffer::pullup(size_t size)
{

}

int buffer::remove(/*out*/void* data, size_t data_len)
{

}

int buffer::drain(size_t len)
{

}

int buffer::copy_out_from(void* data, size_t data_len, Iter* start)
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

inline bool buffer::is_last_chain_with_data(const buffer_chain* current_chain) const
{
    if(current_chain == 0 || current_chain != last_chain_with_data_)
        return false;
    return true;
}