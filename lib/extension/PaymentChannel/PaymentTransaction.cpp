#include "PaymentTransaction.hpp"

class QJsonObject;

PaymentTransaction::PaymentTransaction(const QJsonObject & rawTransaction) {
    // Recover fields
}

PaymentTransaction::PaymentTransaction(const TxOut & contractOutput,
                                       quint64 payorFunds,
                                       quint64 payeeFunds,
                                       const PublicKey & payourKey,
                                       const PublicKey & payeeKey)
    : _contractOutput(contractOutput)
    , _payorFunds(payorFunds)
    , _payeeFunds(payeeFunds)
    , _payeeKey(payeeKey) {

}

QJsonObject PaymentTransaction::rawTransaction() const {
    // Turn into raw json transaction
}

TxOut PaymentTransaction::contractOutput() const {
    return _contractOutput;
}

void PaymentTransaction::setContractOutput(const TxOut &contractOutput) {
    _contractOutput = contractOutput;
}

quint64 PaymentTransaction::payorFunds() const {
    return _payorFunds;
}

void PaymentTransaction::setPayorFunds(const quint64 &payorFunds) {
    _payorFunds = payorFunds;
}

quint64 PaymentTransaction::payeeFunds() const {
    return _payeeFunds;
}

void PaymentTransaction::setPayeeFunds(const quint64 &payeeFunds) {
    _payeeFunds = payeeFunds;
}

PublicKey PaymentTransaction::payourKey() const {
    return _payourKey;
}

void PaymentTransaction::setPayourKey(const PublicKey &payourKey) {
    _payourKey = payourKey;
}

PublicKey PaymentTransaction::payeeKey() const {
    return _payeeKey;
}

void PaymentTransaction::setPayeeKey(const PublicKey &payeeKey) {
    _payeeKey = payeeKey;
}
