#ifndef TX_OUT_HPP
#define TX_OUT_HPP

#include "extension/BitCoin/Hash.hpp"

class TxOut
{
public:

    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    TxOut();
    TxOut(const TxOut& txOut);
    TxOut & operator=(const TxOut& txOut);

    // Constructor
    TxOut(const Hash & txHash, quint32 txOutputIndex);

    // Getters and setters
    Hash txHash() const;
    void setTxHash(const Hash &txHash);

    quint32 txOutputIndex() const;
    void setTxOutputIndex(const quint32 &txOutputIndex);

private:

    // TxId of transaction
    Hash _txHash;

    // Output index
    quint32 _txOutputIndex;
};

#endif // TX_OUT_HPP
