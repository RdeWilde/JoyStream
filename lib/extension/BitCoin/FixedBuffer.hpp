/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef FIXED_BUFFER_HPP
#define FIXED_BUFFER_HPP

#include <QtGlobal> // quint32, ...
#include <QChar>

class QDataStream;

/**
 * Paremetrized buffer w.r.t integer
 */
template <unsigned int L>
class FixedBuffer
{
public:

    static const quint32 length = L;

    // Construct from hex encoded string, hence length of string must be 2L
    FixedBuffer(const QString & string);

    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    FixedBuffer();
    FixedBuffer(const FixedBuffer<L>& fixedbuffer);
    FixedBuffer<L> & operator=(const FixedBuffer<L>& rhs);

    /**
     * Utilities
     */
    QChar at(unsigned int index) const;
    void set(unsigned int index, QChar value);
    void clear();
    //bool isClear() const;
    QString toString() const;

    /**
     * Iterator routines.

    const char * begin() const;
    const char * end() const;

    char * begin();
    char * end();
*/

    /**
     * Operator overloading.
     */
    friend QDataStream & operator<<(QDataStream& stream, const FixedBuffer<L> & fixedBuffer);
    friend QDataStream & operator>>(QDataStream& stream, FixedBuffer<L> & fixedBuffer);

    // Comparison for use with QMap

    // dont work for whatever reason?
    friend bool operator<(const FixedBuffer<L> & lhs, const FixedBuffer<L> & rhs);
    friend bool operator==(const FixedBuffer<L> & lhs, const FixedBuffer<L> & rhs);

    bool less(const FixedBuffer<L> & o) const;
    bool equals(const FixedBuffer<L> & o) const;

private:

    // Actual type fixed buffer
    // 0 is most significant byte for comparisons
    //char _buffer[L];

    // LOL: end using qstring for now since I ran into encoding issues,
    // later we will substitite this class with proper types any way,
    // so not worth messing around with base58 encoding.
    QString _buffer;
};

/**
 * This has to be inline due to how templatingw works.
 */

#include <QDataStream>

template <unsigned int L>
FixedBuffer<L>::FixedBuffer(const QString & string) {

    // Check that string has correct length
    if(string.length() != L)
        throw std::exception("String argument is of incorrect length, should be L.");
    else {

        /**
        // Decode from hex format
        QByteArray b = QByteArray::fromHex(string.toLatin1());

        //Q_ASSERT(b.length() == L);

        // Copy into buffer
        memcpy(&_buffer, b.constData(), L);
        */

        _buffer = string;
    }
}

template <unsigned int L>
FixedBuffer<L>::FixedBuffer() {

    // Clear buffer
    clear();
}

template <unsigned int L>
FixedBuffer<L>::FixedBuffer(const FixedBuffer<L>& fixedBuffer) {

    for(unsigned int i = 0;i < L;i++)
        _buffer[i] = fixedBuffer.at(i);
}

template <unsigned int L>
FixedBuffer<L> & FixedBuffer<L>::operator=(const FixedBuffer<L>& rhs) {

    for(unsigned int i = 0;i < L;i++)
        _buffer[i] = rhs.at(i);

    return *this;
}

template <unsigned int L>
QChar FixedBuffer<L>::at(unsigned int index) const {

    // Check that buffer is not exceeded
    if(index + 1 > L)
        throw std::exception("Index exceeds buffer.");
    else
        return _buffer[index];
}

template <unsigned int L>
void FixedBuffer<L>::set(unsigned int index, QChar value) {

    // Check that buffer is not exceeded
    if(index + 1 > L)
        throw std::exception("Index exceeds buffer.");
    else
        _buffer[index] = value;
}

template <unsigned int L>
void FixedBuffer<L>::clear() {

    //for(unsigned int i = 0;i < L;i++)
    //    _buffer[i] = 0;

    _buffer = QString(L,' ');
}
/*
template <unsigned int L>
bool FixedBuffer<L>::isClear() const {

    // Find non zero byte
    for(unsigned int i = 0;i < L;i++) {
        if(_buffer[i] != 0)
            return false;
    }

    // We didn't, so it is clear
    return true;
}
*/

template <unsigned int L>
QString FixedBuffer<L>::toString() const {

    /**
    // Put into byte array
    QByteArray raw(_buffer, L);

    // Hex encode
    QByteArray hexEncoded = raw.toHex();

    // Return as string
    return QString(hexEncoded.constData());
    */

    return _buffer;
}
/*
template <unsigned int L>
const char * FixedBuffer<L>::begin() const {
    return _buffer[0];
}

template <unsigned int L>
const char * FixedBuffer<L>::end() const {
    return _buffer[L];
}

template <unsigned int L>
char * FixedBuffer<L>::begin() {
    return _buffer[0];
}

template <unsigned int L>
char * FixedBuffer<L>::end() {
    return _buffer[L];
}
*/

template <unsigned int L>
QDataStream & operator<<(QDataStream& stream, const FixedBuffer<L> & fixedBuffer) {

    // Write to stream
    for(unsigned int i = 0;i < L;i++)
        stream << fixedBuffer.at(i);

    return stream;
}

template <unsigned int L>
QDataStream & operator>>(QDataStream& stream, FixedBuffer<L> & fixedBuffer) {

    // Read from stream
    for(unsigned int i = 0;i < L;i++) {

        QChar value;
        stream >> value;

        fixedBuffer.set(i, value);
    }

    return stream;
}

template <unsigned int L>
bool operator<(const FixedBuffer<L> & lhs, const FixedBuffer<L> & rhs) {
    return lhs.less(rhs);
}

template <unsigned int L>
bool operator==(const FixedBuffer<L> & lhs, const FixedBuffer<L> & rhs) {
    return lhs.equals(rhs);
}

template <unsigned int L>
bool FixedBuffer<L>::less(const FixedBuffer<L> & o) const {

    // 0 is most significant byte
    for(unsigned int i = 0;i < L;i++) {

        QChar a = this->at(i);
        QChar b = o.at(i);

        if(a > b)
            return false;
        else if(a < b)
            return true;
    }

    return false;
}

template <unsigned int L>
bool FixedBuffer<L>::equals(const FixedBuffer<L> & o) const {

    // 0 is most significant byte
    for(unsigned int i = 0;i < L;i++) {

        if(this->at(i) != o.at(i))
            return false;
    }

    return true;
}

#include <QHash>

template <unsigned int L>
uint qHash(const FixedBuffer<L> & o) {
    return qHash(o.toString());
}

#endif // FIXED_BUFFER_HPP
