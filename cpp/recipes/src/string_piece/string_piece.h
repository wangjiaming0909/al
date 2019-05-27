#ifndef _STRING_PIECE_H_
#define _STRING_PIECE_H_

#include <string>
#include <cstring>
#include <algorithm>

namespace string_piece
{
template <typename Iter>
class Range
{
public:

    using it_traits = std::iterator_traits<Iter>;
    using value_type = typename it_traits::value_type;
    using pointer = typename it_traits::pointer;
    using difference_type = typename it_traits::difference_type;
    using reference = typename it_traits::reference;

    using iterator = Iter;
    using const_iterator = std::add_const_t<iterator>;
    using size_type = std::size_t;
public:
    Range() : begin_(), end_(){}
    Range(iterator begin, iterator end) : begin_(begin), end_(end){}
    Range(iterator begin, size_type size) : begin_(begin), end_(begin + size){}
    Range(std::string& str) : begin_(str.data()), end_(str.data() + str.size()) {}
    Range(iterator begin) : begin_(begin), end_(begin_ + ::strlen(begin_)){}

    Range(const Range&) = default;
    Range(Range&&) = default;

public:
    iterator begin() {return begin_;}
    const_iterator cbegin() const {return begin_;}
    iterator end() {return end_;}
    const_iterator cend() const {return end_;}

    size_type find(value_type value)
    {
        auto it = std::find(begin(), end(), value);
        return it == end_ ? std::string::npos : (it - begin_);
    }

    size_type find(iterator it)
    {

    }

    // iterator operator+()

private:
    iterator begin_;
    iterator end_;
};

using mutable_string_piece = Range<char*>;
using const_string_piece = Range<const char*>;

}
#endif //_STRING_PIECE_H_

