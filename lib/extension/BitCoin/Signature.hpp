#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

#include "PublicKey.hpp"

class QDataStream;

#define SIGNATURE_LENGTH 11

class Signature
{
public:
    Signature();

    //Signature(tx, sk);

    static const quint32 length = SIGNATURE_LENGTH;

    bool isValid(const PublicKey & key) const;

    friend QDataStream & operator<<(QDataStream & stream, const Signature & key);
    friend QDataStream & operator>>(QDataStream & stream, Signature & key);

private:

    char _raw[SIGNATURE_LENGTH];
};

#endif // SIGNATURE_HPP
