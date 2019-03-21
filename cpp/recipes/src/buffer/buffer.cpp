#include "buffer/buffer.h"


buffer_iter::buffer_iter( buffer* buffer_ptr
    , buffer_chain* chain
    , size_t offset_of_buffer
    , size_t chain_number
    , size_t offset_of_chain)
    : buffer_(buffer_ptr)
    , chain_(chain)
    , offset_of_buffer_(offset_of_buffer)
    , chain_number_(chain_number)
    , offset_of_chain_(offset_of_chain)
{

}

const buffer_iter buffer_iter::NULL_ITER = buffer_iter{0, 0, 0, 0, 0};

//TODO UT
buffer_iter& buffer_iter::operator+(size_t forward_steps)
{
    if(forward_steps == 0) return *this;

    size_t maximum_steps_can_forward = buffer_->total_len() - this->offset_of_buffer_;
    if(forward_steps > maximum_steps_can_forward)
    {
        chain_ = buffer_->last_chain_with_data();
        offset_of_chain_ = chain_->get_offset();
        offset_of_buffer_ = buffer_->total_len();
        chain_number_ = 0;//!
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

buffer_chain::buffer_chain(buffer* parent, size_t capacity) 
    : buffer_(0)
    , off_(0)
    , next_(0)
    , parent_(parent)
{
    if(capacity == 0) 
    {
        LOG(WARNING) << "capacity_ is 0";
        capacity_ = DEFAULT_CHAIN_SIZE;
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

buffer_chain::buffer_chain(const buffer_chain&& other)
{

}

buffer_chain::buffer_chain(const buffer_chain& other)
{
    this->capacity_ = other.capacity_;
    this->buffer_ = static_cast<void*>(new char[capacity_]);
    assert(this->buffer_ != nullptr && ("new operator error size: " + this->capacity_));

    ::memcpy(this->buffer_, other.buffer_, this->capacity_);
    this->next_ = other.next_;
    this->off_ = other.off_;
    this->parent_ = other.parent_;
}

buffer_chain::buffer_chain(const buffer_chain& other, size_t data_len, const Iter* start)
{
    if(data_len > (other.off_ - (start ? start->offset_of_chain_ : 0)))
    {
        buffer_chain{other}; return;
    }

    buffer_chain{other.parent_, data_len};
    memcpy(this->buffer_, other.buffer_ + (start ? start->offset_of_chain_ : 0), data_len);
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
    this->parent_ = other.parent_;
}

int buffer_chain::set_offset(size_t offset)
{
    if(offset > capacity_) return -1;
    off_ = offset; return 0;
}

buffer_chain::Iter buffer_chain::begin()
{
    return Iter{this->parent_, this, this->parent_->iter_of_chain(*this).offset_of_buffer_, 0, 0};
}

buffer_chain::Iter buffer_chain::end()
{
    return Iter{this->parent_, this, this->parent_->iter_of_chain(*this).offset_of_buffer_ + this->off_, 0, off_};
}

size_t buffer_chain::calculate_actual_capacity(size_t given_capacity)
{
    size_t to_alloc = 0;
    if(given_capacity < MAXIMUM_CHAIN_SIZE / 2)
    {
        to_alloc = DEFAULT_CHAIN_SIZE;
        while(to_alloc < given_capacity){
            to_alloc <<= 1;
        }
    }
    else
    {
#ifdef MINIMUM_CHAIN_SIZE
        given_capacity = given_capacity < MINIMUM_CHAIN_SIZE ? MINIMUM_CHAIN_SIZE : given_capacity;
#endif
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

//TODO UT
buffer::buffer(const buffer& other, size_t data_len) 
{
    if(other.total_len_ == 0 || data_len == 0) 
    {
        buffer{}; return;
    }

    //copy all
    if(data_len >= other.total_len())
    {
        buffer{other}; return;
    }

    //partially copy
    size_t remain_to_copy = data_len;
    const buffer_chain* current_chain = &other.first();

    while(!other.is_last_chain_with_data(current_chain) && current_chain->get_offset() < remain_to_copy)
    {
        ASSERT_CHAIN_FULL(current_chain)
        chains_.push_back(*current_chain);
        this->last_chain_with_data_ = &chains_.back();
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

buffer::buffer(const buffer& other, size_t data_len, const Iter* start)
{
    if(start == 0 || !other.validate_iter(*start) || data_len == 0 || other.total_len_ == 0) 
    {
        buffer{}; return;
    }

    buffer_chain* current_chain = start->chain_;
    size_t bytes_can_copy_in_current_chain = current_chain->get_offset() - start->offset_of_chain_;
    Iter start_iter_in_current_chain = *start;
    size_t maximum_bytes_can_copy_out = other.total_len_ - start->offset_of_buffer_;
    size_t remain_to_copy = data_len > maximum_bytes_can_copy_out ? maximum_bytes_can_copy_out : data_len;

    while(  !is_last_chain_with_data(current_chain) && 
            bytes_can_copy_in_current_chain < remain_to_copy)
    {
        ASSERT_CHAIN_FULL(current_chain)
        chains_.push_back(buffer_chain{*current_chain, bytes_can_copy_in_current_chain, &start_iter_in_current_chain});
        this->last_chain_with_data_ = &chains_.back();

        remain_to_copy -= bytes_can_copy_in_current_chain;
        current_chain = current_chain->next();
        bytes_can_copy_in_current_chain = current_chain->get_offset();
        start_iter_in_current_chain = current_chain->begin();
    }

    //get to the last chain or current chain can cover {remain_to_copy}
    assert(current_chain != 0 && "current should not be nullptr");
    buffer_chain last_chain{*current_chain, remain_to_copy};
    chains_.push_back(last_chain);
    this->last_chain_with_data_ = &chains_.back();
    total_len_ = data_len;
}

buffer::Iter buffer::begin()
{
    return Iter{this, &(this->chains_.front()), 0, 0, 0};
}

buffer::Iter buffer::end()
{
    return Iter{this, last_chain_with_data_, this->total_len_, 0, last_chain_with_data_->get_offset()};
}

buffer::Iter buffer::iter_of_chain(const buffer_chain& chain)
{
    size_t offset_of_buffer = 0;
    buffer_chain* current_chain = &this->chains_.front();

    while(current_chain != 0 && current_chain != &chain)
    {
        ASSERT_CHAIN_FULL(current_chain)
        offset_of_buffer += current_chain->chain_capacity();
        current_chain = current_chain->next();
    }

    //the chain is not found
    if(current_chain == 0)
    {
        return Iter{0, 0, 0, 0, 0};
    }
    return Iter{this, current_chain, offset_of_buffer, 0, 0};
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

int buffer::append(const buffer& other, size_t data_len, const Iter* start)
{

}
int buffer::append_printf(const char* fmt, ...)
{

}
int buffer::append_vprintf(const char* fmt, va_list ap)
{

}

int buffer::prepend(const buffer& other, size_t data_len, const Iter* start)
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

int buffer::copy_out_from(void* data, size_t data_len, const Iter* start)
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

buffer_iter buffer::search_eol(size_t* eol_len_out, buffer_eol_style eol_style, const Iter* start)
{

}

int buffer::peek(std::vector<const buffer_iovec*> vec_out, size_t len, const Iter* start)
{

}

inline bool buffer::is_last_chain_with_data(const buffer_chain* current_chain) const
{
    if(current_chain == 0 || current_chain != last_chain_with_data_)
        return false;
    return true;
}

bool buffer::validate_iter(const Iter& iter) const 
{
    if(iter.buffer_ != this)
    {
        return false;
    }

    const buffer_chain* current_chain = &*this->chains_.begin();
    while(current_chain != nullptr && current_chain != iter.chain_)
    {
        current_chain = current_chain->next();
    }

    if(current_chain == iter.chain_)
    {
        if( iter.offset_of_chain_ < current_chain->get_offset() && 
            iter.buffer_ == current_chain->get_buffer())
            return true;
    }
    assert(current_chain == nullptr && "any other conditions???");
    return false;
}

buffer_chain* buffer::expand_if_needed(size_t data_len)
{
    //no data in buffer at all
    if(last_chain_with_data_ == 0)
    {
        assert(chains_.empty() && "chains_ should be empty");
        chains_.push_back(buffer_chain{this, data_len});
        last_chain_with_data_ = &chains_.back();
        return last_chain_with_data_;
    }

    //检查是否值得扩展
    // if()

}