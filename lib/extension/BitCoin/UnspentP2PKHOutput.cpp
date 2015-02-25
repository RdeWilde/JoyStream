#include "UnspentP2PKHOutput.hpp"

UnspentP2PKHOutput::UnspentP2PKHOutput() {
}

UnspentP2PKHOutput::UnspentP2PKHOutput(const KeyPair & fundingOutputKeyPair, const OutPoint & fundingOutput, quint64 fundingValue)
    : _fundingOutputKeyPair(fundingOutputKeyPair)
    , _fundingOutput(fundingOutput)
    , _fundingValue(fundingValue) {
}

KeyPair UnspentP2PKHOutput::fundingOutputKeyPair() const {
    return _fundingOutputKeyPair;
}

void UnspentP2PKHOutput::setFundingOutputKeyPair(const KeyPair &fundingOutputKeyPair) {
    _fundingOutputKeyPair = fundingOutputKeyPair;
}

OutPoint UnspentP2PKHOutput::fundingOutput() const {
    return _fundingOutput;
}

void UnspentP2PKHOutput::setFundingOutput(const OutPoint &fundingOutput) {
    _fundingOutput = fundingOutput;
}
quint64 UnspentP2PKHOutput::fundingValue() const {
    return _fundingValue;
}

void UnspentP2PKHOutput::setFundingValue(quint64 fundingValue) {
    _fundingValue = fundingValue;
}
