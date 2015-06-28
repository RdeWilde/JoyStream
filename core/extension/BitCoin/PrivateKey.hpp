/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PRIVATE_KEY_HPP
#define PRIVATE_KEY_HPP

#include <QtGlobal>

#include <vector>

class QDataStream;
class QString;

class PrivateKey
{
public:

    // Byte length of raw data
    static const quint32 length = 32;

    // Default constructor
    PrivateKey();

    // Copy constructor
    PrivateKey(const PrivateKey & o);

    // Construct from Base58(check) encoded string
    // e.g. 4b13890d16944ab1d8dd1ef4ac604fe589c62c27af74fc7250c4d01e2a1915d9
    PrivateKey(const QString & string);

    // Assignment operator
    PrivateKey & operator=(const PrivateKey & o);

    // Utilities
    void clear();
    bool isClear() const;

    // Encodes as base58
    QString toString() const;

    // Stream operator
    friend QDataStream & operator<<(QDataStream & stream, const PrivateKey & o);
    friend QDataStream & operator>>(QDataStream & stream, PrivateKey & o);

    // Comparison for use with QMap
    bool operator<(const PrivateKey & o) const;
    bool operator==(const PrivateKey & o) const;

    std::vector<unsigned char> buffer() const;
    void setBuffer(const std::vector<unsigned char> & buffer);

private:

    // Raw data
    // 0 is most significant byte for comparisons
    std::vector<unsigned char> _buffer;
};

uint qHash(const PrivateKey & o);

#endif // PRIVATE_KEY_HPP
