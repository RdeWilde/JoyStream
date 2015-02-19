#include "Channel.hpp"

#include "extension/BitCoin/BitSwaprjs.hpp"

Channel::Channel() {
}

/**
Refund Channel::refund(const Hash &contractHash) const {
    return Refund(OutputPoint(contractHash, _index),
                  P2PKHTxOut(_funds - _refundFee, _payorContractKeyPair.pk(), _payeeContractPk),
                  _refundLockTime);
}

Payment Channel::payment(const Hash &contractHash) const {

    // The amount paid so far
    quint64 amountPaid = _price*_numberOfPaymentsMade;

    return Payment(OutputPoint(contractHash, _index),
                   P2PKHTxOut(_funds - amountPaid, _payorContractKeyPair.pk()),
                   P2PKHTxOut(amountPaid - _paymentFee, _payorContractKeyPair.pk()));
}
*/

void Channel::computePayorRefundSignature(const Hash &contractHash) const {

    // Check that channel has been assigned
    if(_state != State::assigned)
        throw std::exception("State incompatile request, must be in assigned state.");

    // Make call to compute signature
    OutPoint contractOutputPoint(contractHash, _index);
    P2PKHTxOut refundOutput(_funds, _payorFinalKeyPair.pk());

    // remove PKs later, no reason we need them

    _payorRefundSignature = BitSwaprjs.compute_payor_refund_signature(contractOutputPoint, _payorContractKeyPair.sk(), _payorContractKeyPair.pk(), _payeeContractPk, refundOutput,_refundLockTime);
}

Signature Channel::paymentSignature(const Hash &contractHash) const {

    // Create pamynent
    //Payment payment = payment(contractHash);

    /**
      QJsonObject raw = payment.rawTransaction();
      return _payorContractKeyPair.sk().sign(raw, sighash);
      */

    return Signature();
}


void Channel::paymentMade() {
    _numberOfPaymentsMade++;
}

QJsonObject Channel::json() const {
    return QJsonObject {
                {"funds", _funds},
                {"firstPk", _payorContractKeyPair.pk().toString()},
                {"secondPk", _payeeContractPk.toString()}
            };
}
/*
void Channel::setPayorConfiguration(const Channel::Configuration::Payor & payorConfiguration) {

}

void Channel::setPayeeConfiguration(const Channel::Configuration::Payee & payeeConfiguration) {

}

*/
