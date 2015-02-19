#include "Payee.hpp"

#include "extension/BitCoin/Signature.hpp"
#include "extension/BitCoin/P2PKHTxOut.hpp"

Payee::Payee()
    : _state(State::waiting_forpayee_information)
    , _numberOfPaymentsMade(0) {
}

void Payee::registerPayeeConfiguration(quint32 lockTime, quint32 price) {

    // Check state
    if(_state != State::waiting_forpayee_information)
        throw std::exception("State incompatile request, must be in State::waiting_forpayee_information state.");

    _lockTime = lockTime;
    _price = price;
    _state = State::waiting_for_contract_information;
}

void Payee::registerContractInformation(const OutPoint &contractOutputPoint, quint64 funds, const PublicKey &payorFinalPk) {

    // Check state
    if(_state != State::waiting_for_contract_information)
        throw std::exception("State incompatile request, must be in State::waiting_for_contract_information state.");

    _contractOutputPoint = contractOutputPoint;
    _funds = funds;
    _payorFinalPk = payorFinalPk;

    _state = State::able_to_receive_payments;
}

Signature Payee::generateRefundSignature() const {

    //return BitSwaprjs.compute_payor_refund_signature(_contractOutputPoint, _payeeContractOutput.sk(), <not needed>, <not needed _payeeContractPk>, P2PKHTxOut(_funds, _payorFinalPk), _lockTime);

    // random bs
    return Signature();
}

bool Payee::registerPayment(const Signature &paymentSignature) {

    bool check = checkNextPaymentSignature(paymentSignature);

    // Increase payment count if signature was valid
    if(check) {
        _numberOfPaymentsMade++;
        _lastValidPaymentSignature = paymentSignature;
    }

    return check;
}

bool Payee::checkNextPaymentSignature(const Signature &paymentSignature) const {

    // check with (_numberOfPaymentsMade + 1)

    // random bs
    return true;
}

float Payee::outputPointVisible() const {

    // random bs
    return 0.5;
}

bool Payee::isContractValid() const {

    // random bs
    return true;
}
