#include "FixedBuffer.hpp"

#include <QDataStream>

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
