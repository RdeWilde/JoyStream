/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "PrivateKey.hpp"
#include "base58.hpp"

#include <QDataStream>
#include <QHash>

PrivateKey::PrivateKey()
    : _buffer(length, 0) {
}

PrivateKey::PrivateKey(const PrivateKey & o)
    : _buffer(o.buffer()) {
}

PrivateKey::PrivateKey(const QString & string)
    : _buffer(length, 0) {

    // Turn into std::string
    std::string stdString = string.toStdString();

    // Base58 base decode into buffer
    bool result = DecodeBase58(stdString, _buffer);

    // Check that decoding worked
    if(!result)
        throw std::exception("Decoding base58 did not work.");
}

PrivateKey & PrivateKey::operator=(const PrivateKey & o) {
    _buffer = o.buffer();
    return *this;
}

void PrivateKey::clear() {

    for(unsigned int i = 0;i < length;i++)
        _buffer[i] = 0;
}

bool PrivateKey::isClear() const {

    // Find non zero byte
    for(int i = 0;i < length;i++) {
        if(_buffer[i] != 0)
            return false;
    }

    // We didn't, so it is clear
    return true;
}

QString PrivateKey::toString() const {

    // Encode as base58 string
    std::string result = EncodeBase58(_buffer);

    return QString::fromStdString(result);
}

QDataStream & operator<<(QDataStream& stream, const PrivateKey & o) {

    // Write to stream from buffer
    std::vector<unsigned char> buffer = o.buffer();

    int bytesWritten = stream.writeRawData((const char *)(buffer.data()), PrivateKey::length);

    if(bytesWritten != PrivateKey::length)
        throw new std::exception("Could not write PrivateKey::length bytes.");

    return stream;
}

QDataStream & operator>>(QDataStream& stream, PrivateKey & o) {

    // Allocate buffer
    std::vector<unsigned char> buffer(PrivateKey::length, 0);

    // Read from stream to buffer
    int bytesRead = stream.readRawData((char *)(buffer.data()), PrivateKey::length);

    if(bytesRead != PrivateKey::length)
        throw new std::exception("Could not read PrivateKey::length bytes.");

    // Update buffer
    o.setBuffer(buffer);

    return stream;
}

bool PrivateKey::operator<(const PrivateKey & o) const {

    // Get buffer
    std::vector<unsigned char> & oBuffer = o.buffer();

    // 0 is most significant byte
    for(unsigned int i = 0;i < length;i++) {

        unsigned char a = _buffer[i];
        unsigned char b = oBuffer[i];

        if(a > b)
            return false;
        else if(a < b)
            return true;
    }

    return false;
}

bool PrivateKey::operator==(const PrivateKey & o) const {
    return _buffer == o.buffer();
}

std::vector<unsigned char> PrivateKey::buffer() const {
    return _buffer;
}

void PrivateKey::setBuffer(const std::vector<unsigned char> & buffer) {
    _buffer = buffer;
}

uint qHash(const PrivateKey & o) {
    return qHash(o.toString());
}
