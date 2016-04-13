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
#include <string>

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
UCharArray<array_length>::UCharArray(const QString & hexEncoded) {

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

        // Decode from hex format
        QByteArray b = QByteArray::fromHex(hexEncoded.toLatin1());

        Q_ASSERT(b.length() == array_length);

        // Copy into array
        // Does not work => static_cast<const unsigned char *>(b.constData())
        fill((const unsigned char *)(b.constData()));
    }
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const char * hexEncodedString)
    : UCharArray<array_length>(QString(hexEncodedString)) {
}

template<unsigned int array_length>
UCharArray<array_length>::UCharArray(const QByteArray & raw) {

    // Check that byte array has correct length
    uchar_vector::size_type byteArrayLength = raw.size();

    if(byteArrayLength != array_length) {

        // Create error message
        std::stringstream s;

        s << "vector argument is of incorrect length, should be "
          << array_length
          << ", but was "
          << byteArrayLength
          << ".";

        throw std::runtime_error(s.str());

    } else
        fill((const unsigned char *)(raw.constData()));
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
void UCharArray<array_length>::fill(const unsigned char * start) {

    // Copy content into array
    for(unsigned int i = 0;i < array_length;i++)
        this->at(i) = start[i];
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
