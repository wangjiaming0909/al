#ifndef _STRING_PIECE_H_
#define _STRING_PIECE_H_

namespace string_piece
{

class string_piece
{
public:
    string_piece(const char* str);
    ~string_piece();
    string_piece(const string_piece& other);
    string_piece& operator=(const string_piece& other);
private:
    // const char*
};

string_piece::string_piece(const char* str)
{
}

string_piece::~string_piece()
{
}

string_piece::string_piece(const string_piece& other)
{

}

string_piece& string_piece::operator=(const string_piece& other)
{

}
}
#endif //_STRING_PIECE_H_

