#ifndef PIECE_HPP
#define PIECE_HPP

#ifndef Q_MOC_RUN
#include <boost/shared_array.hpp>
#endif Q_MOC_RUN

class Piece {

public:

    // Default constructor
    Piece();

    // Constructor
    Piece(int index, int length, bool hasValidData, const boost::shared_array<char> & data);

    // Copy constructor
    Piece(const Piece & piece);

    // Getters and setters
    int index() const;
    void setIndex(int index);

    int length() const;
    void setLength(int length);

    bool hasValidData() const;
    void setHasValidData(bool hasValidData);

    boost::shared_array<char> data() const;
    void setData(const boost::shared_array<char> &data);

private:

    // Index of piece
    int _index;

    // Byte length of piece
    int _length;

    // Whether _data is valid
    bool _hasValidData;

    // Raw data in piece
    boost::shared_array<char> _data;
};

#endif // PIECE_HPP
