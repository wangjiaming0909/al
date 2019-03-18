#ifndef _BUFFER_H_
#define _BUFFER_H_

//** 1, lock or not lock

class buffer
{
private:
    /* data */
public:
    buffer(/* args */);
    ~buffer();

public:
    //* return number of bytes stored in the buffer
    size_t buffer_length();
    //* return number of bytes stored in the first chunk
    size_t first_chunk_length();

    //* add the data to the end of the buffer
    template <typename T>
    int append(const T* data);
};

buffer::buffer(/* args */)
{
}

buffer::~buffer()
{
}

size_t buffer::buffer_length()
{

}


#endif // _BUFFER_H_