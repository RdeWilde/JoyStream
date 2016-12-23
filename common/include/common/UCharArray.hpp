/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef COIN_UCHAR_ARRAY_HPP
#define COIN_UCHAR_ARRAY_HPP

#include <array>
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

    // Construct form unsigned char vector
    UCharArray(const uchar_vector & vector);

    // Constructor from hex encoded string
    UCharArray(const std::string & hexEncodedString);

    // Constructor from hex encoded string
    UCharArray(const char * hexEncodedString);

    // Reverses representation, to switch between little/big-endian
    void reverse();

    // Return length
    static unsigned int length();

    // Clear array, i.e. , set to 0s
    void clear();

    // Check if array is clear
    bool isClear() const;

    // Encodes as hex string
    std::string toHex() const;

    // Convert to unsigned char vector
    uchar_vector toUCharVector() const;

    // Stream operator
    friend std::ostream & operator<< <array_length> (std::ostream & stream, const UCharArray<array_length> & o);
    friend std::istream & operator>> <array_length> (std::istream & stream, UCharArray<array_length> & o);

    /**
    // Comparison for use with QMap
    bool operator<(const FixedUCharArray & o) const;
    bool operator==(const FixedUCharArray & o) const;
    bool operator!=(const FixedUCharArray & rhs) const;
    */

protected:

    // Try to fill array with content starting at start
    void fill(const unsigned char * start);
};

template<unsigned int array_length>
unsigned int qHash(const UCharArray<array_length> & array);

}

// Needed due to c++ needing implementation for all uses of templated types
#include <common/../../src/UCharArray.cpp>

#endif // COIN_UCHAR_ARRAY_HPP
