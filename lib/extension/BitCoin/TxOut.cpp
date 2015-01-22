#include "TxOut.hpp"

TxOut::TxOut() {
}

TxOut::TxOut(const Hash & txHash, quint32 txOutputIndex)
    : _txHash(txHash)
    , _txOutputIndex(txOutputIndex) {
}
