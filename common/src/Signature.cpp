/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#include <common/Signature.hpp>

#include <QString>
#include <QDataStream>

#include <sstream>

namespace Coin {

Signature::Signature() {
}

Signature::Signature(const uchar_vector & raw) {

    // Check that input is no longer than
    uchar_vector::size_type inputLength = raw.size();

    if(inputLength > maxLength) {

        // Create error message and throw exception
        std::stringstream s;

        s << "Input argument must be no longer than"
          << maxLength
          << ", but was of length "
          << inputLength;

        throw std::runtime_error(s.str());
    } else
        setRaw(raw);

}

Signature::Signature(const QString & string) {

    // Check that string has correct length

    int stringLength = string.length();

    if(stringLength > 2*maxLength)
        throw std::runtime_error("String argument is of incorrect length, should be 2*MAX_SIGNATURE_LENGTH.");
    else {

        // Decode from hex format
        QByteArray b = QByteArray::fromHex(string.toLatin1());

        Q_ASSERT(b.length()*2 == stringLength);

        // Read into vector buffer
        unsigned const char * begin = (unsigned const char *)b.constBegin();
        unsigned const char * end = (unsigned const char *)b.constEnd();
        _raw = std::vector<unsigned char>(begin, end);
    }
}

Signature::Signature(const Signature & signature)
    : _raw(signature.raw()) {
}

Signature::~Signature() {
    _raw.clear();
}

Signature & Signature::operator=(const Signature & signature) {
    _raw = signature.raw();
    return *this;
}

bool Signature::operator==(const Signature & rhs) {
    return _raw == rhs.raw();
}

unsigned int Signature::length() const {
    return _raw.size();
}

QString Signature::toString() const {

    // Put into byte array
    const char * begin = (const char *)_raw.data();
    QByteArray raw(begin, _raw.size());

    // Hex encode
    QByteArray hexEncoded = raw.toHex();

    // Return as string
    return QString(hexEncoded.constData());
}

QByteArray Signature::toByteArray() const {

    // Get pointer to data
    const char * data = reinterpret_cast<const char *>(_raw.data());

    // Construct byte array and return it
    return QByteArray(data, _raw.size());
}

uchar_vector Signature::toUCharVector() const {

    // Get pointer to data
    const unsigned char * data = reinterpret_cast<const unsigned char *>(_raw.data());

    // Construct byte array and return it
    return uchar_vector(data, _raw.size());
}

int Signature::readFromStream(QDataStream & stream, int length) {

    // Check that signature is not to large
    if(length > maxLength)
        throw std::runtime_error("Length argument is to large, should be MAX_SIGNATURE_LENGTH.");

    // Allocate buffer
    _raw = std::vector<unsigned char>(length, 0);

    // Read from stream
    char * data = (char *)_raw.data();
    int bytesRead = stream.readRawData(data, length);

    if(bytesRead != length)
        throw new std::runtime_error("Could not read length bytes.");

    return bytesRead;
}

int Signature::writeToStream(QDataStream & stream) const {

    if(_raw.size() == 0)
        return 0;

    // Write to stream
    const char * data = (const char *)_raw.data();
    int bytesWritten = stream.writeRawData(data, _raw.size());

    if(bytesWritten != _raw.size())
        throw new std::runtime_error("Could not write length bytes.");

    return bytesWritten;
}

uchar_vector Signature::raw() const {
    return _raw;
}

void Signature::setRaw(const uchar_vector & buffer) {
    _raw = buffer;
}

}
