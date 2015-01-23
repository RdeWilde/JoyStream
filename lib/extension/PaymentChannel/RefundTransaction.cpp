#include "RefundTansaction.hpp"

#include <QJsonObject>

RefundTransaction::RefundTransaction(const QJsonObject & rawTransaction) {
    // Recover fields
}

RefundTransaction::RefundTransaction(const OutputPoint & contractOutput, const P2SHTxOut& refund, quint32 _lockTime)
    : _contractOutput(contractOutput)
    , _refund(refund)
    , _lockTime(lockTime) {
}

QJsonObject RefundTransaction::rawTransaction() const {
    // Turn into raw json transaction
}

OutputPoint RefundTransaction::contractOutput() const {
    return _contractOutput;
}

void RefundTransaction::setContractOutput(const OutputPoint &contractOutput) {
    _contractOutput = contractOutput;
}

P2SHTxOut RefundTransaction::refund() const {
    return _refund;
}

void RefundTransaction::setRefund(const P2SHTxOut &refund) {
    _refund = refund;
}
quint32 RefundTransaction::lockTime() const {
    return _lockTime;
}

void RefundTransaction::setLockTime(const quint32 &lockTime) {
    _lockTime = lockTime;
}

