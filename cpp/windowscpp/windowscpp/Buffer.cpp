#include "buffer.h"
#include <exception>
#include <utility>

buffer_iter::buffer_iter(const buffer* buffer_ptr
    , const buffer_chain* chain
    , uint32_t offset_of_buffer
    , uint32_t chain_number
    , uint32_t offset_of_chain)
    : buffer_(buffer_ptr)
    , chain_(chain)
    , offset_of_buffer_(offset_of_buffer)
    , chain_number_(chain_number)
    , offset_of_chain_(offset_of_chain)
{

}

const buffer_iter buffer_iter::NULL_ITER = buffer_iter{ nullptr, nullptr, 0, 0, 0 };

buffer_iter& buffer_iter::operator+(uint32_t forward_steps)
{
    if (forward_steps == 0) return *this;

    uint32_t maximum_steps_can_forward = buffer_->total_len() - this->offset_of_buffer_;
    if (forward_steps > maximum_steps_can_forward)
    {
        chain_ = buffer_->last_chain_with_data();
        offset_of_chain_ = chain_->get_offset();
        offset_of_buffer_ = buffer_->total_len();
        chain_number_ = 0;//!
        return *this;
    }

    //    int forward_chain_numbers = -1;
    int64_t steps_can_forward_in_current_chain = chain_->get_offset() - this->offset_of_chain_;
    assert(steps_can_forward_in_current_chain >= 0 && "offset of a chain should > offset_of_chain in buffer_iter");
    uint32_t remain_steps = forward_steps;

    //current is not the last chain_with_data
    while (!buffer_->is_last_chain_with_data(chain_) && remain_steps > steps_can_forward_in_current_chain)
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

bool buffer_iter::operator>(const buffer_iter& other)
{
    //�������κ�һ��buffer��0�Ͳ��Ƚ��ˣ�ֱ�ӷ���false
    if (buffer_ == nullptr || other.buffer_ == nullptr) return false;
    // �����Ͳ���ͬһ��buffer��
    if (other.buffer_ != buffer_) return false;

    if (!buffer_->validate_iter(other) || !other.buffer_->validate_iter(*this)) return false;

    return offset_of_buffer_ > other.offset_of_buffer_;
}

bool buffer_iter::operator<(const buffer_iter& other)
{
    return !this->operator>(other);
}

bool buffer_iter::operator==(const buffer_iter& other)
{
    if (buffer_ == nullptr || other.buffer_ == nullptr) return false;
    if (other.buffer_ != buffer_) return false;
    if (!buffer_->validate_iter(other) || !other.buffer_->validate_iter(*this)) return false;
    return offset_of_buffer_ == other.offset_of_buffer_;
}

buffer_chain::buffer_chain(buffer* parent, uint32_t capacity)
    : buffer_(nullptr)
    , off_(0)
    , next_(nullptr)
    , parent_(parent)
    , misalign_(0)
{
    if (capacity == 0)
    {
        capacity_ = DEFAULT_CHAIN_SIZE;
    }
    else
    {
        capacity_ = this->calculate_actual_capacity(capacity);
    }

    // buffer_ = static_cast<void*>(new char[capacity_]);
    buffer_ = static_cast<char*>(::calloc(capacity_, 1));
    assert(buffer_ != nullptr && ("new operator error size"));
}

buffer_chain::~buffer_chain()
{
    if (buffer_ != nullptr)
        free(buffer_);
}

buffer_chain::buffer_chain(buffer_chain&& other)
{
    buffer_ = other.buffer_;
    capacity_ = other.capacity_;
    off_ = other.off_;
    next_ = other.next_;
    parent_ = other.parent_;
    misalign_ = other.misalign_;

    other.buffer_ = nullptr;
    other.capacity_ = 0;
    other.off_ = 0;
    other.next_ = nullptr;
    other.parent_ = nullptr;
    other.misalign_ = 0;
}

//// other ԭ����ʲô�������ƺ�Ķ���Ҳ��ʲô����capacity, misalign�Ĵ�С����һ����
buffer_chain::buffer_chain(const buffer_chain& other)
{
    capacity_ = calculate_actual_capacity(other.capacity_);
    buffer_ = static_cast<char*>(::calloc(capacity_, 1));
    assert(buffer_ != nullptr && ("new operator error size"));

    ::memcpy(buffer_, other.get_start_buffer(), other.size());
    misalign_ = 0;
    next_ = other.next_;
    off_ = other.size();
    parent_ = other.parent_;
}

buffer_chain::buffer_chain(const buffer_chain& other, uint32_t data_len, Iter start)
{
    //check if start is in the chain {other}, and within the range of other
    if (!other.validate_iter(start) || //TODO < or <=  
        data_len > (other.off_ - start.offset_of_chain_)) //data_len̫��, ����other�����еĳ���
    {
        throw std::exception();
    }

    //��Ҫ�������ֽ����������е��ֽ���(������ָ��chain�����iter, ����chain.size())
    // if(data_len > (other.off_ - start.offset_of_chain_))
    // {
    //     //TODO ò�Ʋ���ȷ
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
    if (this == &other) return *this;

    if (capacity_ < other.capacity_)
    {
        if (buffer_ != nullptr)
            free(buffer_);
        capacity_ = other.capacity_;
        buffer_ = static_cast<char*>(::calloc(capacity_, 1));
    }

    ::memcpy(buffer_, other.buffer_ + misalign_, other.size());
    next_ = other.next_;
    off_ = other.size();
    parent_ = other.parent_;
    misalign_ = 0;
    return *this;
}

uint32_t buffer_chain::append(const buffer_chain& chain)
{
    uint32_t size = chain.size();//��ֹ�Լ�append���Լ����ȼ���size
    if (size > chain_free_space()) return 0;
    ::memcpy(buffer_ + off_, chain.buffer_ + chain.misalign_, size);
    off_ += size;
    return size;
}

uint32_t buffer_chain::append(const buffer_chain& chain, uint32_t len, Iter start)
{
    if (len > chain.size() || !chain.validate_iter(start) || len > chain.off_ - start.offset_of_chain_)
    {
        return 0;
    }
    ::memcpy(buffer_ + off_, chain.buffer_ + start.offset_of_chain_, len);
    off_ += len;
    return len;
}

int64_t buffer_chain::append(const void* data, uint32_t data_len)
{
    if (data == 0 || data_len == 0) return -1;

    if (chain_free_space() < data_len) return -1;

    ::memcpy(buffer_ + off_, data, data_len);
    off_ += data_len;
    return data_len;
}

bool buffer_chain::validate_iter(Iter it) const
{
    if (it.chain_ != this ||
        it.offset_of_chain_ >= this->off_ ||
        it.offset_of_chain_ < this->misalign_)  //TODO < or <= 
        return false;
    return true;
}

int buffer_chain::set_offset(uint32_t offset)
{
    if (offset > capacity_) return -1;
    off_ = offset; return 0;
}

buffer_chain::Iter buffer_chain::begin() const
{
    return Iter{ this->parent_, this, this->parent_->iter_of_chain(*this).offset_of_buffer_, 0, misalign_ };
}

buffer_chain::Iter buffer_chain::end() const
{
    return Iter{ this->parent_, this, this->parent_->iter_of_chain(*this).offset_of_buffer_ + this->off_, 0, off_ };
}

uint32_t buffer_chain::calculate_actual_capacity(uint32_t given_capacity)
{
    uint32_t to_alloc = 0;
    if (given_capacity < MAXIMUM_CHAIN_SIZE / 2)
    {
        to_alloc = DEFAULT_CHAIN_SIZE;
        while (to_alloc < given_capacity) {
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
    , last_chain_with_data_(nullptr)
    , total_len_(0)
{

}

buffer::buffer(const buffer& other) : chains_(), last_chain_with_data_(nullptr), total_len_(0)
{
    this->chains_ = other.chains_;
    update_last_chain_with_data(other);
    update_next_field_after_copy();
    this->total_len_ = other.total_len_;
}

buffer::buffer(const buffer& other, uint32_t data_len) : chains_(), last_chain_with_data_(nullptr), total_len_(0)
{
    if (other.total_len_ == 0 || data_len == 0)
    {
        ::new(this)buffer{}; return;
    }

    //copy all
    if (data_len >= other.total_len())
    {
        ::new(this)buffer{ other }; return;
    }

    //partially copy
    uint32_t remain_to_copy = data_len;
    const buffer_chain* current_chain = &other.first();

    while (!other.is_last_chain_with_data(current_chain) && current_chain->size() < remain_to_copy)
    {
        // ASSERT_CHAIN_FULL(current_chain)// ����һ����full��
        append(*current_chain);
        this->last_chain_with_data_ = &chains_.back();
        remain_to_copy -= current_chain->size();
        current_chain = current_chain->next();
    }

    //get to the last_chain_with_data or current_chain can cover remain_to_copy
    //even it is the last_chain_with_data, it can also cover the remain_to_copy
    buffer_chain last_chain{ *current_chain, remain_to_copy, current_chain->begin() };
    append(last_chain);
    //update {next chain} of �����ڶ��� chain, ��Ϊ�ո������һ��chain
    if (last_chain_with_data_)
        last_chain_with_data_->set_next_chain(&chains_.back());
    last_chain_with_data_ = &chains_.back();
    total_len_ = data_len;
}

buffer::buffer(const buffer& other, uint32_t data_len, Iter start) : chains_(), last_chain_with_data_(nullptr), total_len_(0)
{
    if (!other.validate_iter(start) || data_len == 0 || other.total_len_ == 0)
    {
        ::new(this)buffer{}; return;
    }

    const buffer_chain* current_chain = start.chain_;
    if (!current_chain->validate_iter(start)) //validate start in current_chain iter, start must within misalign_ and off_
    {
        ::new(this)buffer{}; return;
    }

    uint32_t bytes_can_copy_in_current_chain = current_chain->off_ - start.offset_of_chain_;
    Iter start_iter_in_current_chain = start;
    uint32_t maximum_bytes_can_copy_out = other.total_len_ - start.offset_of_buffer_;
    uint32_t remain_to_copy = data_len > maximum_bytes_can_copy_out ? maximum_bytes_can_copy_out : data_len;
    bool start_from_begin = false;// if the start_iter has been used, so next time we need to read from front of next chain

    while (!other.is_last_chain_with_data(current_chain) &&
        bytes_can_copy_in_current_chain < remain_to_copy)
    {
        buffer_chain _chain{ *current_chain, bytes_can_copy_in_current_chain, start_iter_in_current_chain };
        append(_chain);

        this->last_chain_with_data_ = &chains_.back();

        remain_to_copy -= bytes_can_copy_in_current_chain;
        current_chain = current_chain->next();
        bytes_can_copy_in_current_chain = current_chain->size();
        start_iter_in_current_chain = current_chain->begin();
        start_from_begin = true;
    }

    //get to the last chain or current chain can cover {remain_to_copy}
    assert(current_chain != nullptr && "current should not be nullptr");

    if (start_from_begin)
        append(buffer_chain{ *current_chain, remain_to_copy, current_chain->begin() });
    else
        append(buffer_chain{ *current_chain, remain_to_copy, start_iter_in_current_chain });

    this->last_chain_with_data_ = &chains_.back();
    total_len_ = data_len;
}

buffer_chain* buffer::push_back(buffer_chain&& chain)
{
    //rvalue ref can't call lvalue ref
    // return push_back(chain);
    if (chains_.size() == 0)
    {
        chains_.push_back(std::move(chain));
    }
    else
    {
        auto& last_chain = chains_.back();
        chains_.push_back(std::move(chain));
        last_chain.next_ = &chains_.back();
        chains_.back().set_next_chain(nullptr);
    }
    return &chains_.back();
}

//push_back��dataû�й�ϵ, ��������chains_����ӽڵ�
buffer_chain* buffer::push_back(const buffer_chain& chain)
{
    if (chains_.size() == 0)
    {
        chains_.push_back(chain);
    }
    else
    {
        auto& last_chain = chains_.back();
        chains_.push_back(chain);
        last_chain.next_ = &chains_.back();
        chains_.back().set_next_chain(nullptr);
    }

    return &chains_.back();
}

//ͬ����, push_front�����������ݵ����⣬ ��������chains��ͷ�������һ��chain
buffer_chain* buffer::push_front(buffer_chain&& chain)
{
    buffer_chain* first_chain = chains_.size() == 0 ? nullptr : &chains_.front();
    chains_.push_front(std::move(chain));

    chains_.front().next_ = first_chain;
    return &chains_.front();
}

buffer_chain* buffer::push_front(buffer_chain& chain)
{
    buffer_chain* first_chain = chains_.size() == 0 ? nullptr : &chains_.front();
    chains_.push_front(chain);
    chains_.front().next_ = first_chain;
    return &chains_.front();
}

buffer::Iter buffer::begin()
{
    return Iter{ this, &(this->chains_.front()), 0, 0, this->chains_.front().misalign_ };
}

buffer::Iter buffer::end()
{
    return Iter{ this, last_chain_with_data_, this->total_len_, 0, last_chain_with_data_->get_offset() };
}

buffer::Iter buffer::iter_of_chain(const buffer_chain& chain)
{
    if (total_len_ == 0) return Iter::NULL_ITER;
    uint32_t offset_of_buffer = 0;
    buffer_chain* current_chain = &this->chains_.front();

    while (current_chain != nullptr && current_chain != &chain)
    {
        // ASSERT_CHAIN_FULL(current_chain)
        offset_of_buffer += current_chain->chain_capacity();
        current_chain = current_chain->next();
    }

    //the chain is not found
    if (current_chain == nullptr)
    {
        return Iter::NULL_ITER;
    }
    return Iter{ this, current_chain, offset_of_buffer, 0, current_chain->misalign_ };
}

buffer& buffer::operator=(const buffer& other)
{
    if (this == &other) return *this;
    if (other.buffer_length() == 0)
        return *this;
    chains_ = other.chains_;
    total_len_ = other.total_len_;
    update_last_chain_with_data(other);
    return *this;
}

uint32_t buffer::first_chain_length()
{
    if (chains_.size() == 0 || chains_.front().get_offset() == 0) return 0;
    return chains_.front().size();
}

int64_t buffer::append(const buffer& other, uint32_t data_len, Iter start)
{
    if (!other.validate_iter(start) || other.total_len_ == 0 || data_len == 0)
    {
        return -1;
    }

    free_trailing_empty_chains();

    const buffer_chain* current_chain = start.chain_;
    if (!current_chain->validate_iter(start))
    {
        return -1;
    }

    uint32_t bytes_can_copy_in_current_chain = current_chain->off_ - start.offset_of_chain_;
    Iter start_iter_in_current_chain = start;
    uint32_t maximum_bytes_can_copy_out = other.total_len_ - start.offset_of_buffer_;
    uint32_t remain_to_copy = data_len > maximum_bytes_can_copy_out ? maximum_bytes_can_copy_out : data_len;
    uint32_t total_bytes_going_to_copy = remain_to_copy;
    bool start_from_begin = false; // if the start_iter has been used, so next time we need to read from front of next chain

    while (!other.is_last_chain_with_data(current_chain) &&
        bytes_can_copy_in_current_chain < remain_to_copy)
    {
        buffer_chain _chain{ *current_chain, bytes_can_copy_in_current_chain, start_iter_in_current_chain };
        append(_chain);

        this->last_chain_with_data_ = &chains_.back();
        //        total_len_ += bytes_can_copy_in_current_chain;

        remain_to_copy -= bytes_can_copy_in_current_chain;
        current_chain = current_chain->next();
        bytes_can_copy_in_current_chain = current_chain->size();
        start_iter_in_current_chain = current_chain->begin();
        start_from_begin = true;
    }

    //get to the last chain or current chain can cover {remain_to_copy}
    assert(current_chain != nullptr && "current should not be nullptr");

    if (start_from_begin)
    {
        auto _chain = buffer_chain{ *current_chain, remain_to_copy, current_chain->begin() };
        append(_chain);
    }
    else
    {
        auto _chain = buffer_chain{ *current_chain, remain_to_copy, start_iter_in_current_chain };
        append(_chain);
    }

    //    total_len_ += total_bytes_going_to_copy;
    return total_bytes_going_to_copy;
}

int64_t buffer::append(const buffer_chain& chain)//TODO copy too much
{
    uint32_t size = chain.size();
    auto* current_chain = expand_if_needed(size);
    assert(current_chain->chain_free_space() >= size);
    //TODO check if that current_chain is the same as last_chain_with_data
    //expand֮���current_chain����֮ǰ��last_chain_with_data, ���û����չ
    if (last_chain_with_data_ == current_chain)//��֮ǰ��λ�ý���copy
    {
        ::memcpy(current_chain->buffer_ + last_chain_with_data_->off_, chain.get_start_buffer(), size);
    }
    else
    {//�Ѿ�expand��,��˴�ͷ��ʼcopy, ���п���֮ǰ��û���κ�����,���last_chain_with_data_ == 0
        assert(last_chain_with_data_ == nullptr || last_chain_with_data_->next_ == current_chain);
        ::memcpy(current_chain->buffer_, chain.get_start_buffer(), size);
    }

    current_chain->off_ += size;
    last_chain_with_data_ = current_chain;
    total_len_ += size;
    return size;
}

int64_t buffer::append(buffer_chain&& chain)
{
    if (last_chain_with_data_ != nullptr)
    {
        auto* last_chain = free_trailing_empty_chains();
        chains_.push_back(std::move(chain));
        last_chain->next_ = &chains_.back();
    }
    else
    {
        chains_.push_back(std::move(chain));
    }
    chains_.back().next_ = nullptr;
    chains_.back().parent_ = this;
    return chains_.back().size();//!!
}

int64_t buffer::append(const void* data, uint32_t data_len)
{
    if (data == 0 || data_len == 0) return -1;

    buffer_chain* data_chain = expand_if_needed(data_len);
    if (data_chain->chain_free_space() < data_len) return -1;//for expand error

    data_chain->append(data, data_len);
    total_len_ += data_len;
    last_chain_with_data_ = data_chain;
    return data_len;
}

int64_t buffer::append_printf(const char* fmt, ...)
{
    if (fmt == 0) {
        //LOG(WARNING) << "fmt error: " << fmt;
        return -1;
    }
    int ret = 0;
    buffer_chain* data_chain = expand_if_needed(64);
    char* data_p = data_chain->buffer_ + data_chain->off_;
    uint32_t data_size = data_chain->chain_free_space();

    for (;;)
    {
        va_list va;
        va_start(va, fmt);
        int vs = vsnprintf(data_p, data_size, fmt, va);
        va_end(va);

        if (vs < 0) return vs;//vsnprintf return error

        if (buffer_chain::MAXIMUM_CHAIN_SIZE < static_cast<uint32_t>(vs))//fmt �ִ�̫��
        {
            //LOG(WARNING) << "Too long for a chain, size: " << vs;
            return -1;
        }

        if ((uint32_t)vs < data_size)//data_size��������fmt�ִ�
        {
            data_chain->off_ += static_cast<uint32_t>(vs);
            total_len_ += static_cast<uint32_t>(vs);
            ret = vs;
            last_chain_with_data_ = data_chain;
            break;
        }

        data_chain = expand_if_needed(static_cast<uint32_t>(vs) + 1);
        data_p = data_chain->buffer_ + data_chain->off_;
        data_size = data_chain->chain_free_space();
    }
    return ret;
}

int64_t buffer::append_vprintf(const char* fmt, va_list ap)
{
    (void)fmt;
    (void)ap;
    return 0;
}

int buffer::prepend(const buffer& other, uint32_t data_len, Iter start)
{
    (void)other;
    (void)data_len;
    (void)start;
    return 0;
}

unsigned char* buffer::pullup(int64_t size)
{
    //���size �� total_len_ ��, ��ô�����ܱ�֤��һ��node���Դﵽsize�Ĵ�С,��˷���nullptr
    if (size == 0 || size > total_len_)
        return nullptr;

    if (size < 0) //���ݸ�ֵ,��ʾȫ��align����һ���ڵ�
        size = total_len_;

    buffer_chain* first_chain = &chains_.front();
    //�����һ��chain�Ĵ�С�Ѿ�����size��,��ôֱ�ӷ���
    if (first_chain->size() >= size)
        return static_cast<unsigned char*>(first_chain->get_start_buffer());

    //��һ��chain����
    int64_t remain_to_pullup = size - first_chain->size();

    //���first_chain��free size�������¶�����Ĳ���, ��ô�Ϳ��Բ��ƶ��κ�Ԫ�أ�ֱ�ӿ���������Ĳ���
    if (first_chain->chain_free_space() >= remain_to_pullup)
    {
        //        buffer_chain& next_chain = *first_chain.next();
        //        ::memcpy(first_chain.off_, next_chain.buffer_ + next_chain.misalign_, remain_to_pullup);
        //        first_chain.off_ += remain_to_pullup;
        //        next_chain.misalign_ += remain_to_pullup;
    }
    else//���������¶�����Ĳ���,���·���һ��chain, ��֤��һ��chain size >= size
    {
        buffer_chain chain{ this, static_cast<uint32_t>(size) };
        chain = *first_chain;//�˴�����align, ������chain��next
        chains_.pop_front();
        chains_.push_front(std::move(chain));
    }

    first_chain = &chains_.front();
    buffer_chain* current_chain = first_chain->next();

    //current_chian�����Ǿ���last_chain_with_data_, ����current_chain�������ǿ�chain
    while (current_chain != nullptr && remain_to_pullup >= current_chain->size())
    {
        assert(current_chain->size() > 0);
        remain_to_pullup -= current_chain->size();
        first_chain->append(*current_chain);
        first_chain->next_ = current_chain->next_;
        chains_.erase(++chains_.begin());
        current_chain = first_chain->next_;
    }

    //current_chain == 0 ���� ֻ��Ҫ current_chain �е�һ����
    if (remain_to_pullup != 0)//����һ������Ҫ����
    {
        first_chain->append(*current_chain, static_cast<uint32_t>(remain_to_pullup), current_chain->begin());
        current_chain->misalign_ += remain_to_pullup;
    }

    last_chain_with_data_ = current_chain ? current_chain : first_chain;
    return static_cast<unsigned char*>(first_chain->get_start_buffer());
}

int64_t buffer::remove(/*out*/void* data, uint32_t data_len)
{
    if (data == nullptr || data_len == 0) return -1;
    //������û������ or ���ݲ���
    if (last_chain_with_data_ == nullptr || total_len_ < data_len)  return -1;

    //���ݹ�
    buffer_chain* current_chain = &chains_.front();
    buffer_chain* next_chain = current_chain->next();
    uint32_t remain_to_remove = data_len;
    uint32_t dest_start_pos = 0;

    for (; remain_to_remove > 0; current_chain = next_chain, next_chain = current_chain->next()) {
        if (current_chain->size() <= remain_to_remove)
        {
            //remove this chain
            assert(current_chain != nullptr);
            assert(current_chain->size() > 0);
            ::memcpy(static_cast<char*>(data) + dest_start_pos, current_chain->get_start_buffer(), current_chain->size());
            remain_to_remove -= current_chain->size();
            dest_start_pos += current_chain->size();
            //            assert(last_chain_with_data_ != current_chain);//��һ�������ܵ�ǰchain�������һ����data �� chain��remain_to_remove �͵��� size
            total_len_ -= current_chain->size();
            chains_.pop_front();

            //!!! ERROR ��Ӧ���ж�next�ǲ��ǿ�,��Ӧ���ж� ��ǰ chian�ǲ���last_chain_with_data
            if (/*next_chain == 0*/ current_chain == last_chain_with_data_)  //����ǰchain�Ѿ������һ����ʱ��,next���ǿյ�,��ʱ����Ҫ��ѭ����,�Ѿ�������
            {
                assert(remain_to_remove == 0);
                break;
            }
            continue;
        }

        //remain data to remove && ��ǰ remove  �� ��ǰ chain ����, ���ҵ�ǰchain ����ȫ��remove
        ::memcpy(static_cast<char*>(data) + dest_start_pos, current_chain->get_start_buffer(), remain_to_remove);
        current_chain->misalign_ += remain_to_remove;
        total_len_ -= remain_to_remove;
        break;
    }
    return data_len;
}

//TODO remove the calloc and free
int64_t buffer::drain(uint32_t len)
{
    void* datap = ::calloc(len, 1);
    int64_t ret = remove(datap, len);
    free(datap);
    return ret;
}

int64_t buffer::copy_out_from(void* data, uint32_t data_len, Iter start)
{
    if (!validate_iter(start) || data == nullptr)
        return -1;
    if (data_len == 0) return 0;

    buffer tmp_buf{ *this, data_len, start };//COPY ONCE
    return tmp_buf.remove(data, data_len);//COPY ONCE
}

int64_t buffer::read_line(char* read_out, uint32_t n, buffer_eol_style eol_style)
{
    if (read_out == nullptr || n == 0) return -1;

    uint32_t eol_len = 0;
    auto it = search_eol(&eol_len, eol_style, begin());
    if (!it.is_valid()) return -1;
    //�������n���ֽڣ����ǲ�û�ж������з�����ô�ͷ���-1, ��ʾread_out��������һ�е�����
    if (n < it.offset_of_buffer_) return -1;

    int64_t bytes_read = remove(read_out, it.offset_of_buffer_);
    drain(eol_len);
    return bytes_read;
}

buffer_iter buffer::search(const char* what, uint32_t len, Iter start)
{
    return search_range(what, len, start, end());
}

buffer_iter buffer::search_range(const char* what, uint32_t len, Iter start, Iter end)
{
    if (!validate_iter(start) || !validate_iter(end) || len == 0 || what == nullptr)
    {
        return Iter::NULL_ITER;
    }

    auto* first_chain = start.chain_;
    auto* last_chain = end.chain_;
    uint32_t off = start.offset_of_chain_;
    uint32_t last_off = start.offset_of_chain_;
    auto* current_chain = first_chain;
    auto* next_chain = current_chain->next_;
    const char first = what[0];
    void* target = nullptr;
    uint32_t target_offset_of_chain = 0;
    bool FOUND = false;

    for (;
        current_chain && current_chain->size() > 0;
        current_chain = next_chain, next_chain = current_chain ? current_chain->next_ : nullptr, off = current_chain ? current_chain->misalign_ : 0)
    {
        if (current_chain == last_chain) off = last_off;

        while (true)
        {
            target = ::memchr(current_chain->buffer_ + off, first, current_chain->off_ - off);
            if (target == nullptr) break;
            //in one chain, there could be some first char
            off = static_cast<char*>(target) - current_chain->buffer_ + 1;
            target_offset_of_chain = static_cast<char*>(target) - static_cast<const char*>(current_chain->get_start_buffer());
            //memcmp, ע�����֮����ַ�����һ��chain��
            if (buffer_memcmp(what, len, iter_of_chain(*current_chain) + (target_offset_of_chain - current_chain->misalign_)))
            {
                FOUND = true;
                break;
            }
        }
        if (FOUND) break;
    }

    if (!FOUND)
    {
        return Iter::NULL_ITER;
    }

    return iter_of_chain(*current_chain) + target_offset_of_chain;
}

bool buffer::buffer_memcmp(const char* source, uint32_t len, Iter start)
{
    if (!validate_iter(start) || source == nullptr || len > total_len_ - start.offset_of_buffer_)
    {
        return false;
    }

    const buffer_chain* current_chain = start.chain_;
    uint32_t off = start.offset_of_chain_;
    const buffer_chain* next_chain = current_chain->next_;
    uint32_t remain_to_compare = len;
    uint32_t off_of_source = 0;

    while (current_chain && current_chain->size() > 0 && remain_to_compare > 0) {
        uint32_t size_going_to_compare = remain_to_compare > (current_chain->off_ - off) ? (current_chain->off_ - off) : remain_to_compare;
        if (::memcmp(current_chain->buffer_ + off, source + off_of_source, size_going_to_compare) != 0)
            return false;
        remain_to_compare -= size_going_to_compare;
        off_of_source += size_going_to_compare;

        if (next_chain == nullptr) break;
        current_chain = next_chain;
        next_chain = current_chain->next_;
        off = current_chain->misalign_;
    }

    if (remain_to_compare == 0)  return true;
    return false;
}

buffer_iter buffer::search_eol(uint32_t* eol_len_out, buffer_eol_style eol_style, Iter start)
{
    if (eol_len_out == nullptr || !validate_iter(start) || total_len_ == 0)
    {
        return Iter::NULL_ITER;
    }

    auto it_CRLF = search_range("\r\n", 2, begin(), end());
    auto it_LF = search_range("\n", 1, begin(), end());
    auto it_NUL = search_range("\0", 1, begin(), end());

    Iter it_ret = Iter::NULL_ITER;

    switch (eol_style)
    {
    case buffer_eol_style::BUFFER_EOL_CRLF:
    {
        //��\n, ҲҪ��\r
        if (!it_CRLF.is_valid())
        {
            if (it_LF.is_valid())
            {
                it_ret = it_LF < it_CRLF ? it_LF : it_CRLF;//when it_LF == it_CRLF, return it_CRLF
                *eol_len_out = 2;
            }
        }
        it_ret = it_CRLF;
        *eol_len_out = 1;
        break;
    }
    case buffer_eol_style::BUFFER_EOL_CRLF_STRICT:
    {
        //��\r\n
        it_ret = it_CRLF;
        *eol_len_out = 2;
        break;
    }
    case buffer_eol_style::BUFFER_EOL_LF:
        //��\n
        it_ret = it_LF;
        *eol_len_out = 1;
        break;
    case buffer_eol_style::BUFFER_EOL_NUL:
        //��\0
        it_ret = it_NUL;
        *eol_len_out = 1;
        break;
    }

    return it_ret;
}

int buffer::peek(std::vector<const buffer_iovec*> vec_out, uint32_t len, Iter start)
{
    (void)vec_out;
    (void)len;
    (void)start;
    return 0;
}

inline bool buffer::is_last_chain_with_data(const buffer_chain* current_chain) const
{
    if (current_chain == nullptr || current_chain != last_chain_with_data_)
        return false;
    return true;
}

bool buffer::validate_iter(const Iter& iter) const
{
    //validate buffer
    if (iter.buffer_ != this)
    {
        return false;
    }

    //find the chain
    const buffer_chain* current_chain = &*this->chains_.begin();
    while (current_chain != nullptr && current_chain != iter.chain_)
    {
        current_chain = current_chain->next_;
    }

    //validate the chain
    if (current_chain == iter.chain_)
    {
        if (iter.chain_->off_ == current_chain->off_ &&
            iter.chain_->buffer_ == current_chain->buffer_ &&
            iter.chain_->misalign_ == current_chain->misalign_)
            return true;
    }
    assert(current_chain == nullptr && "any other conditions???");
    return false;
}

buffer_chain* buffer::expand_if_needed(uint32_t data_len)
{
    //no data in buffer at all
    //TODO ERROR last_chain_with_data_ == 0��������û��chain������м����յ�chain���Ϳ���ֱ��ʹ�ã�����Ҫ�·����ڴ�
    if (/*last_chain_with_data_ == 0*/ total_len_ == 0)
    {
        //        free_trailing_empty_chains();//�п��ܳ���buffer��ֻ�пյ�chain, �˴��ǽ���free ��֮�����²���
                //!! TODO ���Ը��Ӹ�Чһ��, ����Ƿ��пյ�chain���оͼ���С��������������£���ֱ��ʹ�ã���������µ�
                //! //Ҳ����ֱ����ǰ������µģ���free��֮����ڴ棬��֮�������в����ʱ�򣬿����ظ�ʹ���ⲿ���ڴ�
        //        assert(chains_.empty() && "chains_ should be empty");
        push_front(buffer_chain{ this, data_len });
        return &chains_.back();
    }

    //if {last_chain_with_data_} has enough space for {data_len} then do nothing
    if (last_chain_with_data_->chain_free_space() >= data_len)
    {
        return last_chain_with_data_;
    }

    //����Ƿ�ֵ����չ
    buffer_chain* lc = last_chain_with_data_;
    if (lc->chain_free_space() < (lc->chain_capacity() / 8) || //(������)��ǰchain ��ʣ��ռ� ֻ�� capacity �� 1/8, resize ��Ҫ�����϶��ڴ�
        lc->get_offset() > buffer_chain::MAXIMUM_SIZE_WHEN_EXPAND ||  // (����ֵ��)chain ���Ѿ����� 4096 ���ֽ���
        data_len >= (buffer_chain::MAXIMUM_CHAIN_SIZE - lc->get_offset())) //���������չ��������չ֮��Ŀ����ڴ滹�������� data_len
    {
        //��˲�������չ��so ��lc->next �İ�
        if (lc->next() && lc->next()->chain_free_space() > data_len)
        {
            //����, �������㹻�Ŀռ����� data_len
            assert(lc->next()->get_offset() == 0 && "lc->next() should be empty");
            return lc->next();
        }
        else {
            //Ҫô�Ǹ���������next, Ҫô��next ����(��Ȼ�ǿյ�)
            free_trailing_empty_chains();
            buffer_chain _chain{ this, data_len };
            push_back(_chain);
        }
    }
    else {
        //now we can resize lc 
        uint32_t length_needed = lc->get_offset() + data_len;
        buffer_chain chain_newed{ this, length_needed };
        //! lc һ�������һ��chainô?? Ϊʲôpop_back()
        chain_newed = *lc;//copy from lc into chain_newed
        chains_.pop_back();
        //pop_back ֮��ȡ�����һ��chain, �ٽ����ڵ����һ��chain ��next ����Ϊ֮��������chain
        last_chain_with_data_ = push_back(chain_newed);
        // last_chain_with_data_ = &chains_.back();
    }
    return &chains_.back();
}

buffer_chain* buffer::free_trailing_empty_chains()
{
    buffer_chain* chain = last_chain_with_data_;
    //no data at all
    if (total_len_ == 0)
    {
        chains_.clear(); return nullptr;
    }
    assert(chain->get_offset() > 0);

    auto start_iter = chains_.begin();
    while (&*start_iter != chain)
    {
        ++start_iter;
    }

    auto back_iter = chains_.end();
    --back_iter;
    while (back_iter != start_iter) {
        chains_.pop_back();
        chains_.back().set_next_chain(nullptr);
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
    return last_chain_with_data_;
}

void buffer::update_next_field_after_copy()
{
    //����һ��chains_ ����ʱ, ÿ��chain ��nextҲ���ǿ�����, ���next field�ǲ��Ե�,
    //ͨ������һ�����ÿ���ڵ��next
    if (chains_.size() <= 1) return;
    buffer_chain* current_chain = &chains_.front();
    for (auto it = ++chains_.begin(); it != chains_.end(); it++)
    {
        current_chain->next_ = &*it;
        current_chain = &*it;
    }
    chains_.back().next_ = nullptr;
}
