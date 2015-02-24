#ifndef PAYOR_HPP
#define PAYOR_HPP

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"
#include "extension/BitCoin/KeyPair.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/Signature.hpp"
#include "extension/BitCoin/OutPoint.hpp"

#include <QVector>

/**
 * Manages the payor side of a 1-to-N payment channel using design in CBEP.
 * https://github.com/bedeho/CBEP
 *
 */
class Payor
{
public:

    /**
     * @brief States of a payor.
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
         * @brief States of channel.
         */
        enum class State {
            unassigned,
            assigned,
            refund_signed
        };

        /**
         * @brief Payor provided settings in channel,
         * as through wire message.
         */
        class PayorSettings {

        public:

            // Default constructor
            PayorSettings();

            // Member constructor
            PayorSettings(quint64 funds, const KeyPair & contractKeyPair, const KeyPair & finalKeyPair);

            // Copy constructor
            PayorSettings(const PayorSettings & o);

            // Assignment operator
            PayorSettings & PayorSettings::operator=(const PayorSettings & o);

            // Getters and setters
            quint64 funds() const;
            void setFunds(quint64 funds);

            KeyPair contractKeyPair() const;
            void setContractKeyPair(const KeyPair &contractKeyPair);

            KeyPair finalKeyPair() const;
            void setFinalKeyPair(const KeyPair &finalKeyPair);

        private:

            // Funds allocated to output
            quint64 _funds;

            // Controls payour output of multisig
            KeyPair _contractKeyPair;

            // Controls final payment to payor
            KeyPair _finalKeyPair;
        };

        /**
         * @brief Payee provided settings in channel,
         * as through wire message.
         */
        class PayeeSettings {

        public:

            // Default constructor
            PayeeSettings();

            // Constructor based on members
            PayeeSettings(quint64 price, const PublicKey & contractPk, const PublicKey & finalPk, quint32 refundLockTime);

            // Copy constructor
            PayeeSettings(const PayeeSettings & o);

            // Assignment operator
            PayeeSettings & PayeeSettings::operator=(const PayeeSettings & o);

            // Getters and setters
            quint64 price() const;
            void setPrice(quint64 price);

            PublicKey contractPk() const;
            void setContractPk(const PublicKey & contractPk);

            PublicKey finalPk() const;
            void setFinalPk(const PublicKey & finalPk);

            quint32 refundLockTime() const;
            void setRefundLockTime(quint32 refundLockTime);

        private:

            // Size of single payment
            quint64 _price;

            // Controls payee output of multisig, received in joinin_contract.pk
            PublicKey _contractPk;

            // Controls payee payments, received in sign_refund.pk
            PublicKey _finalPk;

            // Refund lock time
            quint32 _refundLockTime;
        };

        /**
         * @brief Snap shot state of channel
         */
        class Status {

        public:

            // Slot state
            State _state;

            // Number of payments made
            quint64 _numberOfPaymentsMade;



        private:

        };

        // Default constructor
        Channel();

        // Default/Copy constructor and assignemtn operator needed to put in container.
        /*
         *

        Channel(const Channel& slot);
        Channel & operator=(const Channel& rhs);
        */

        // Set all fields, e.g. loading from file
        /*
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
             quint32 refundLockTime);*/

        /**
         * Payment channel operations
         */

        // Refund transaction for slot
        //Refund refund(const Hash &contractHash) const;

        // Payment transaction for slot, based on current _numberOfPaymentsMade value
        //Payment payment(const Hash &contractHash) const;

        // Compute payor refund signature
        void computePayorRefundSignature(const Hash &contractHash);

        // Payment signature
        Signature paymentSignature(const Hash &contractHash) const;

        // Registers that a payment was made
        void paymentMade();

        QJsonObject json() const;

        // Getters and setters
        quint32 index() const;
        void setIndex(quint32 index);

        State state() const;
        void setState(const State & state);

        quint64 price() const;
        void setPrice(quint64 price);

        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(quint64 numberOfPaymentsMade);

        quint64 funds() const;
        void setFunds(quint64 funds);

        KeyPair payorContractKeyPair() const;
        void setPayorContractKeyPair(const KeyPair & payorContractKeyPair);

        KeyPair payorFinalKeyPair() const;
        void setPayorFinalKeyPair(const KeyPair & payorFinalKeyPair);

        PublicKey payeeContractPk() const;
        void setPayeeContractPk(const PublicKey & payeeContractPk);

        PublicKey payeeFinalPk() const;
        void setPayeeFinalPk(const PublicKey & payeeFinalPk);

        Signature payorRefundSignature() const;
        void setPayorRefundSignature(const Signature & payorRefundSignature);

        Signature payeeRefundSignature() const;
        void setPayeeRefundSignature(const Signature & payeeRefundSignature);

        quint64 refundFee() const;
        void setRefundFee(quint64 refundFee);

        quint64 paymentFee() const;
        void setPaymentFee(quint64 paymentFee);

        quint32 refundLockTime() const;
        void setRefundLockTime(quint32 refundLockTime);

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

    /**
     * @brief Snap shot status of payor.
     */
    class Status {

    public:

        // Construct from members
        Status(const QVector<Channel::Status> & channels,
               State state,
               quint32 numberOfSignatures);

        // Getters and setters
        QVector<Channel::Status> channels() const;
        void setChannels(const QVector<Channel::Status> &channels);

        State state() const;
        void setState(const State &state);

        quint32 numberOfSignatures() const;
        void setNumberOfSignatures(const quint32 &numberOfSignatures);

    private:

        // Status of channels
        QVector<Channel::Status> _channels;

        // State of payor
        State _state;

        // Number of valid signatures collected
        quint32 _numberOfSignatures;
    };

    /**
     * @brief Persistant
     */
    class Configuration {

    public:

    private:

    };

    // Default constructor
    Payor();

    // Constructor based on configuration
    //Payor(const Payor::Configuration & configuration);

    // Constructor based on members
    //Payor(const QSet<Channel::PayorConfiguration> & configurations, const OutPoint& fundingOutput, const KeyPair& fundingOutputKeyPair);
    Payor(const OutPoint& fundingOutput, const KeyPair& fundingOutputKeyPair);

    // Add channel
    quint32 addChannel(const Channel::PayorSettings & configuration);

    // Finds an unassigned slot
    // ========================
    // If one is found then the
    // given payee slot configurations are saved in slot,
    // and if this was last unassigned slot, then payor state is switched.
    quint32 assignUnassignedSlot(const Channel::PayeeSettings & configuration);

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
    // ============================================
    Signature getPresentPaymentSignature(quint32 index) const;

    // Attempts to claim refund for given slot
    // Returns false iff (time lock has not experied on refund or output has been double spent)
    bool claimRefund(quint32 index) const;

    // Checks if output is spent
    bool spent(quint32 index) const;

    // Getters and setters
    OutPoint fundingOutput() const;
    void setFundingOutput(const OutPoint & fundingOutput);

    quint32 numberOfSignatures() const;
    void setNumberOfSignatures(quint32 numberOfSignatures);

private:

    // Payor state
    State _state;

    // Contract outputs
    QVector<Channel> _channels;

    // Unspent output funding channel
    OutPoint _fundingOutput;

    // Controls output funding channel
    KeyPair _fundingOutputKeyPair;

    // Controls change output in contract
    KeyPair _changeOutputKeyPair;

    // Change amount sent back to payor,
    // this value, together with the _funds in all the slots
    // determines how much is paid in contract fee implicitly.
    quint64 _changeValue;

    /**
    // Maximum price accepted (satoshies)
    quint64 _maxPrice;

    // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
    quint32 _maxLock;

    // Maximum fee per byte in contract transaction (satoshies)
    quint64 _maxFeePerByte;
    */

    /**
     * Contract:
     * ==========================
     * Is recomputed every time a full set of sellers is established,
     * and is cleared whenever a signature failed.
     */

    //Contract _contract;
    Hash _contractHash;
    quint32 _numberOfSignatures;

};

#endif // PAYOR_HPP
