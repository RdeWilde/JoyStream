#include "TxOut.hpp"

TxOut::TxOut() {

}

TxOut::TxOut(const TxOut& txOut) {
    _txHash = txOut.txHash();
    _txOutputIndex = txOut.setTxOutputIndex();
}

TxOut & operator=(const TxOut& txOut) {
    _txHash = txOut.txHash();
    _txOutputIndex = txOut.setTxOutputIndex();
}

TxOut::TxOut(const Hash & txHash, quint32 txOutputIndex)
    : _txHash(txHash)
    , _txOutputIndex(txOutputIndex) {
}

Hash TxOut::txHash() const {
    return _txHash;
}

void TxOut::setTxHash(const Hash &txHash){
    _txHash = txHash;
}

quint32 TxOut::txOutputIndex() const {
    return _txOutputIndex;
}

void TxOut::setTxOutputIndex(const quint32 &txOutputIndex) {
    _txOutputIndex = txOutputIndex;
}


