#include "TxId.hpp"

#include <QDataStream>
#include <QHash>

TxId::TxId() {
    // Clear buffer
    clear();
}

TxId::TxId(const TxId & o) {
    memcpy(_buffer, o.buffer(), length);
}

TxId::TxId(const QString & string) {

    // Check that string has correct length
    if(string.length() != 2*length)
        throw std::exception("String argument is of incorrect length, should be 2*length.");
    else {

        // Decode from hex format
        QByteArray b = QByteArray::fromHex(string.toLatin1());

        Q_ASSERT(b.length() == length);

        // Copy into buffer
        memcpy(_buffer, b.constData(), length);
    }
}

TxId & TxId::operator=(const TxId & o) {

    // Copy into buffer
    memcpy(_buffer, o.buffer(), length);

    return *this;
}

char TxId::at(unsigned int index) const {

    // Check that buffer is not exceeded
    if(index + 1 > length)
        throw std::exception("Index exceeds buffer.");
    else
        return _buffer[index];
}

void TxId::set(unsigned int index, char value) {

    // Check that buffer is not exceeded
    if(index + 1 > length)
        throw std::exception("Index exceeds buffer.");
    else
        _buffer[index] = value;
}

void TxId::clear() {

    for(unsigned int i = 0;i < length;i++)
        _buffer[i] = 0;
}

bool TxId::isClear() const {

    // Find non zero byte
    for(unsigned int i = 0;i < length;i++) {
        if(_buffer[i] != 0)
            return false;
    }

    // We didn't, so it is clear
    return true;
}

const char * TxId::buffer() const {
    return _buffer;
}

char * TxId::buffer() {
    return _buffer;
}

QString TxId::toString() const {

    // Put into byte array
    QByteArray raw(_buffer, length);

    // Hex encode
    QByteArray hexEncoded = raw.toHex();

    // Return as string
    return QString(hexEncoded.constData());
}

QDataStream & operator<<(QDataStream& stream, const TxId & o) {

    // Write to stream
    int bytesWritten = stream.writeRawData(o.buffer(), TxId::length);

    if(bytesWritten != TxId::length)
        throw new std::exception("Could not write TxId::length bytes.");

    return stream;
}

QDataStream & operator>>(QDataStream& stream, TxId & o) {

    // Read from stream
    int bytesRead = stream.readRawData(o.buffer(), TxId::length);

    if(bytesRead != TxId::length)
        throw new std::exception("Could not read TxId::length bytes.");

    return stream;
}

bool TxId::operator<(const TxId & o) const {

    // 0 is most significant byte
    for(unsigned int i = 0;i < length;i++) {

        char a = _buffer[i];
        char b = o.at(i);

        if(a > b)
            return false;
        else if(a < b)
            return true;
    }

    return false;
}

bool TxId::operator==(const TxId & o) const {

    for(unsigned int i = 0;i < length;i++) {

        if(_buffer[i] != o.at(i))
            return false;
    }

    return true;
}

uint qHash(const TxId & o) {
    return qHash(o.toString());
}
