#include "Refund.hpp"

#include <QJsonObject>

Refund::Refund(const QJsonObject & rawTransaction) {
    // Recover fields
}

Refund::Refund(const OutputPoint & contractOutput, const P2PKHTxOut& ouput, quint32 _lockTime)
    : _contractOutput(contractOutput)
    , _output(ouput)
    , _lockTime(lockTime) {
}

QJsonObject Refund::rawTransaction() const {
    // Turn into raw json transaction
}

OutputPoint Refund::contractOutput() const {
    return _contractOutput;
}

void Refund::setContractOutput(const OutputPoint &contractOutput) {
    _contractOutput = contractOutput;
}

P2PKHTxOut Refund::output() const {
    return _output;
}

void Refund::setOutput(const P2PKHTxOut &output) {
    _output = output;
}

quint32 Refund::lockTime() const {
    return _lockTime;
}

void Refund::setLockTime(const quint32 &lockTime) {
    _lockTime = lockTime;
}
