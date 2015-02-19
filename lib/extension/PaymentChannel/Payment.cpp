#include "Payment.hpp"

#include <QJsonObject>

Payment::Payment(const QJsonObject & rawTransaction) {
    // Recover fields
}

Payment::Payment(const OutPoint& contractOutput,
                 const P2PKHTxOut& payorOutput,
                 const P2PKHTxOut& payeeOutput)
    : _contractOutput(contractOutput)
    , _payorOutput(payorOutput)
    , _payeeOutput(payeeOutput) {
}

QJsonObject Payment::rawTransaction() const {

    // Turn into raw json transaction
    return QJsonObject();
}

OutPoint Payment::contractOutput() const {
    return _contractOutput;
}

void Payment::setContractOutput(const OutPoint &contractOutput) {
    _contractOutput = contractOutput;
}

P2PKHTxOut Payment::payorOutput() const {
    return _payorOutput;
}

void Payment::setPayorOutput(const P2PKHTxOut &payorOutput) {
    _payorOutput = payorOutput;
}

P2PKHTxOut Payment::payeeOutput() const {
    return _payeeOutput;
}

void Payment::setPayeeOutput(const P2PKHTxOut &payeeOutput) {
    _payeeOutput = payeeOutput;
}
