/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "PublicKey.hpp"
#include "base58.hpp"

#include <QDataStream>
#include <QHash>

PublicKey::PublicKey()
    : _buffer(length, 0) {
}

PublicKey::PublicKey(const PublicKey & o)
    : _buffer(o.buffer()) {
}

PublicKey::PublicKey(const QString & string)
    : _buffer(length, 0) {

    // Check that string has correct length
    if(string.length() != 2*length)
        throw std::runtime_error("String argument is of incorrect length, should be 2*length.");
    else {

        // Decode from hex format
        QByteArray b = QByteArray::fromHex(string.toLatin1());

        Q_ASSERT(b.length() == length);

        // Copy into buffer
        memcpy(_buffer.data(), b.constData(), length);
    }

    /** Old code which decoded base58

    // Turn into std::string
    std::string stdString = string.toStdString();

    // Base58 base decode into buffer
    bool result = DecodeBase58(stdString, _buffer);

    // Check that decoding worked
    if(!result)
        throw std::runtime_error("Decoding base58 did not work.");
        */
}

PublicKey & PublicKey::operator=(const PublicKey & o) {
    _buffer = o.buffer();
    return *this;
}

void PublicKey::clear() {

    for(unsigned int i = 0;i < length;i++)
        _buffer[i] = 0;
}

bool PublicKey::isClear() const {

    // Find non zero byte
    for(int i = 0;i < length;i++) {
        if(_buffer[i] != 0)
            return false;
    }

    // We didn't, so it is clear
    return true;
}

QString PublicKey::toString() const {

    /**
    // Base58 encode
    std::string result = EncodeBase58(_buffer);

    return QString::fromStdString(result);
    */

    QByteArray b = QByteArray::fromRawData(_buffer.data(), length);
    std::string s = b.toHex().toStdString();

    return QString::fromStdString(s);
}

QDataStream & operator<<(QDataStream& stream, const PublicKey & o) {

    // Write to stream from buffer
    std::vector<char> buffer = o.buffer();

    int bytesWritten = stream.writeRawData((const char *)(buffer.data()), PublicKey::length);

    if(bytesWritten != PublicKey::length)
        throw new std::runtime_error("Could not write PublicKey::length bytes.");

    return stream;
}

QDataStream & operator>>(QDataStream& stream, PublicKey & o) {

    // Allocate buffer
    std::vector<char> buffer(PublicKey::length, 0);

    // Read from stream to buffer
    int bytesRead = stream.readRawData((char *)(buffer.data()), PublicKey::length);

    if(bytesRead != PublicKey::length)
        throw new std::runtime_error("Could not read PublicKey::length bytes.");

    // Update buffer
    o.setBuffer(buffer);

    return stream;
}

bool PublicKey::operator<(const PublicKey & o) const {

    // Get buffer
    std::vector<char> oBuffer = o.buffer();

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

bool PublicKey::operator==(const PublicKey & o) const {
    return _buffer == o.buffer();
}

std::vector<char> PublicKey::buffer() const {
    return _buffer;
}

void PublicKey::setBuffer(const std::vector<char> & buffer) {
    _buffer = buffer;
}

uint qHash(const PublicKey & o) {
    return qHash(o.toString());
}
