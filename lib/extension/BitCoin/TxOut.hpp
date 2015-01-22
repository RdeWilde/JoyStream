#ifndef TX_OUT_HPP
#define TX_OUT_HPP

#include "extension/BitCoin/Hash.hpp"

class TxOut
{
public:

    // Default constructor
    TxOut();

    //
    TxOut(const Hash & txHash, quint32 txOutputIndex);

private:

    // TxId of transaction
    Hash _txHash;

    // Output index
    quint32 _txOutputIndex;
};

#endif // TX_OUT_HPP
