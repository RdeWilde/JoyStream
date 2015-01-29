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

QJsonObject Refund::bitswaprjsEncoding() const {
    // Turn into raw json transaction
}

bool Refund::isRefundValid(const Signature &payorSignature, const Signature &payeeSignature) const {
    // do check


    // Create refund transaction
    // Tx refund = refund(contractHash, quint32 lockTime)

    // Create payor refund signature
    // Signature sig1 = _payorKeyPair.sk().sign(refund, sighash... );

    // Check that both can spend multisig output
    // BitCoin::checkMultisignature(contractOutput, sig1, signature);
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
