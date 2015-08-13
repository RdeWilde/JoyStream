/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef UCHAR_ARRAY_HPP
#define UCHAR_ARRAY_HPP

#include <array>

class QByteArray;
class QString;
class uchar_vector;
class QDataStream;

namespace Coin {

// (type safe length) fixed length array of unsigned chars
template<unsigned int array_length>
class UCharArray : public std::array<unsigned char, array_length> {

public:

    // Default constructor
    UCharArray();

    // Construct form unsigned char vector
    UCharArray(const uchar_vector & vector);

    // Constructor from hex encoded string
    UCharArray(const QString & hexEncoded);

    // Constructor from raw byte array
    UCharArray(const QByteArray & byteArray);

    // Return length
    static unsigned int length();

    // Clear array, i.e. , set to 0s
    void clear();

    // Check if array is clear
    bool isClear() const;

    // Encodes as hex string
    QString toHex() const;

    // Convert to unsigned char vector
    uchar_vector toUCharVector() const;

    // Convert to byte array
    QByteArray toByteArray() const;

    // Stream operator
    friend QDataStream & operator<<(QDataStream & stream, const UCharArray & o);
    friend QDataStream & operator>>(QDataStream & stream, UCharArray & o);

    /**
    // Comparison for use with QMap
    bool operator<(const FixedUCharArray & o) const;
    bool operator==(const FixedUCharArray & o) const;
    bool operator!=(const FixedUCharArray & rhs) const;
    */

private:

    // Try to fill array with content starting at start
    void fill(const unsigned char * start, int length);
};

template<unsigned int array_length>
uint qHash(const UCharArray<array_length> & array);

}

// Needed due to c++ needing implementation for all uses of templated types
//#include "src/FixedUCharArray.cpp"

/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#include <QByteArray>
#include <QDataStream>
#include <QString>

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

        s << "vector argument is of incorrect length, should be"
          << array_length
          << ", but was "
          << vectorLength;

        throw std::runtime_error(s.str());

    } else
        fill(static_cast<const unsigned char *>(vector.data()), vectorLength);
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const QString & hexEncoded) {

    // Check that string has correct length
    if(hexEncoded.length() != 2*array_length) {

        // Create error message
        std::stringstream s;

        s << "String argument is of incorrect length, should be"
          << 2*array_length
          << ", but was "
          << hexEncoded.length();

        throw std::runtime_error(s.str());

    } else {

        // Decode from hex format
        QByteArray b = QByteArray::fromHex(hexEncoded.toLatin1());

        Q_ASSERT(b.length() == array_length);

        // Copy into array
        // Does not work => static_cast<const unsigned char *>(b.constData())
        fill((const unsigned char *)(b.constData()), array_length);
    }
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const QByteArray & byteArray) {

    // Check that byte array has correct length
    uchar_vector::size_type byteArrayLength = byteArray.size();

    if(byteArrayLength != array_length) {

        // Create error message
        std::stringstream s;

        s << "vector argument is of incorrect length, should be"
          << array_length
          << ", but was "
          << byteArrayLength;

        throw std::runtime_error(s.str());

    } else
        fill(static_cast<const unsigned char *>(byteArray.constData()), byteArrayLength);
}

template<unsigned int array_length>
unsigned int UCharArray<array_length>::length() {
    return array_length;
}

template<unsigned int array_length>
void UCharArray<array_length>::clear() {

    for(int i = 0;i < array_length;i++)
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
QString UCharArray<array_length>::toHex() const {

    const char * ptr = (const char *)(this->data()); // this conversion is not safe ? think

    // Wrap in byte array
    QByteArray byteArray(ptr, array_length);

    // Convert to hex and return
    return QString(byteArray.toHex());
}

template<unsigned int array_length>
uchar_vector UCharArray<array_length>::toUCharVector() const {

    // Get pointer to data
    const unsigned char * data = static_cast<const unsigned char *>(this->data());

    // Construct vector and return it
    return uchar_vector(data, array_length);
}

template<unsigned int array_length>
QByteArray UCharArray<array_length>::toByteArray() const {

    // Get pointer to data
    const char * data = reinterpret_cast<const char *>(this->data());

    // Construct byte array and return it
    return QByteArray(data, array_length);
}

template<unsigned int array_length>
void UCharArray<array_length>::fill(const unsigned char * start, int length) {

    if(length != array_length) {

        std::stringstream s;

        s << "Required "
          << array_length
          << " bytes, but was provided"
          << length;

        throw std::runtime_error(s.str());
    } else {

        // Copy content into array
        for(int i = 0;i < array_length;i++)
            this->at(i) = start[i];
    }
}

template<unsigned int array_length>
QDataStream & operator<<(QDataStream& stream, const Coin::UCharArray<array_length> & o) {

    // Write to stream from buffer
    int bytesWritten = stream.writeRawData((const char *)(o.data()), array_length);

    if(bytesWritten != array_length) {

        // Create error message
        std::stringstream s;

        s << "Failed writing" << array_length << " bytes to the stream.";

        throw new std::runtime_error(s.str());

    }

    return stream;
}

template<unsigned int array_length>
QDataStream & operator>>(QDataStream& stream, Coin::UCharArray<array_length> & o) {

    // Read from stream to array
    int bytesRead = stream.readRawData((char *)(o.data()), array_length);

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

#endif // UCHAR_ARRAY_HPP
