#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

#include "PublicKey.hpp"

class QDataStream;

class Signature
{
public:
    Signature();

    //Signature(tx, sk);

    static quint32 length();

    bool isValid(const PublicKey & key) const;

    friend QDataStream & operator<<(QDataStream & stream, const Signature & key);
    friend QDataStream & operator>>(QDataStream & stream, Signature & key);
};

#endif // SIGNATURE_HPP
