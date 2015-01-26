#include "Contract.hpp"

#include <QJsonObject>

Contract::Contract() {
}

Contract::Contract(const Contract& contract)
    : _fundingOutput(contract.fundingOutput())
    , _p2shTxOuts(contract.p2shTxOuts())
    , _change(cntract.refund()) {
}

Contract & Contract::operator=(const Contract& contract) {
    _fundingOutput = contract.fundingOutput();
    _p2shTxOuts = contract.p2shTxOuts();
    _change = contract.refund();
}

Contract::Contract(quint32 numberOfMultisigOutputs)
    : _p2shTxOuts(numberOfMultisigOutputs) {
}

Contract::Contract(const QJsonObject & rawTransaction) {
    // extract fields
}

QJsonObject Contract::rawTransaction() const {
    // return raw transacton form in json form
}

OutputPoint Contract::fundingOutput() const {
    return _fundingOutput;
}

void Contract::setFundingOutput(const OutputPoint &fundingOutput) {
    _fundingOutput = fundingOutput;
}

QVector<P2SHTxOut> Contract::p2shTxOuts() const {
    return _p2shTxOuts;
}

void Contract::setP2shTxOuts(const QVector<P2SHTxOut> &p2shTxOuts) {
    _p2shTxOuts = p2shTxOuts;
}

P2PKHTxOut Contract::change() const {
    return _change;
}

void Contract::setChange(const P2PKHTxOut &change) {
    _change = change;
}

void Contract::setOutput(quint32 index, const P2SHTxOut& output) {

    //if(index < _p2shTxOuts.size())
        _p2shTxOuts[index] = output;
    //else
    //    throw std::exception("invalid");
}
