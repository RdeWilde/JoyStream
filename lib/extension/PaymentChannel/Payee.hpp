#ifndef PAYEE_HPP
#define PAYEE_HPP

#include "extension/BitCoin/KeyPair.hpp"
#include "extension/BitCoin/OutPoint.hpp"
#include "extension/BitCoin/Signature.hpp"

class Payee {

public:

    /**
     * @brief The payee state
     */
    enum class State {

        //
        waiting_forpayee_information,

        //
        waiting_for_contract_information,

        //
        able_to_receive_payments,

        //
        final_payment_spent,

        //
        refund_spent,
    };

    Payee();

    // When payee configurations are chosen
    void registerPayeeConfiguration(quint32 lockTime, quint32 price);

    // When contract information is known, as advertised in
    void registerContractInformation(const OutPoint &contractOutputPoint, quint64 funds, const PublicKey &payorFinalPk);

    Signature generateRefundSignature() const;

    // Attempts to register payment if signature is valid
    // ==================================================
    // A valid signature will lead to an increase of _numberOfPaymentsMade,
    // and storing signature in _lastValidPaymentSignature
    bool registerPayment(const Signature &paymentSignature);

    // Attempts to check validity of given payment signature with (_numberOfPaymentsMade + 1)
    bool checkNextPaymentSignature(const Signature &paymentSignature) const;

    /**
     * Routines below check contract validity in various ways
     */

    // Returns the rate at which peers have output point
    // of contract in mempool or chain.
    float outputPointVisible() const;

    // Checks contract validitity
    // ==========================
    // 1) There is tx with _contractOutputPoint in longest chain
    // 2) tx has contract output point
    // 3) scriptSig of tx is controlled by given keys and has the correct quanitity of funds
    // 4) channel has to correct number of participants.
    bool isContractValid() const;

private:

    // Payee state
    State _state;

    //
    quint64 _numberOfPaymentsMade;


    Signature _lastValidPaymentSignature;

    //
    quint32 _lockTime;

    //
    quint32 _price;

    //
    quint32 _maximumNumberOfSellers;

    //
    OutPoint _contractOutputPoint;

    // Controls payee portion of contract output
    KeyPair _payeeContractOutput;

    // Controls payee output in payment_lastValidPaymentSignature
    KeyPair _payeeFinalPaymentOutput;

    // Controls payor
    // PublicKey _payorContractOutput;
    PublicKey _payorFinalPk;

    //
    quint64 _funds;
};

#endif // PAYEE_HPP