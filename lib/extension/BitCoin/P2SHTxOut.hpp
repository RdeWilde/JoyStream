#ifndef P2SH_TX_OUT_HPP
#define P2SH_TX_OUT_HPP

#include "extension/BitCoin/PublicKey.hpp"

class QJsonObject;

class P2SHTxOut
{
public:

    P2SHTxOut();

    // Constructor based on members
    P2SHTxOut(quint64 value, const PublicKey & firstPk, const PublicKey & secondPk);

    // Copy constructor
    P2SHTxOut(const P2SHTxOut& p2shTxOut);

    // Assignement operator
    P2SHTxOut & operator=(const P2SHTxOut& p2shTxOut);

    // Turns into json
    QJsonObject json() const;

    // Getters and setters
    quint64 value() const;
    void setValue(const quint64 &value);

    PublicKey firstPk() const;
    void setFirstPk(const PublicKey &firstPk);

    PublicKey secondPk() const;
    void setSecondPk(const PublicKey &secondPk);

private:

    // Funds allocated to output
    quint64 _value;

    // First public key controlling multisig output
    PublicKey _firstPk;

    // Second public key controlling multisig output
    PublicKey _secondPk;
};

#endif // P2SH_TX_OUT_HPP
