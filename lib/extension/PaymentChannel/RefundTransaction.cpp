#include "RefundTansaction.hpp"

#include <QJsonObject>

RefundTransaction::RefundTransaction(const QJsonObject & rawTransaction) {
    // Recover fields
}

RefundTransaction::RefundTransaction(const TxOut & contractOutput, quint64 funds)
    : _contractOutput(contractOutput)
    , _funds(funds) {
}

QJsonObject RefundTransaction::rawTransaction() const {
    // Turn into raw json transaction
}

TxOut RefundTransaction::contractOutput() const {
    return _contractOutput;
}

void RefundTransaction::setContractOutput(const TxOut &contractOutput) {
    _contractOutput = contractOutput;
}

quint64 RefundTransaction::funds() const {
    return _funds;
}

void RefundTransaction::setFunds(const quint64 &funds) {
    _funds = funds;
}
