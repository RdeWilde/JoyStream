/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "Signature.hpp"
#include <QDataStream>

#include "base58.hpp"

Signature::Signature(){
}

Signature::Signature(const Signature & signature)
    : _buffer(signature.buffer()) {
}

Signature::Signature(const QString & string) {

    // Check that string has correct length
    if(string.length() > 2*maxLength)
        throw std::runtime_error("String argument is of incorrect length, should be 2*MAX_SIGNATURE_LENGTH.");
    else {

        // Decode from hex format
        QByteArray b = QByteArray::fromHex(string.toLatin1());

        Q_ASSERT(b.length()*2 == string.length());

        // Read into vector buffer
        unsigned const char * begin = (unsigned const char *)b.constBegin();
        unsigned const char * end = (unsigned const char *)b.constEnd();
        _buffer = std::vector<unsigned char>(begin, end);
    }
}

Signature & Signature::operator=(const Signature & signature) {
    _buffer = signature.buffer();
    return *this;
}

std::vector<unsigned char> Signature::buffer() const {
    return _buffer;
}

void Signature::setBuffer(const std::vector<unsigned char> & buffer) {
    _buffer = buffer;
}

int Signature::length() const {
    return _buffer.size();
}

QString Signature::toString() const {

    // Put into byte array
    const char * begin = (const char *)_buffer.data();
    QByteArray raw(begin, _buffer.size());

    // Hex encode
    QByteArray hexEncoded = raw.toHex();

    // Return as string
    return QString(hexEncoded.constData());
}

int Signature::readFromStream(QDataStream & stream, int length) {

    // Check that signature is not to large
    if(length > maxLength)
        throw std::runtime_error("Length argument is to large, should be MAX_SIGNATURE_LENGTH.");

    // Allocate buffer
    _buffer = std::vector<unsigned char>(length, 0);

    // Read from stream
    char * data = (char *)_buffer.data();
    int bytesRead = stream.readRawData(data, length);

    if(bytesRead != length)
        throw new std::runtime_error("Could not read length bytes.");

    return bytesRead;
}

int Signature::writeToStream(QDataStream & stream) const {

    if(_buffer.size() == 0)
        return 0;

    // Write to stream
    const char * data = (const char *)_buffer.data();
    int bytesWritten = stream.writeRawData(data, _buffer.size());

    if(bytesWritten != _buffer.size())
        throw new std::runtime_error("Could not write length bytes.");

    return bytesWritten;
}
