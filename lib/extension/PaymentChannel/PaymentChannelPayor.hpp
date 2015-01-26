#ifndef PAYMENT_CHANNEL_PAYOR_HPP
#define PAYMENT_CHANNEL_PAYOR_HPP

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"
#include "extension/BitCoin/KeyPair.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/Signature.hpp"
#include "extension/BitCoin/OutputPoint.hpp"

class Refund;
class Payment;
class Contract;

/**
 * 1-to-N payment channel from payor perspective, using design in CBEP.
 * https://github.com/bedeho/CBEP
 *
 */
class PaymentChannelPayor
{
public:

    /**
     * @brief State of a payor.
     */
    enum class State {

        // Getting keys and refund signature
        constructing_channel,

        // Full set of signatures acheived
        all_refunds_collected,

        //
        contract_broadcasted,


        all_refunds_uspendable
    };

    /**
     * Represents payment channel participant as seen by a payor.
     */
    class Slot {

    public:

        /**
         * @brief Enumeration of possible slot states.
         */
        enum class State {
            unassigned,
            assigned,
            refund_signed
        };

        // Default/Copy constructor and assignemtn operator needed to put in container.
        Slot();
        Slot(const Slot& slot);
        Slot & operator=(const Slot& rhs);

        // Set all fields, e.g. loading from file
        Slot(quint32 index,
             const State &state,
             quint64 priceIncrement,
             quint64 numberOfPaymentsMade,
             quint64 funds,
             const KeyPair &payorContractKeyPair,
             const PublicKey &payorFinalPk,
             const PublicKey &payeeContractPk,
             const PublicKey &payeeFinalPk,
             const Signature &refund,
             quint64 refundFee,
             quint64 paymentFee);

        /**
         * Payment channel operations
         */

        // Refund transaction for slot
        Refund refund(const Hash &contractHash, quint32 lockTime) const;

        // Payment transaction for slot, based on current _numberOfPaymentsMade value
        Payment payment(const Hash &contractHash) const;

        // Refund signature
        Signature refundSignature(const Hash &contractHash, quint32 lockTime) const;

        // Payment signature
        Signature paymentSignature(const Hash &contractHash) const;

        // Registers that a payment was made
        void paymentMade();

        /**
         * Getters and setters
         */

        quint32 index() const;
        void setIndex(const quint32 &index);

        State state() const;
        void setState(const State &state);

        quint64 priceIncrement() const;
        void setPriceIncrement(const quint64 &priceIncrement);

        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(const quint64 &numberOfPaymentsMade);

        quint64 funds() const;
        void setFunds(const quint64 &funds);

        KeyPair payorContractKeyPair() const;
        void setPayorContractKeyPair(const KeyPair &keyPair);

        PublicKey payorFinalPk() const;
        void setPayorFinalPk(const PublicKey &pk);

        PublicKey payeeContractPk() const;
        void setPayeeContractPk(const PublicKey &pk);

        PublicKey payeeFinalPk() const;
        void setPayeeFinalPk(const PublicKey &pk);

        Signature refund() const;
        void setRefund(const Signature &signature);

        quint64 refundFee() const;
        void setRefundFee(const quint64 &refundFee);

        quint64 paymentFee() const;
        void setPaymentFee(const quint64 &paymentFee);

    private:

        // Index in contract
        quint32 _index;

        // Slot state
        State _state;

        // Size of single payment
        quint64 _priceIncrement;

        // Number of payments made
        quint64 _numberOfPaymentsMade;

        // Funds allocated to output
        quint64 _funds;

        // Controls payour output of multisig
        KeyPair _payorContractKeyPair;

        // Controls final payment to payor
        PublicKey _payorFinalPk;

        // Controls payee output of multisig, received in joinin_contract.pk
        PublicKey _payeeContractPk;

        // Controls payee payments, received in sign_refund.pk
        PublicKey _payeeFinalPk;

        // Controls output refund for payee
        Signature _refund;

        // Fee used in refund transaction, is unlikely to vary across slots,
        quint64 _refundFee;

        // Fee used in payment transaction
        quint64 _paymentFee;

    };

    // Default constructor
    PaymentChannelPayor();

    // Constructor based on members
    //PaymentChannelPayor(quint32 numberOfPayees, const OutputPoint& fundingOutput, const KeyPair& fundingOutputKeyPair);

    // Get contract for
    Contract getContract() const;

    // Creates refund transaction for given output with given lock
    Refund refundTransaction(quint32 index) const;

    // Checks if output is spent
    bool spent(quint32 index) const;

private:

    // Payor state
    State _state;

    // Contract outputs
    QVector<Slot> _slots;

    // Lock time of refund
    quint32 _refundLockTime;

    // Unspent output funding channel
    OutputPoint _fundingOutput;

    // Controls output funding channel
    KeyPair _fundingOutputKeyPair;

    // Hash of contract
    Hash _contractHash;
};

#endif // PAYMENT_CHANNEL_PAYOR_HPP
