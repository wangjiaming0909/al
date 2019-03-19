#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <cstdarg>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>

#define LOG(level) std::cout << (level)
#define WARNING "WARNING"

enum class buffer_eol_style{
    BUFFER_EOL_LF, //'\n'
    BUFFER_EOL_CRLF_STRICT, //"\r\n"
    BUFFER_EOL_CRLF, //'\n' or "\r\n"
    BUFFER_EOL_NUL // ASCII NUL
};

class buffer;
class buffer_chain;

class buffer_iter{
    friend class buffer;
protected:
    buffer_iter(
        buffer* buffer_ptr, 
        buffer_chain* chunk, 
        size_t offset_of_buffer, 
        size_t chunk_number, 
        size_t offset_of_chunk);
public:
    //the position from the start of buffer
    size_t pos() const {return offset_of_buffer_;}
    const buffer* get_buffer() const {return buffer_;}

    //manipulates the {pos_}, success returns 0, error returns -1
    //! any rearranging of the buffer could invalidate all iter
    //TODO add operators for iter
private:
    buffer*             buffer_;
    size_t              offset_of_buffer_;
    size_t              chunk_number_;
    size_t              offset_of_chunk_;
    buffer_chain*       chunk_;
};

struct buffer_iovec{
    void*       iov_base;
    size_t      iob_len;
};

class buffer_chain{
public:
    buffer_chain(size_t capacity = DEFAULT_CHUNK_SIZE);
    ~buffer_chain();
    buffer_chain(const buffer_chain& other);
    //* note that if(this->capacity_ > other.capacity_), 
    //* this function will not change the capacity of this
    buffer_chain& operator= (const buffer_chain& other);

public:
    size_t chain_capacity() const { return capacity_; }
    void* buffer() { return buffer_; }
    void set_next_chain(buffer_chain* next) {next_ = next;}

private:
    // 内存分配策略: precondition(given_capacity > 0)
    //    如果capacity > MAXIMUM_CHUNK_SIZE / 2, 直接分配内存
    //    如果capacity < MAXIMUM_CHUNK_SIZE / 2, 那么以 1024 的偶数倍递增
    size_t calculate_actual_capacity(size_t given_capacity);
public:
    static const size_t DEFAULT_CHUNK_SIZE = 1024;
    static const size_t MAXIMUM_CHUNK_SIZE = __INT32_MAX__;
private:
    void*               buffer_;
    size_t              capacity_;
    buffer_chain*       next_;
};

//** 1, lock or not lock

class buffer
{
public:
    using Iter = buffer_iter;
public:
    buffer(/* args */);
    ~buffer();
    buffer(const buffer& other);
    buffer(const buffer&& other);
    //copy {dataLen} data to {this} from {other}
    buffer(const buffer& other, size_t dataLen);
    //copy {dataLen} data to {this} from {start} to {start + dataLen} in {other}
    buffer(const buffer& other, size_t dataLen, Iter* start);
    buffer& operator=(const buffer& other);

public:
    //* return number of bytes stored in the buffer
    size_t buffer_length();
    //* return number of bytes stored in the first chunk
    size_t first_chunk_length();

    //* add the data to the end of the buffer
    template <typename T>
    int append(const T& data);
    int append(const buffer& other, size_t dataLen);
    //append {dataLen} bytes from other, start from {start}
    int append(const buffer& other, size_t dataLen, Iter* start);
    int append_printf(const char* fmt, ...);
    int append_vprintf(const char* fmt, va_list ap);

    template <typename T>
    int prepend(const T& data);
    int prepend(const buffer& other, size_t dataLen);
    //prepend {dataLen} bytes from other, start from {start}
    int prepend(const buffer& other, size_t dataLen, Iter* start);

    //alters the last chunk of the memory in the buffer, 
    //or add a chunk so that the buffer is large enough to add dataLen bytes without any allocation
    int expand(size_t dataLen);

    //"linearizes" the first size bytes of this, to ensure that they are all contiguous and occupying the same chunk of memory
    //if size is negative, the function lineratizes the entire buffer
    //if size is greater than the number of bytes in the buffer, the function returns null
    //otherwise pullup returns the first byte in the buffer
    //* note that if the size is the same as first_chunk_length will do nothing
    unsigned char* pullup(size_t size);

    //remove the first datalen bytes to the {data}
    //if total length is small than dataLen, all data will be copied
    //error returns -1, success will return bytes that copied
    int remove(/*out*/void* data, size_t dataLen);
    //behave the same as remove but do not return the removed data, just remove the first {len} bytes
    int drain(size_t len);
    int copy_out_from(void* data, size_t dataLen, Iter* start);
    char* read_line(size_t *n_read_out, buffer_eol_style eol_style);

    //search
    buffer_iter search(const char* what, size_t len, const Iter* start = 0);
    buffer_iter search_range(const char* what, size_t len, const Iter* start = 0, const Iter* end = 0);
    //detect line_endings as read_line, but do not copy out the line, returns a iter to the start of the end-of-line character(s)
    //if {eol_len_out} is non-NULL, it is set to the length of the EOL string
    buffer_iter search_eol(const buffer_iter* start, size_t* eol_len_out, buffer_eol_style eol_style);

    //inspecting data without cpoying, returns bytes that returned
    int peek(size_t len, Iter* start, std::vector<const buffer_iovec*> vec_out);
private:
    buffer_chain*                   first_chain_;
    buffer_chain*                   last_chain_;
    buffer_chain*                   last_chain_with_data_;//最后一个有数据的chain
    size_t                          total_len_;
};

template <typename T>
int buffer::append(const T& data)
{

}

template <typename T>
int buffer::prepend(const T& data)
{

}

#endif // _BUFFER_H_