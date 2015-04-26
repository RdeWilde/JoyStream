#ifndef P2PKH_TX_OUT_HPP
#define P2PKH_TX_OUT_HPP

#include "extension/BitCoin/PublicKey.hpp"

class QJsonObject;

class P2PKHTxOut
{
public:

    // Default constructor
    P2PKHTxOut();

    // Constructor from members
    P2PKHTxOut(quint64 value, const PublicKey & pk);

    // Copy constructor
    P2PKHTxOut(const P2PKHTxOut& p2pkhTxOut);

    // Assignment operator
    P2PKHTxOut & operator=(const P2PKHTxOut& p2pkhTxOut);

    // Converts to json
    QJsonObject json() const;

    // Getters and setters
    quint64 value() const;
    void setValue(const quint64 &value);

    PublicKey pk() const;
    void setPk(const PublicKey &pk);

private:

    // Value of output
    quint64 _value;

    // Public key controlling output
    PublicKey _pk;
};

#endif // P2PKH_TX_OUT_HPP
