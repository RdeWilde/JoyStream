#ifndef P2PKH_TX_OUT_HPP
#define P2PKH_TX_OUT_HPP

#include "extension/BitCoin/PublicKey.hpp"

class P2PKHTxOut
{
public:

    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    P2PKHTxOut();
    P2PKHTxOut(const P2PKHTxOut& p2pkhTxOut);
    P2PKHTxOut & operator=(const P2PKHTxOut& p2pkhTxOut);

    // Constructor from members
    P2PKHTxOut(quint64 value, const PublicKey& pk);

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
