#include "ContractTransaction.hpp"

#include <QJsonObject>

ContractTransaction::ContractTransaction() {
}

ContractTransaction::ContractTransaction(const ContractTransaction& contractTransaction) {
    _fundingOutput = contractTransaction.fundingOutput();
    _p2shTxOuts = contractTransaction.p2shTxOuts();
    _refund = contractTransaction.refund();
}

ContractTransaction & ContractTransaction::operator=(const ContractTransaction& contractTransaction) {
    _fundingOutput = contractTransaction.fundingOutput();
    _p2shTxOuts = contractTransaction.p2shTxOuts();
    _refund = contractTransaction.refund();
}

ContractTransaction::ContractTransaction(const QJsonObject & rawTransaction) {
    // extract fields
}

QJsonObject ContractTransaction::rawTransaction() const {
    // return raw transacton form in json form
}

OutputPoint ContractTransaction::fundingOutput() const {
    return _fundingOutput;
}

void ContractTransaction::setFundingOutput(const OutputPoint &fundingOutput) {
    _fundingOutput = fundingOutput;
}

QVector<P2SHTxOut> ContractTransaction::p2shTxOuts() const {
    return _p2shTxOuts;
}

void ContractTransaction::setP2shTxOuts(const QVector<P2SHTxOut> &p2shTxOuts) {
    _p2shTxOuts = p2shTxOuts;
}

P2PKHTxOut ContractTransaction::refund() const {
    return _refund;
}

void ContractTransaction::setRefund(const P2PKHTxOut &refund) {
    _refund = refund;
}
