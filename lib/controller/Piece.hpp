#ifndef PIECE_HPP
#define PIECE_HPP

#ifndef Q_MOC_RUN
#include <boost/shared_array.hpp>
#endif Q_MOC_RUN

class Piece {

public:

    // Constructor
    Piece(int index, int length, const boost::shared_array<char> & data);

    // Getters and setters
    int index() const;
    void setIndex(int index);

    int length() const;
    void setLength(int length);

    boost::shared_array<char> data() const;
    void setData(const boost::shared_array<char> &data);

private:

    // Index of piece
    int _index;

    // Byte length of piece
    int _length;

    // Raw data in piece
    boost::shared_array<char> _data;
};

#endif // PIECE_HPP
