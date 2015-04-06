#include "PublicKey.hpp"
#include "base58.hpp"

#include <QDataStream>
#include <QHash>

PublicKey::PublicKey(const QString & string)
    : _buffer(length, 0) {

    // Turn into std::string
    std::string stdString = string.toStdString();

    // Base58 base decode into buffer
    bool result = DecodeBase58(stdString, _buffer);

    // Check that decoding worked
    if(!result)
        throw std::exception("Decoding base58 did not work.");
}

PublicKey::PublicKey()
    : _buffer(length, 0) {
}

PublicKey::PublicKey(const PublicKey & o) {
    *this = o;
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

    // Base58 encode
    std::string result = EncodeBase58(_buffer);

    return QString::fromStdString(result);
}

QDataStream & operator<<(QDataStream& stream, const PublicKey & o) {

    // Write to stream from buffer
    std::vector<unsigned char> buffer = o.buffer();

    int bytesWritten = stream.writeRawData((const char *)(buffer.data()), PublicKey::length);

    if(bytesWritten != PublicKey::length)
        throw new std::exception("Could not write PublicKey::length bytes.");

    return stream;
}

QDataStream & operator>>(QDataStream& stream, PublicKey & o) {

    // Allocate buffer
    std::vector<unsigned char> buffer(PublicKey::length, 0);

    // Read from stream to buffer
    int bytesRead = stream.readRawData((char *)(buffer.data()), PublicKey::length);

    if(bytesRead != PublicKey::length)
        throw new std::exception("Could not read PublicKey::length bytes.");

    // Update buffer
    o.setBuffer(buffer);

    return stream;
}

bool PublicKey::operator<(const PublicKey & o) const {

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

bool PublicKey::operator==(const PublicKey & o) const {
    return _buffer == o.buffer();
}

std::vector<unsigned char> PublicKey::buffer() const {
    return _buffer;
}

void PublicKey::setBuffer(const std::vector<unsigned char> & buffer) {
    _buffer = buffer;
}

uint qHash(const PublicKey & o) {
    return qHash(o.toString());
}