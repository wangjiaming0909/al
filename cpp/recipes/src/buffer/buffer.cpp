#include "buffer/buffer.h"
#include <exception>


buffer_iter::buffer_iter( const buffer* buffer_ptr
    , const buffer_chain* chain
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
        steps_can_forward_in_current_chain = chain_->get_offset();
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
    , misalign_(0)
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

    // buffer_ = static_cast<void*>(new char[capacity_]);
    buffer_ = ::calloc(capacity_, 1);
    assert(buffer_ != nullptr && ("new operator error size:" + capacity_));
}

buffer_chain::~buffer_chain()
{
    if(buffer_ != 0) 
        free(buffer_);
}

buffer_chain::buffer_chain(buffer_chain&& other)
{
    buffer_ = other.buffer_;
    other.buffer_ = 0;

    capacity_ = other.capacity_;
    off_ = other.off_;
    next_ = other.next_;
    parent_ = other.parent_;
    misalign_ = other.misalign_;
}

//// other 原来是什么样，复制后的对象也是什么样，capacity, misalign的大小都是一样的
buffer_chain::buffer_chain(const buffer_chain& other)
{
    capacity_ = calculate_actual_capacity(other.size());
    buffer_ = ::calloc(capacity_, 1);
    assert(buffer_ != nullptr && ("new operator error size: " + capacity_));

    ::memcpy(buffer_, other.buffer_ + other.misalign_, other.size());
    misalign_ = 0;
    next_ = other.next_;
    off_ = other.size();
    parent_ = other.parent_;
}

buffer_chain::buffer_chain(const buffer_chain& other, size_t data_len, Iter start)
{
    //check if start is in the chain {other}, and within the range of other
    if(start.chain_ != &other 
        || start.offset_of_chain_ >= other.off_
        || start.offset_of_chain_ < other.misalign_  //TODO < or <= 
        || data_len > (other.off_ - start.offset_of_chain_)) //data_len太长, 超过other中已有的长度
    {
        throw std::exception();
    }
    //TODO add misalign_

    //想要拷贝的字节数大于现有的字节数(现有是指从chain的最后到iter, 不是chain.size())
    // if(data_len > (other.off_ - start.offset_of_chain_))
    // {
    //     //TODO 貌似不正确
    //     ::new(this)buffer_chain{other}; return;
    // }

    ::new(this)buffer_chain(other.parent_, data_len);
    memcpy(buffer_, other.buffer_ + start.offset_of_chain_, data_len);
    next_ = other.next_;
    off_ = data_len;
    misalign_ = 0;
}

buffer_chain& buffer_chain::operator= (const buffer_chain& other)
{
    if(this == &other) return *this;

    if(capacity_ < other.capacity_)
    {
        if(buffer_ != 0) 
            free(buffer_);
        capacity_ = other.capacity_;
        buffer_ = ::calloc(capacity_, 1);
    }

    ::memcpy(buffer_, other.buffer_ + misalign_, other.size());
    next_ = other.next_;
    off_ = other.size();
    parent_ = other.parent_;
    misalign_ = 0;
}

int buffer_chain::set_offset(size_t offset)
{
    if(offset > capacity_) return -1;
    off_ = offset; return 0;
}

buffer_chain::Iter buffer_chain::begin() const
{
    return Iter{this->parent_, this, this->parent_->iter_of_chain(*this).offset_of_buffer_, 0, misalign_};
}

buffer_chain::Iter buffer_chain::end() const
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
    update_last_chain_with_data(other);
    this->total_len_ = other.total_len_;
}

buffer::buffer(const buffer& other, size_t data_len) 
{
    if(other.total_len_ == 0 || data_len == 0) 
    {
        ::new(this)buffer{}; return;
    }

    //copy all
    if(data_len >= other.total_len())
    {
        ::new(this)buffer{other}; return;
    }

    //partially copy
    size_t remain_to_copy = data_len;
    const buffer_chain* current_chain = &other.first();

    while(!other.is_last_chain_with_data(current_chain) && current_chain->get_offset() < remain_to_copy)
    {
        // ASSERT_CHAIN_FULL(current_chain)// 并不一定是full了
        push_back(*current_chain);
        this->last_chain_with_data_ = &chains_.back();
        remain_to_copy -= current_chain->get_offset();
        current_chain = current_chain->next();
    }

    //get to the last_chain_with_data or current_chain can cover remain_to_copy
    //even it is the last_chain_with_data, it can also cover the remain_to_copy
    buffer_chain last_chain{*current_chain, remain_to_copy, current_chain->begin()};
    push_back(last_chain);
    //update {next chain} of 倒数第二个 chain, 因为刚刚新添加一个chain
    if(last_chain_with_data_)
        last_chain_with_data_->set_next_chain(&chains_.back());
    last_chain_with_data_ = &chains_.back();
    total_len_ = data_len;
}

buffer::buffer(const buffer& other, size_t data_len, const Iter* start)
{
    //TODO vcalidate start
    if(start == 0 || !other.validate_iter(*start) || data_len == 0 || other.total_len_ == 0) 
    {
        ::new(this)buffer{}; return;
    }

    const buffer_chain* current_chain = start->chain_;
    size_t bytes_can_copy_in_current_chain = current_chain->get_offset() - start->offset_of_chain_;
    Iter start_iter_in_current_chain = *start;
    size_t maximum_bytes_can_copy_out = other.total_len_ - start->offset_of_buffer_;
    size_t remain_to_copy = data_len > maximum_bytes_can_copy_out ? maximum_bytes_can_copy_out : data_len;
    bool start_from_front = false;// if the start_iter has been used, so next time we need to read from front of next chain

    while(  !other.is_last_chain_with_data(current_chain) && 
            bytes_can_copy_in_current_chain < remain_to_copy)
    {
        push_back(buffer_chain{*current_chain, bytes_can_copy_in_current_chain, start_iter_in_current_chain});

        this->last_chain_with_data_ = &chains_.back();

        remain_to_copy -= bytes_can_copy_in_current_chain;
        current_chain = current_chain->next();
        bytes_can_copy_in_current_chain = current_chain->get_offset();
        start_iter_in_current_chain = current_chain->begin();
        start_from_front = true;
    }

    //get to the last chain or current chain can cover {remain_to_copy}
    assert(current_chain != 0 && "current should not be nullptr");

    if(start_from_front)
        push_back(buffer_chain{*current_chain, remain_to_copy, current_chain->begin()});
    else
        push_back(buffer_chain{*current_chain, remain_to_copy, start_iter_in_current_chain});

    this->last_chain_with_data_ = &chains_.back();
    total_len_ = data_len;
}

buffer_chain* buffer::push_back(const buffer_chain&& chain)
{
    return push_back(chain);//看起来像是这个的问题
}

//push_back与data没有关系, 仅仅是向chains_中添加节点
buffer_chain* buffer::push_back(const buffer_chain& chain)
{
    if(chains_.size() == 0) 
    {
        chains_.push_back(chain);
    } 
    else 
    {
        auto& last_chain = chains_.back();
        chains_.push_back(chain);
        last_chain.set_next_chain(&chains_.back());
        chains_.back().set_next_chain(0);
    }

    return &chains_.back();
}

buffer::Iter buffer::begin()
{
    return Iter{this, &(this->chains_.front()), 0, 0, this->chains_.front().misalign_};
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
        // ASSERT_CHAIN_FULL(current_chain)
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
    if(this == &other) return *this;
    if(other.buffer_length() == 0) 
        return *this;
    chains_ = other.chains_;
    total_len_ = other.total_len_;
    update_last_chain_with_data(other);
}

size_t buffer::first_chain_length()
{
    if(chains_.size() == 0 || chains_.front().get_offset() == 0) return 0;
    return chains_.front().get_offset();
}

int buffer::append(const buffer& other, size_t data_len, Iter start)
{
    if(!other.validate_iter(start) || other.total_len_ == 0 || data_len == 0)
    {
        return -1;
    }

    free_trailing_empty_chains();

    const buffer_chain *current_chain = start.chain_;
    size_t bytes_can_copy_in_current_chain = current_chain->get_offset() - start.offset_of_chain_;
    Iter start_iter_in_current_chain = start;
    size_t maximum_bytes_can_copy_out = other.total_len_ - start.offset_of_buffer_;
    size_t remain_to_copy = data_len > maximum_bytes_can_copy_out ? maximum_bytes_can_copy_out : data_len;
    size_t total_bytes_going_to_copy = remain_to_copy;
    bool start_from_front = false; // if the start_iter has been used, so next time we need to read from front of next chain

    while(  !other.is_last_chain_with_data(current_chain) && 
            bytes_can_copy_in_current_chain < remain_to_copy)
    {
        append(buffer_chain{*current_chain, bytes_can_copy_in_current_chain, start_iter_in_current_chain});

        this->last_chain_with_data_ = &chains_.back();
        total_len_ += bytes_can_copy_in_current_chain;

        remain_to_copy -= bytes_can_copy_in_current_chain;
        current_chain = current_chain->next();
        bytes_can_copy_in_current_chain = current_chain->get_offset();
        start_iter_in_current_chain = current_chain->begin();
        start_from_front = true;
    }

    //get to the last chain or current chain can cover {remain_to_copy}
    assert(current_chain != 0 && "current should not be nullptr");

    if(start_from_front)
        append(buffer_chain{*current_chain, remain_to_copy, current_chain->begin()});
    else
        append(buffer_chain{*current_chain, remain_to_copy, start_iter_in_current_chain});

    total_len_ += total_bytes_going_to_copy;
    return total_bytes_going_to_copy;
}

int buffer::append(const buffer_chain &chain)
{
    auto* current_chain = expand_if_needed(chain.off_);
    assert(current_chain->chain_free_space() >= chain.off_);
    ::memcpy(last_chain_with_data_->buffer_, chain.buffer_, chain.off_);
    current_chain->off_ += chain.off_;
    last_chain_with_data_ = current_chain;
    return chain.off_;
}

int buffer::append(const buffer_chain &&chain)
{
    return append(chain);
}

int buffer::append_printf(const char* fmt, ...)
{

}
int buffer::append_vprintf(const char* fmt, va_list ap)
{

}

int buffer::prepend(const buffer& other, size_t data_len, Iter start)
{
    
}

unsigned char* buffer::pullup(size_t size)
{
    //如果size 比 total_len_ 大, 那么将不能保证第一个node可以达到size的大小,因此返回nullptr
    if(size == 0 || size > total_len_)
        return nullptr;

    if(size < 0) //传递负值,表示全部align到第一个节点
        size = total_len_;

    buffer_chain& fisrt_chain = chains_.front();
    //如果第一个chain的大小已经满足size了,那么直接返回
    // if(fisrt_chain.)
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
    //validate buffer
    if(iter.buffer_ != this)
    {
        return false;
    }

    //find the chain
    const buffer_chain* current_chain = &*this->chains_.begin();
    while(current_chain != nullptr && current_chain != iter.chain_)
    {
        current_chain = current_chain->next();
    }

    //validate the chain
    if(current_chain == iter.chain_)
    {
        if( iter.chain_->get_offset() == current_chain->get_offset() && 
            iter.chain_->get_buffer() == current_chain->get_buffer())
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
        push_back(buffer_chain{this, data_len});
        return &chains_.back();
    }

    //if {last_chain_with_data_} has enough space for {data_len} then do nothing
    if (last_chain_with_data_->chain_free_space() >= data_len)
    {
        return last_chain_with_data_;
    }

    //检查是否值得扩展
    buffer_chain* lc = last_chain_with_data_;
    if (lc->chain_free_space() < (lc->chain_capacity() / 8) || //(比例上)当前chain 的剩余空间 只有 capacity 的 1/8, resize 需要拷贝较多内存
        lc->get_offset() > buffer_chain::MAXIMUM_SIZE_WHEN_EXPAND ||  // (绝对值上)chain 中已经存了 4096 个字节了
        data_len >= (buffer_chain::MAXIMUM_CHAIN_SIZE - lc->get_offset())) //即便进行扩展，可能扩展之后的空闲内存还是塞不下 data_len
    {
        //因此不进行扩展，so 看lc->next 的吧
        if(lc->next() && lc->next()->chain_free_space() > data_len)
        {
            //存在, 并且有足够的空间塞下 data_len
            assert(lc->next()->get_offset() == 0 && "lc->next() should be empty");
            return lc->next();
        } else {
            //要么是根本不存在next, 要么是next 不够(虽然是空的)
            free_trailing_empty_chains();
            push_back(buffer_chain{this, data_len});
        }
    } else {
        //now we can resize lc 
        size_t length_needed = lc->get_offset() + data_len;
        buffer_chain chain_newed{this, length_needed};
        //! lc 一定是最后一个chain么?? 为什么pop_back()
        chain_newed = *lc;//copy from lc into chain_newed
        chains_.pop_back();
        //pop_back 之后，取得最后一个chain, 再将现在的最后一个chain 的next 设置为之后加入的新chain
        last_chain_with_data_ = push_back(chain_newed);
        // last_chain_with_data_ = &chains_.back();
    }
    return &chains_.back();
}

buffer_chain* buffer::free_trailing_empty_chains()
{
    buffer_chain* chain = last_chain_with_data_;
    //no data at all
    if(chain == 0)
    {
        chains_.clear(); return 0;
    }
    assert(chain->get_offset() > 0);
    
    auto start_iter = chains_.begin();
    while(&*start_iter != chain) 
    {
        ++start_iter;
    }

    auto back_iter = chains_.end();
    --back_iter;
    while(back_iter != start_iter) {
        chains_.pop_back();
        chains_.back().set_next_chain(0);
        --back_iter;
    }
    return chain;
}

buffer_chain* buffer::update_last_chain_with_data(const buffer& other)
{
    auto start = chains_.begin();
    auto end = chains_.end();
    auto other_start = other.chains_.begin();
    auto other_end = other.chains_.end();
    for (; 
        start != end && 
        other_start != other_end && 
        (&*other_start != other.last_chain_with_data_); 
        start++, other_start++);
    last_chain_with_data_ = &*start;
}