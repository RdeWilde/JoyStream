#ifndef P2SH_TX_OUT_HPP
#define P2SH_TX_OUT_HPP

#include "extension/BitCoin/PublicKey.hpp"

class P2SHTxOut
{
public:
    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    P2SHTxOut();
    P2SHTxOut(const P2SHTxOut& p2shTxOut);
    P2SHTxOut & operator=(const P2SHTxOut& p2shTxOut);

    // Constructor based on members
    P2SHTxOut(quint64 value, const PublicKey & firstPk, const PublicKey & secondPk);

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
