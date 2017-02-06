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
UCharArray<array_length>::UCharArray(const std::vector<unsigned char>& rawBytes) {
    setRaw(rawBytes);
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const std::string &hexString) {
    setRawHex(hexString);
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const unsigned char* rawBytes) {
    setRaw(rawBytes);
}

template<unsigned int array_length>
void UCharArray<array_length>::reverse() {
    std::reverse(this->begin(), this->end());
}

template<unsigned int array_length>
unsigned int UCharArray<array_length>::rawLength() {
    return array_length;
}

template<unsigned int array_length>
void UCharArray<array_length>::clear() {

    for(unsigned int i = 0; i < array_length; i++)
        this->at(i) = 0;
}

template<unsigned int array_length>
bool UCharArray<array_length>::isClear() const {

    for(int i = 0; i < array_length; i++) {
        if(this[i] != 0)
            return false;
    }

    return true;
}

template<unsigned int array_length>
std::string UCharArray<array_length>::getRawHex() const {

    return uchar_vector(this->data(), array_length).getHex();
}

template<unsigned int array_length>
uchar_vector UCharArray<array_length>::toUCharVector() const {

    // Get pointer to data
    const unsigned char * data = static_cast<const unsigned char *>(this->data());

    // Construct vector and return it
    return uchar_vector(data, array_length);
}

template<unsigned int array_length>
void UCharArray<array_length>::setRawHex(std::string hexString) {
    static std::string HEXCHARS("0123456789abcdefABCDEF");


    // pad on the left if hex contains an odd number of digits.
    if (hexString.size() % 2 == 1)
        hexString = "0" + hexString;

    // Check that string has correct length - must be zero padded
    if(hexString.size() != 2*array_length) {

        // Create error message
        std::stringstream err;

        err << "String argument is of incorrect length, should be "
          << 2*array_length
          << ", but was "
          << hexString.length()
          << ".";

        throw std::runtime_error(err.str());

    }

    uint byte;
    std::string nibbles;

    for(unsigned int i = 0, j = 0; j < array_length; i += 2, j++) {
       nibbles = hexString.substr(i, 2);

      // verify we have valid hex characters
      if(HEXCHARS.find(nibbles[0]) == std::string::npos ||
         HEXCHARS.find(nibbles[1]) == std::string::npos) {
        throw std::runtime_error("Invalid characters in hex string");
      }

      sscanf(nibbles.c_str(), "%x", &byte);
      this->at(j) = byte;
    }
}

template<unsigned int array_length>
void UCharArray<array_length>::setRaw(const unsigned char * start) {

    // Copy content into array
    for(unsigned int i = 0; i < array_length; i++)
        this->at(i) = start[i];
}

template<unsigned int array_length>
void UCharArray<array_length>::setRaw(const std::vector<unsigned char> & rawBytes) {

    // Check that vector has correct length
    if(rawBytes.size() != array_length) {

        // Create error message
        std::stringstream err;

        err << "vector argument is of incorrect size, should be "
          << array_length
          << ", but was "
          << rawBytes.size()
          << ".";

        throw std::runtime_error(err.str());

    }

    setRaw(rawBytes.data());
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

}
