#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <cstdarg>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cstring>

#define DEFAULT_CHUNK_SIZE 128

enum class buffer_eol_style{
    BUFFER_EOL_LF, //'\n'
    BUFFER_EOL_CRLF_STRICT, //"\r\n"
    BUFFER_EOL_CRLF, //'\n' or "\r\n"
    BUFFER_EOL_NUL // ASCII NUL
};

class buffer;

class buffer_iter{
    friend class buffer;
protected:
    buffer_iter(buffer* buffer_ptr, size_t offset_of_buffer, size_t chunk_number, size_t offset_of_chunk);
public:
    //the position from the start of buffer
    size_t pos() const {return offset_of_buffer_;}
    const buffer* get_buffer() const {return buffer_;}
private:
    buffer*     buffer_;
    size_t      offset_of_buffer_;
    size_t      chunk_number_;
    size_t      offset_of_chunk_;
};

struct buffer_iovec{
    void*       iov_base;
    size_t      iob_len;
};

class buffer_chunk{
public:
    buffer_chunk(size_t capacity = DEFAULT_CHUNK_SIZE);
    ~buffer_chunk();
    buffer_chunk(const buffer_chunk& other);
    buffer_chunk& operator= (const buffer_chunk& other);

public:
    size_t chunk_capacity() const { return capacity_; }
    void* data() { return data_; }
private:
    size_t      capacity_;
    void*       data_;
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

    //manipulates the {pos_}, success returns 0, error returns -1
    //! any rearranging of the buffer could invalidate all iter
    //set the {pos_} to {pos}
    int set_pos(const Iter& pos);
    //add the {pos_} by position
    int add_pos(size_t position);

    //inspecting data without cpoying, returns bytes that returned
    int peek(size_t len, Iter* start, std::vector<const buffer_iovec*> vec_out);
private:
    Iter                            pos_;
    std::vector<buffer_chunk*>      chain_;
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