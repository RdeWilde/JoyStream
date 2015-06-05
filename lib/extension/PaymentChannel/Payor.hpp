#ifndef PAYOR_HPP
#define PAYOR_HPP

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"
#include "extension/BitCoin/KeyPair.hpp"
#include "extension/BitCoin/TxId.hpp"
#include "extension/BitCoin/Signature.hpp"
#include "extension/BitCoin/OutPoint.hpp"

#include "extension/BitCoin/UnspentP2PKHOutput.hpp"

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

        // Contract has been broadcasted, and at least one contract output
        // remains unspent and with sufficient balanc
        can_pay,

        // When
        all_contract_outputs_spent
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
         * @brief Peristant state of Channel.
         */
        class Configuration {

        public:

            // Default constructor.
            Configuration();

            // Constructor from members.
            Configuration(quint32 index,
                          State state,
                          quint64 price,
                          quint64 numberOfPaymentsMade,
                          quint64 funds,
                          const KeyPair & payorContractKeyPair,
                          const KeyPair & payorFinalKeyPair,
                          const PublicKey & payeeContractPk,
                          const PublicKey & payeeFinalPk,
                          const Signature & payorRefundSignature,
                          const Signature & payeeRefundSignature,
                          quint64 refundFee,
                          quint64 paymentFee,
                          quint32 refundLockTime);

            // Getters and setters
            quint32 index() const;
            void setIndex(quint32 index);

            State state() const;
            void setState(State state);

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
         * @brief Snap shot state of channel
         *
         * weird class, will eventually be as large as Channel itself?
         */
        class Status {

        public:

            // Default constructor
            Status();

            // Constructor from members
            Status(quint32 index,
                   State state,
                   quint64 price,
                   quint64 numberOfPaymentsMade,
                   quint64 funds,
                   quint32 refundLockTime);

            // Getters and setters
            quint32 index() const;
            void setIndex(quint32 index);

            State state() const;
            void setState(State state);

            quint64 price() const;
            void setPrice(quint64 price);

            quint64 numberOfPaymentsMade() const;
            void setNumberOfPaymentsMade(quint64 numberOfPaymentsMade);

            quint64 funds() const;
            void setFunds(quint64 funds);

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

            // Lock time of refund, received in
            quint32 _refundLockTime;
        };

        // Default constructor
        Channel();

        // Construct from configuration
        Channel(const Channel::Configuration & configuration);

        // Default/Copy constructor and assignemtn operator needed to put in container.
        // Channel(const Channel& slot);
        // Channel & operator=(const Channel& rhs);

        // Set all fields, e.g. loading from file
        Channel(quint32 index,
             const State &state,
             quint64 price,
             quint64 numberOfPaymentsMade,
             quint64 funds,
             const KeyPair & payorContractKeyPair,
             const KeyPair & payorFinalKeyPair,
             const PublicKey & payeeContractPk,
             const PublicKey & payeeFinalPk,
             const Signature & payorRefundSignature,
             const Signature & payeeRefundSignature,
             quint64 refundFee,
             quint64 paymentFee,
             quint32 refundLockTime);

        /**
         * Payment channel operations
         */

        // Refund transaction for slot
        //Refund refund(const Hash &contractHash) const;

        // Payment transaction for slot, based on current _numberOfPaymentsMade value
        //Payment payment(const Hash &contractHash) const;

        // Compute payor refund signature
        void computeAndSetPayorRefundSignature(const TxId &contractHash);

        // Payment signature
        Signature paymentSignature(const TxId &contractHash) const;

        // Registers that a payment was made
        void paymentMade();

        Status status() const;

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
        void computeAndSetPayorRefundSignature(const Signature & payorRefundSignature);

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

        // Default constructor
        Status();

        // Construct from members
        Status(const QVector<Channel::Status> & channels,
               State state,
               const UnspentP2PKHOutput & utxo,
               quint64 changeValue,
               quint64 contractFee,
               const TxId & contractTxId,
               quint32 numberOfSignatures);

        // Getters and setters
        QVector<Channel::Status> channels() const;
        void setChannels(const QVector<Channel::Status> & channels);

        State state() const;
        void setState(State state);

        UnspentP2PKHOutput utxo() const;
        void setUtxo(const UnspentP2PKHOutput & utxo);

        quint64 changeValue() const;
        void setChangeValue(quint64 changeValue);

        quint64 contractFee() const;
        void setContractFee(quint64 contractFee);

        TxId contractTxId() const;
        void setContractTxId(const TxId &contractTxId);

        quint32 numberOfSignatures() const;
        void setNumberOfSignatures(quint32 numberOfSignatures);

    private:

        // Status of channels
        QVector<Channel::Status> _channels;

        // State of payor
        State _state;

        // Funding utxo
        UnspentP2PKHOutput _utxo;

        // Change amount sent back to payor,
        // this value, together with the _funds in all the slots
        // determines how much is paid in contract fee implicitly.
        quint64 _changeValue;

        // Contract fee
        quint64 _contractFee;

        // Transaction id of contract
        TxId _contractTxId;

        // Number of valid signatures collected
        quint32 _numberOfSignatures;
    };

    /**
     * @brief Persistant state of payor
     *
     */
    class Configuration {

    public:

        // Default constructor
        Configuration();

        // Constructor for a fresh payor.
        Configuration(State state,
                      const QVector<Channel::Configuration> & channels,
                      const UnspentP2PKHOutput & utxo,
                      const KeyPair & changeOutputKeyPair,
                      quint64 changeValue,
                      quint64 contractFee,
                      const TxId & contractHash,
                      quint32 numberOfSignatures);

        // Getters and setters
        State state() const;
        void setState(State state);

        QVector<Channel::Configuration> channels() const;
        void setChannels(const QVector<Channel::Configuration> & channels);

        UnspentP2PKHOutput utxo() const;
        void setUtxo(const UnspentP2PKHOutput &utxo);

        /**
        OutPoint fundingOutPoint() const;
        void setFundingOutPoint(const OutPoint & fundingOutPoint);

        quint64 fundingValue() const;
        void setFundingValue(quint64 fundingValue);

        KeyPair fundingOutputKeyPair() const;
        void setFundingOutputKeyPair(const KeyPair & fundingOutputKeyPair);
        */

        KeyPair changeOutputKeyPair() const;
        void setChangeOutputKeyPair(const KeyPair & changeOutputKeyPair);

        quint64 changeValue() const;
        void setChangeValue(quint64 changeValue);

        quint64 contractFee() const;
        void setContractFee(quint64 contractFee);

        TxId contractHash() const;
        void setContractHash(const TxId & contractHash);

        quint32 numberOfSignatures() const;
        void setNumberOfSignatures(quint32 numberOfSignatures);


    private:

        // Payor state
        State _state;

        // Contract outputs
        QVector<Channel::Configuration> _channels;

        // Unspent output funding channel
        //OutPoint _fundingOutPoint;

        // Value of output funding channel (had to add due to bitcore requirements, remove later?)
        //quint64 _fundingValue;

        // Controls output funding channel
        //KeyPair _fundingOutputKeyPair;

        // Funding
        UnspentP2PKHOutput _utxo;

        // Controls change output in contract
        KeyPair _changeOutputKeyPair;

        // Change amount sent back to payor,
        // this value, together with the _funds in all the slots
        // determines how much is paid in contract fee implicitly.
        quint64 _changeValue;

        // Contract fee
        quint64 _contractFee;

        // Contract _contract;
        TxId _contractHash;

        // Number of valid refund signatures
        quint32 _numberOfSignatures;

    };

    // Default constructor
    Payor();

    // Constructor based on configuration
    Payor(const Payor::Configuration & configuration);

    // Finds an unassigned slot
    // ========================
    // If one is found then the
    // given payee slot configurations are saved in slot,
    // and if this was last unassigned slot, then payor state is switched.
    quint32 assignUnassignedSlot(quint64 price, const PublicKey & contractPk, const PublicKey & finalPk, quint32 refundLockTime);

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

    // Broadcast the current contract
    // ============================================
    void broadcast_contract();

    // Increments the payment counter for the given channel
    // ============================================
    quint64 incrementPaymentCounter(quint32 index);

    // Returns the payment signature for the present payment increment of given slot
    // ============================================
    Signature getPresentPaymentSignature(quint32 index) const;

    // Attempts to claim refund for given slot
    // Returns false iff (time lock has not experied on refund or output has been double spent)
    bool claimRefund(quint32 index) const;

    // Checks if output is spent
    bool spent(quint32 index) const;

    Status status() const;

    quint32 numberOfChannels() const;

    quint32 numberOfChannelsWithState(Channel::State state) const;

    bool isFull() const;

    bool allRefundsSigned() const;

    // Some utility routines
    static quint64 computeContractFee(int numberOfSellers, quint64 feePerKb);
    static quint64 minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB);

    // Getters and setters
    State state() const;
    void setState(State state);

    QVector<Channel> & channels();
    const Payor::Channel & channel(int i) const;

    OutPoint fundingOutPoint() const;
    void setFundingOutPoint(const OutPoint & fundingOutPoint);

    TxId contractHash() const;
    void setContractHash(const TxId & contractHash);

    quint32 numberOfSignatures() const;
    void setNumberOfSignatures(quint32 numberOfSignatures);

    quint64 contractFee() const;

private:

    // Payor state
    State _state;

    // Contract outputs
    QVector<Channel> _channels;

    // Unspent output funding channel
    //OutPoint _fundingOutPoint;

    // Value of output funding channel (had to add due to bitcore requirements, remove later?)
    //quint64 _fundingValue;

    // Controls output funding channel
    //KeyPair _fundingOutputKeyPair;

    // Funding
    UnspentP2PKHOutput _utxo;

    // Controls change output in contract
    KeyPair _changeOutputKeyPair;

    // Change amount sent back to payor,
    // this value, together with the _funds in all the slots
    // determines how much is paid in contract fee implicitly.
    quint64 _changeValue;

    // Contract fee
    quint64 _contractFee;

    /**
     * Contract:
     * ==========================
     * Is recomputed every time a full set of sellers is established,
     * and is cleared whenever a signature failed.
     */

    // Add variable here for number of channels assignd as well

    // Contract _contract;
    TxId _contractTxId;

    quint32 _numberOfSignatures;
};

#endif // PAYOR_HPP
