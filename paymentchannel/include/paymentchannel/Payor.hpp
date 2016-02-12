/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PAYMENT_CHANNEL_PAYOR_HPP
#define PAYMENT_CHANNEL_PAYOR_HPP

#include <paymentchannel/PayorState.hpp>
#include <paymentchannel/Channel.hpp>


#include <common/Network.hpp>

#include <common/PublicKey.hpp>
#include <common/PrivateKey.hpp>

#include <common/TransactionId.hpp>
#include <common/Signature.hpp>

#include <common/UnspentP2PKHOutput.hpp>

#include <CoinCore/CoinNodeData.h> // Coin::Transaction

namespace Coin {
    class Transaction;
    class TxOut;
    class P2SHScriptPubKey;
}

namespace joystream {
namespace paymentchannel {

    class PayorConfiguration;
    class Contract;

    /**
     * Manages the payor side of a 1-to-N payment channel using design in CBEP.
     * https://github.com/bedeho/CBEP
     *
     */
    class Payor {

    public:

        Payor(const Coin::Network network,
              PayorState state,
              const std::vector<Channel> & channels,
              const Coin::UnspentP2PKHOutput & utxo,
              const Coin::KeyPair & changeOutputKeyPair,
              quint64 changeValue,
              quint64 contractFee);

        // Payor which is initialize to the start of exchange,
        // before payee information is available.
        static Payor unknownPayees();

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
        static quint64 minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB, quint64 paychanSettlementFee);

        // Getters and setters
        PayorState state() const;
        void setState(PayorState state);


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

        // Network
        Coin::Network _network;

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

        // Settlement fee
        quint64 _settlementFee;

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
