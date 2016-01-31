/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PAYMENT_CHANNEL_PAYOR_HPP
#define PAYMENT_CHANNEL_PAYOR_HPP

#include <paymentchannel/PayorState.hpp>
#include <paymentchannel/ChannelState.hpp>
#include <common/Network.hpp>
#include <common/PublicKey.hpp>
#include <common/PrivateKey.hpp>
#include <common/KeyPair.hpp>
#include <common/TransactionId.hpp>
#include <common/Signature.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/UnspentP2PKHOutput.hpp>

#include <CoinCore/CoinNodeData.h> // Coin::Transaction

namespace Coin {
    class Transaction;
    class TxOut;
    class P2SHScriptPubKey;
}

namespace joystream {
namespace paymentchannel {

    class ChannelConfiguration;
    class PayorConfiguration;
    class Contract;
    class Commitment;
    class Refund;
    class Settlement;

    /**
     * Manages the payor side of a 1-to-N payment channel using design in CBEP.
     * https://github.com/bedeho/CBEP
     *
     */
    class Payor {

    public:

        // Network
        const static Coin::Network network;

        // Represents a single channel.
        class Channel {

        public:

            // Default constructor
            Channel();

            // Construct from configuration
            Channel(const ChannelConfiguration & configuration);

            // Default/Copy constructor and assignemtn operator needed to put in container.
            // Channel(const Channel& slot);
            // Channel & operator=(const Channel& rhs);

            // Set all fields, e.g. loading from file
            Channel(quint32 index,
                 ChannelState state,
                 quint64 price,
                 quint64 numberOfPaymentsMade,
                 quint64 funds,
                 const Coin::KeyPair & payorContractKeyPair,
                 const Coin::KeyPair & payorFinalKeyPair,
                 const Coin::PublicKey & payeeContractPk,
                 const Coin::PublicKey & payeeFinalPk,
                 const Coin::Signature & payorRefundSignature,
                 const Coin::Signature & payeeRefundSignature,
                 quint64 refundFee,
                 quint64 paymentFee,
                 quint32 refundLockTime);

            // Outpoint in contract transaction to which channel comitment corresponds
            Coin::typesafeOutPoint contractOutPoint(const Coin::TransactionId & contractTxId) const;

            /**
             * Perhaps hide these two routines, and reintroduce wrappers below
             * such that payor does not need to deal with them directly.
             * Later...
             */

            // Commitment for channel
            Commitment commitment() const;

            // Refund for channel
            Refund refund(const Coin::TransactionId & contractTxId) const;

            // Payment for channel
            Settlement settlement(const Coin::TransactionId & contractTxId) const;

            /**
            // (unsigned) Transaction spending contract output
            Coin::Transaction contractSpendingTransaction(const Coin::TransactionId & contractHash,
                                                          quint64 returnedToPayor,
                                                          quint32 refundLockTime = 0) const;

            // Compute payor refund signature
            Coin::Signature createPayorRefundSignature(const Coin::TransactionId & contractHash) const;

            // Payment signature
            Coin::Signature createPaymentSignature(const Coin::TransactionId & contractHash) const;

            // Validates payee refund signature
            bool validateRefundSignature(const Coin::TransactionId & contractHash, const Coin::Signature & payeeSig) const;
            */

            // Increment payment counter, should only be done in response to valid
            // payment signature
            void increaseNumberOfPayments();

            // Getters and setters
            quint32 index() const;
            void setIndex(quint32 index);

            ChannelState state() const;
            void setState(ChannelState state);

            quint64 price() const;
            void setPrice(quint64 price);

            quint64 numberOfPaymentsMade() const;
            void setNumberOfPaymentsMade(quint64 numberOfPaymentsMade);

            quint64 funds() const;
            void setFunds(quint64 funds);

            Coin::KeyPair payorContractKeyPair() const;
            void setPayorContractKeyPair(const Coin::KeyPair & payorContractKeyPair);

            Coin::KeyPair payorFinalKeyPair() const;
            void setPayorFinalKeyPair(const Coin::KeyPair & payorFinalKeyPair);

            Coin::PublicKey payeeContractPk() const;
            void setPayeeContractPk(const Coin::PublicKey & payeeContractPk);

            Coin::PublicKey payeeFinalPk() const;
            void setPayeeFinalPk(const Coin::PublicKey & payeeFinalPk);

            Coin::Signature payorRefundSignature() const;
            void setPayorRefundSignature(const Coin::Signature & payorRefundSignature);

            Coin::Signature payeeRefundSignature() const;
            void setPayeeRefundSignature(const Coin::Signature & payeeRefundSignature);

            quint64 refundFee() const;
            void setRefundFee(quint64 refundFee);

            quint64 paymentFee() const;
            void setPaymentFee(quint64 paymentFee);

            quint32 refundLockTime() const;
            void setRefundLockTime(quint32 refundLockTime);

        private:

            // Index in contract
            quint32 _index;

            // Channel state
            ChannelState _state;

            // Size of single payment
            quint64 _price;

            // Number of payments made
            quint64 _numberOfPaymentsMade;

            // Funds allocated to output
            quint64 _funds;

            // Controls payour output of multisig
            Coin::KeyPair _payorContractKeyPair;

            // Controls final payment to payor
            Coin::KeyPair _payorFinalKeyPair;

            // Controls payee output of multisig, received in joinin_contract.pk
            Coin::PublicKey _payeeContractPk;

            // Controls payee payments, received in sign_refund.pk
            Coin::PublicKey _payeeFinalPk;

            // Controls refund for payor
            Coin::Signature _payorRefundSignature;

            // Controls refund for payee
            Coin::Signature _payeeRefundSignature;

            // Fee used in refund transaction, is unlikely to vary across slots,
            quint64 _refundFee;

            // Fee used in payment transaction
            quint64 _paymentFee;

            // Lock time of refund, received in
            quint32 _refundLockTime;
        };

        // Default constructor
        Payor();

        // Constructor based on configuration
        Payor(const PayorConfiguration & configuration);

        // Finds an unassigned slot
        // ========================
        // If one is found then the
        // given payee slot configurations are saved in slot,
        // and if this was last unassigned slot, then payor state is switched.
        quint32 assignUnassignedSlot(quint64 price, const Coin::PublicKey & contractPk, const Coin::PublicKey & finalPk, quint32 refundLockTime);

        // Resets slot state to unassigned
        // ===============================
        // If payor has state State::waiting_for_full_set_of_refund_signatures, then it is switched
        // back to State::waiting_for_full_set_of_sellers, and all all other slot state
        // set to Slot::State::assigned, even if they had Slot::State::refund_signed, since
        // new signatures are now required.
        void unassignSlot(quint32 index);

        // Generates contract transaction
        // ===============================
        // Explain ...
        Contract contract() const;

        // Generates contract transaction
        // ===============================
        //Coin::Transaction contractTransaction() const;

        // Returns validity of signature for given slot
        // ============================================
        // If payor is collecting signatures
        // and if signature is valid, then
        // 1) saves signature
        // 2) updates slot state refund_assigned
        // 3) updates payor state to all_signed, if all all are now signed.
        bool processRefundSignature(quint32 index, const Coin::Signature & signature);

        // Increments the payment counter for the given channel
        // ============================================
        quint64 incrementPaymentCounter(quint32 index);

        // Returns the payment signature for the present payment increment of given slot
        // ============================================
        Coin::Signature getPresentPaymentSignature(quint32 index) const;

        quint32 numberOfChannels() const;

        quint32 numberOfChannelsWithState(ChannelState state) const;

        bool isFull() const;

        bool allRefundsSigned() const;

        // Some utility routines
        static quint64 computeContractFee(int numberOfSellers, quint64 feePerKb);
        static quint64 minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB);

        // Getters and setters
        PayorState state() const;
        void setState(PayorState state);

        std::vector<Channel> & channels();
        const Payor::Channel & channel(int i) const;

        /**
        Coin::typesafeOutPoint fundingOutPoint() const;
        void setFundingOutPoint(const Coin::typesafeOutPoint & fundingOutPoint);

        Coin::TransactionId contractHash() const;
        void setContractHash(const Coin::TransactionId & contractHash);
        */

        Coin::Transaction contractTransaction() const;

        quint32 numberOfSignatures() const;
        void setNumberOfSignatures(quint32 numberOfSignatures);

        quint64 contractFee() const;

    private:

        // Payor state
        PayorState _state;

        // Contract outputs
        std::vector<Channel> _channels;

        /**
        // Unspent output funding channel
        //OutPoint _fundingOutPoint;

        // Value of output funding channel (had to add due to bitcore requirements, remove later?)
        //quint64 _fundingValue;

        // Controls output funding channel
        //KeyPair _fundingOutputKeyPair;
        */

        // Funding
        Coin::UnspentP2PKHOutput _utxo;

        // Controls change output in contract
        Coin::KeyPair _changeOutputKeyPair;

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
        //Coin::TransactionId _contractTxId;

        // Contract transaction
        // =============================================================
        // ** Reason this is saved is because it should not be
        // resigned when needed multiple times, as this will change
        // signature, which will change contract TxId, which may
        // invalidate channel if not used with care, e.g. if broadcasted
        Coin::Transaction _contractTx;

        quint32 _numberOfSignatures;
    };

}
}

#endif // PAYMENT_CHANNEL_PAYOR_HPP
