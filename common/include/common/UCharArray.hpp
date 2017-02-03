/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef COIN_UCHAR_ARRAY_HPP
#define COIN_UCHAR_ARRAY_HPP

#include <array>
#include <vector>
#include <string>
#include <iostream>

class uchar_vector;

namespace Coin {

// Forward declaration for stream operators
template<unsigned int array_length>
class UCharArray;

// Stream operator
template<unsigned int array_length>
std::ostream & operator<<(std::ostream & stream, const UCharArray<array_length> & o);

template<unsigned int array_length>
std::istream & operator>>(std::istream & stream, UCharArray<array_length> & o);

// (type safe length) fixed length array of unsigned chars
template<unsigned int array_length>
class UCharArray : public std::array<unsigned char, array_length> {

public:

    // Default constructor
    UCharArray();
    UCharArray(const std::vector<unsigned char>&);
    UCharArray(const std::string&);
    UCharArray(const unsigned char*);

    // TODO : make toUCharVector(), getRawHex() and length() protected to be used only by derived classes
    // as they deal with internal representation. Derived classes should have equivalent public methods
    // for different encoding types
    // Convert to unsigned char vector
    uchar_vector toUCharVector() const;  //lets get rid of this if possible

    // Encodes as hex string
    std::string getRawHex() const;

    // Return length
    static unsigned int rawLength();

    // Clear array, i.e. , set to 0s
    void clear();

    // Check if array is clear
    bool isClear() const;

    // Stream operator
    friend std::ostream & operator<< <array_length> (std::ostream & stream, const UCharArray<array_length> & o);
    friend std::istream & operator>> <array_length> (std::istream & stream, UCharArray<array_length> & o);

protected:
    // we are taking a copy not a reference to do 0 padding
    void setRawHex(std::string);

    // Try to fill array with content starting at start
    void setRaw(const unsigned char* start);

    // Copy data from vector into array - size is checked
    void setRaw(const std::vector<unsigned char>&);

    // Reverses representation, to switch between little/big-endian
    void reverse();
};

}

// Needed due to c++ needing implementation for all uses of templated types
#include <common/../../src/UCharArray.cpp>

#endif // COIN_UCHAR_ARRAY_HPP
