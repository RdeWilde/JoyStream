#ifndef PAYOR_HPP
#define PAYOR_HPP

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"
#include "extension/BitCoin/KeyPair.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/Signature.hpp"
#include "extension/BitCoin/OutputPoint.hpp"

//#include "Contract.hpp"

class Refund;
class Payment;
//class Contract;

/**
 * 1-to-N payment channel from payor perspective, using design in CBEP.
 * https://github.com/bedeho/CBEP
 *
 */
class Payor
{
public:

    /**
     * @brief State of a payor.
     */
    enum class State {

        // Waiting to get PayeeConfiguration for all slots
        waiting_for_full_set_of_sellers,

        // Waiting to get refund signatures from all sellers
        waiting_for_full_set_of_refund_signatures,

        // Payments are being made
        paying,

        // No more payments are being made, but some refunds are still spendable.
        waiting_for_all_refunds_to_be_spendable,

        // Ally refunds are spent or double spent, hence we are done
        done
    };

    /**
     * Represents a single channel.
     */
    class Channel {

    public:

        /**
         * @brief The configurations of a channel set by the payor
         *
         * IS THIS NEEDED ???
         */
        class PayorConfiguration {

        public:

            // Constructor
            PayorConfiguration(quint64 funds,
                               const KeyPair & contractKeyPair,
                               const KeyPair & finalKeyPair,
                               quint64 refundFee,
                               quint32 refundLockTime);

            // Getters and setters
            quint64 funds() const;
            void setFunds(const quint64 &funds);

            KeyPair contractKeyPair() const;
            void setContractKeyPair(const KeyPair &contractKeyPair);

            KeyPair finalKeyPair() const;
            void setFinalKeyPair(const KeyPair &finalKeyPair);

            quint64 refundFee() const;
            void setRefundFee(const quint64 &refundFee);

            quint32 getRefundLockTime() const;
            void setRefundLockTime(const quint32 &value);

        private:

            // Funds allocated to output
            quint64 _funds;

            // Controls payour output of multisig
            KeyPair _contractKeyPair;

            // Controls final payment to payor
            KeyPair _finalKeyPair;

            // Fee used in refund transaction, is unlikely to vary across slots,
            quint64 _refundFee;

            // Refund lock time
            quint32 _refundLockTime;
        };

        /**
         * @brief The configurations of a channel set by the payee
         */
        class PayeeConfiguration {

        public:

            // Constructor
            PayeeConfiguration(quint64 price, const PublicKey &contractPk, const PublicKey &finalPk, quint64 paymentFee);

            // Getters and setters
            quint64 price() const;
            void setPrice(const quint64 &price);

            PublicKey contractPk() const;
            void setContractPk(const PublicKey &contractPk);

            PublicKey finalPk() const;
            void setFinalPk(const PublicKey &finalPk);

            quint64 paymentFee() const;
            void setPaymentFee(const quint64 &paymentFee);

        private:

            // Size of single payment
            quint64 _price;

            // Controls payee output of multisig, received in joinin_contract.pk
            PublicKey _contractPk;

            // Controls payee payments, received in sign_refund.pk
            PublicKey _finalPk;

            // Fee used in payment transaction
            quint64 _paymentFee;
        };

        /**
         * @brief Enumeration of possible channel states.
         */
        enum class State {
            unassigned,
            assigned,
            refund_signed
        };

        // Default/Copy constructor and assignemtn operator needed to put in container.
        Channel();
        Channel(const Channel& slot);
        Channel & operator=(const Channel& rhs);

        // Set all fields, e.g. loading from file
        Channel(quint32 index,
             const State &state,
             quint64 price,
             quint64 numberOfPaymentsMade,
             quint64 funds,
             const KeyPair &payorContractKeyPair,
             const PublicKey &payorFinalPk,
             const PublicKey &payeeContractPk,
             const PublicKey &payeeFinalPk,
             const Signature &refund,
             quint64 refundFee,
             quint64 paymentFee,
             quint32 refundLockTime);

        /**
         * Payment channel operations
         */

        // Refund transaction for slot
        Refund refund(const Hash &contractHash) const;

        // Payment transaction for slot, based on current _numberOfPaymentsMade value
        Payment payment(const Hash &contractHash) const;

        // Compute payor refund signature
        void computePayorRefundSignature(const Hash &contractHash) const;

        // Payment signature
        Signature paymentSignature(const Hash &contractHash) const;

        // Registers that a payment was made
        void paymentMade();

        QJsonObject json() const;

        /**
         * Getters and setters
         */

        quint32 index() const;
        void setIndex(const quint32 &index);

        State state() const;
        void setState(const State &state);

        quint64 price() const;
        void setPrice(const quint64 &price);

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

        KeyPair payorFinalKeyPair() const;
        void setPayorFinalKeyPair(const KeyPair &payorFinalKeyPair);

        Signature refund() const;
        void setRefund(const Signature &signature);

        quint64 refundFee() const;
        void setRefundFee(const quint64 &refundFee);

        quint64 paymentFee() const;
        void setPaymentFee(const quint64 &paymentFee);

        PublicKey payeeFinalPk() const;
        void setPayeeFinalPk(const PublicKey &payeeFinalPk);

        quint32 refundLockTime() const;
        void setRefundLockTime(const quint32 &refundLockTime);

    private:

        // Index in contract
        quint32 _index;

        // Slot state
        State _state;

        // Size of single payment
        quint64 _price;

        // Number of payments made
        quint64 _numberOfPaymentsMade;

        // Funds allocated to output
        quint64 _funds;

        // Controls payour output of multisig
        KeyPair _payorContractKeyPair;

        // Controls final payment to payor
        KeyPair _payorFinalKeyPair;

        // Controls payee output of multisig, received in joinin_contract.pk
        PublicKey _payeeContractPk;

        // Controls payee payments, received in sign_refund.pk
        PublicKey _payeeFinalPk;

        // Controls refund for payor
        Signature _payorRefundSignature;

        // Controls refund for payee
        Signature _payeeRefundSignature;

        // Fee used in refund transaction, is unlikely to vary across slots,
        quint64 _refundFee;

        // Fee used in payment transaction
        quint64 _paymentFee;

        // Lock time of refund, received in
        quint32 _refundLockTime;
    };

    // Default constructor
    Payor();

    // Constructor based on members
    Payor(const QSet<Channel::PayorConfiguration> & configurations, const OutputPoint& fundingOutput, const KeyPair& fundingOutputKeyPair);

    // Finds an unassigned slot
    // ========================
    // If one is found then the
    // given payee slot configurations are saved in slot,
    // and if this was last unassigned slot, then payor state is switched.
    quint32 assignUnassignedSlot(const Channel::PayeeConfiguration & configuration);

    // Resets slot state to unassigned
    // ===============================
    // If payor has state State::waiting_for_full_set_of_refund_signatures, then it is switched
    // back to State::waiting_for_full_set_of_sellers, and all all other slot state
    // set to Slot::State::assigned, even if they had Slot::State::refund_signed, since
    // new signatures are now required.
    void unassignSlot(quint32 index);

    // Returns validity of signature for given slot
    // ============================================
    // If payor is collecting signatures
    // and if signature is valid, then
    // 1) saves signature
    // 2) updates slot state refund_assigned
    // 3) updates payor state to all_signed, if all all are now signed.
    bool processRefundSignature(quint32 index, const Signature & signature);

    // Returns the payment signature for the present payment increment of given slot
    Signature presentPaymentSignature(quint32 index) const;

    // Attempts to claim refund for given slot
    // Returns false iff (time lock has not experied on refund or output has been double spent)
    bool claimRefund(quint32 index) const;

    // Checks if output is spent
    bool spent(quint32 index) const;

    // Getters and setters
    OutputPoint fundingOutput() const;
    void setFundingOutput(const OutputPoint &fundingOutput);

    quint32 numberOfSignedSlots() const;
    void setNumberOfSignedSlots(const quint32 &numberOfSignedSlots);

private:

    // Payor state
    State _state;

    // Contract outputs
    QVector<Channel> _channels;

    // Unspent output funding channel
    OutputPoint _fundingOutput;

    // Controls output funding channel
    KeyPair _fundingOutputKeyPair;

    // Controls change output in contract
    KeyPair _changeOutputKeyPair;

    // Change amount sent back to payor,
    // this value, together with the _funds in all the slots
    // determines how much is paid in contract fee implicitly.
    quint64 _changeValue;

    /**
     * Contract:
     * Is recomputed every time a full set of sellers is established,
     * and is cleared whenever a signature failed.
     */

    //Contract _contract;
    Hash _contractHash;
    quint32 _numberOfSignedSlots;
    //Contract contract() const;
    void compute_and_set_contract_tx_hash();

};

#endif // PAYOR_HPP
