/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#include <stdutils/uchar_vector.h>

#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

class QString;
class QDataStream;
class QByteArray;

namespace Coin {

// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR
// WARNING: NEEDS SECURE ALLOCATOR & DESTRUCTOR

class Signature
{
public:

    // The maximum byte length of a valid signature
    const static unsigned int maxLength = 73;

    // Default constructor
    Signature();

    // Constructor from members
    Signature(const uchar_vector & raw);

    // Hex encoded signature
    Signature(const QString & signature);

    // Copy constructor
    Signature(const Signature & signature);

    // Safe destructor
    ~Signature();

    // Assignment operator
    Signature & operator=(const Signature & signature);

    // Length of signature
    unsigned int length() const;

    // Hex encoded signature
    QString toString() const;

    // Encode as byte array
    QByteArray toByteArray() const;

    // Stream processing
    int readFromStream(QDataStream & stream, int length);
    int writeToStream(QDataStream & stream) const;

    // Getters and setters
    uchar_vector raw() const;
    void setRaw(const uchar_vector & raw);

private:

    // (Strict?) DER encoded signature
    // 0 is most significant byte for comparisons
    uchar_vector _raw;
};

}

#endif // SIGNATURE_HPP

