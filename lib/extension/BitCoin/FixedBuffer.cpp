#include "FixedBuffer.hpp"

#include <QDataStream>

FixedBuffer::FixedBuffer(const QString & string) {

    // Check that string has correct length
    if(string.length() != 2*L)
        throw std::exception("String argument is of incorrect length, should be 2*L.");
    else {

        // Decode from hex format
        QByteArray b = QByteArray::fromHex(string);

        Q_ASSERT(b.length() == L);

        // Copy into buffer
        memcpy(&_buffer, b.constData(), L);
    }
}

FixedBuffer::FixedBuffer() {

    // Clear buffer
    clear();
}

FixedBuffer::FixedBuffer(const FixedBuffer<L>& fixedBuffer) {

    for(unsigned int i = 0;i < L;i++)
        _buffer[i] = fixedBuffer.at(i);
}

FixedBuffer & FixedBuffer::operator=(const FixedBuffer<L>& rhs) {

    for(unsigned int i = 0;i < L;i++)
        _buffer[i] = rhs.at(i);
}

char FixedBuffer::at(unsigned int index) const {

    // Check that buffer is not exceeded
    if(index + 1 > L)
        throw std::exception("Index exceeds buffer.");
    else
        return _buffer[index];
}

void FixedBuffer::set(unsigned int index, char value) {

    // Check that buffer is not exceeded
    if(index + 1 > L)
        throw std::exception("Index exceeds buffer.");
    else
        _buffer[index] = value;
}

void FixedBuffer::clear() {

    for(unsigned int i = 0;i < L;i++)
        _buffer[i] = 0;
}

bool FixedBuffer::isClear() const {

    // Find non zero byte
    for(unsigned int i = 0;i < L;i++) {
        if(_buffer[i] != 0)
            return false;
    }

    // We didn't, so it is clear
    return true;
}

QString FixedBuffer::toString() const {

    // Put into byte array
    QByteArray raw(_buffer, L);

    // Hex encode
    QByteArray hexEncoded = raw.toHex();

    // Return as string
    return QString(hexEncoded.constData());
}

const char * FixedBuffer::begin() const {
    return _buffer[0];
}

const char * FixedBuffer::end() const {
    return _buffer[L];
}

char * FixedBuffer::begin() {
    return _buffer[0];
}

char * FixedBuffer::end() {
    return _buffer[L];
}

QDataStream & operator<<(QDataStream& stream, const FixedBuffer<L>& fixedBuffer) {

    // Write to stream
    for(unsigned int i = 0;i < L;i++)
        stream << fixedBuffer.at(i);

    return stream;
}

QDataStream & operator>>(QDataStream& stream, FixedBuffer<L>& fixedBuffer) {

    // Read from stream
    for(unsigned int i = 0;i < L;i++) {

        char value;
        stream >> value;

        fixedBuffer.set(i, value);
    }

    return stream;
}
