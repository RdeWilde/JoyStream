/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#include <common/Utilities.hpp>
#include <stdutils/uchar_vector.h>

#include <sstream> // stringstream

namespace Coin {

template<unsigned int array_length>
UCharArray<array_length>::UCharArray() {
    clear();
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const uchar_vector & vector) {

    // Check that vector has correct length
    uchar_vector::size_type vectorLength = vector.size();

    if(vectorLength != array_length) {

        // Create error message
        std::stringstream s;

        s << "vector argument is of incorrect length, should be "
          << array_length
          << ", but was "
          << vectorLength
          << ".";

        throw std::runtime_error(s.str());

    } else {
        fill(static_cast<const unsigned char *>(vector.data()));
    }
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const std::string & hexEncoded) {

    // Check that string has correct length
    if(hexEncoded.length() != 2*array_length) {

        // Create error message
        std::stringstream s;

        s << "String argument is of incorrect length, should be "
          << 2*array_length
          << ", but was "
          << hexEncoded.length()
          << ".";

        throw std::runtime_error(s.str());

    } else {
        UCharArray<array_length>(Coin::toUCharVector(hexEncoded));
    }
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const char * hexEncodedString)
    : UCharArray<array_length>(std::string(hexEncodedString)) {
}

template<unsigned int array_length>
void UCharArray<array_length>::reverse() {
    std::reverse(this->begin(), this->end());
}

template<unsigned int array_length>
unsigned int UCharArray<array_length>::length() {
    return array_length;
}

template<unsigned int array_length>
void UCharArray<array_length>::clear() {

    for(unsigned int i = 0;i < array_length;i++)
        this->at(i) = 0;
}

template<unsigned int array_length>
bool UCharArray<array_length>::isClear() const {

    for(int i = 0;i < array_length;i++) {
        if(this[i] != 0)
            return false;
    }

    return true;
}

template<unsigned int array_length>
std::string UCharArray<array_length>::toHex() const {

    const char * ptr = (const char *)(this->data()); // this conversion is not safe ? think
    return std::string(ptr);
}

template<unsigned int array_length>
uchar_vector UCharArray<array_length>::toUCharVector() const {

    // Get pointer to data
    const unsigned char * data = static_cast<const unsigned char *>(this->data());

    // Construct vector and return it
    return uchar_vector(data, array_length);
}

template<unsigned int array_length>
void UCharArray<array_length>::fill(const unsigned char * start) {

    // Copy content into array
    for(unsigned int i = 0;i < array_length;i++)
        this->at(i) = start[i];
}

template<unsigned int array_length>
std::ostream & operator<<(std::ostream& stream, const Coin::UCharArray<array_length> & o) {

    int bytesWritten;

    // Get initial position of pointer the stream
    auto before = stream.tellp();

    // Write to stream from buffer
    stream.write((const char *)(o.data()), array_length);

    // Get position of pointer after writing to stream
    auto after = stream.tellp();

    if(after != -1 && before != -1)
        bytesWritten = after - before;
    else bytesWritten = -1;

    if(bytesWritten != array_length) {

        // Create error message
        std::stringstream s;

        s << "Failed writing" << array_length << " bytes to the stream.";

        throw new std::runtime_error(s.str());

    }

    return stream;
}

template<unsigned int array_length>
std::istream & operator>>(std::istream& stream, Coin::UCharArray<array_length> & o) {

    // Read from stream to array
    stream.read((char *)(o.data()), array_length);
    int bytesRead = stream.gcount();

    if(bytesRead != array_length) {

        // Create error message
        std::stringstream s;

        s << "Could not read " << array_length << " bytes from stream.";

        throw new std::runtime_error(s.str());
    }

    return stream;
}

/**
template<unsigned int array_length>
bool FixedUCharArray<array_length>::operator<(const FixedUCharArray<array_length> & o) const {
    // 0 is most significant byte
    for(unsigned int i = 0;i < length;i++) {
        unsigned char a = this->at(i);
        unsigned char b = o[i];
        if(a > b)
            return false;
        else if(a < b)
            return true;
    }
    return false;
}
template<unsigned int array_length>
bool FixedUCharArray<array_length>::operator==(const FixedUCharArray<array_length> & o) const {
    return _buffer == o.buffer();
}
*/

template<unsigned int array_length>
uint qHash(const Coin::UCharArray<array_length> & o) {
    return qHash(o.toHex());
}

}
