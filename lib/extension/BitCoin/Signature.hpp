#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

#include "PublicKey.hpp"

class QDataStream;

#define SIGNATURE_LENGTH 11

class Signature
{
public:

    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    Signature();
    Signature(const Signature& signature);
    Signature(const QString & signature);
    Signature & operator=(const Signature& signature);

    static const quint32 length = SIGNATURE_LENGTH;

    bool isValid(const PublicKey & key) const;
    QString toString() const;

    friend QDataStream & operator<<(QDataStream & stream, const Signature & key);
    friend QDataStream & operator>>(QDataStream & stream, Signature & key);

private:

    char _raw[SIGNATURE_LENGTH];
};

#endif // SIGNATURE_HPP
