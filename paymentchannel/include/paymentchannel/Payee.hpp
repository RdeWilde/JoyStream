/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PAYMENT_CHANNEL_PAYEE_HPP
#define JOYSTREAM_PAYMENT_CHANNEL_PAYEE_HPP

#include <common/KeyPair.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/PubKeyHash.hpp>
#include <common/Signature.hpp>

namespace Coin {
    class Transaction;
    class P2SHScriptPubKey;
    class TxOut;
}

namespace joystream {
namespace paymentchannel {

    class Contract;
    class Commitment;
    class Refund;
    class Settlement;

    /**
     * Manages the payee side of a 1-to-N payment channel using design in CBEP.
     * https://github.com/JoyStream/CBEP
     */
    class Payee {

    public:

        Payee();

        Payee(quint64 numberOfPaymentsMade,
              quint32 lockTime,
              quint64 price,
              quint64 funds,
              quint64 settlementFee,
              quint64 refundFee,
              const Coin::typesafeOutPoint & contractOutPoint,
              const Coin::KeyPair & payeeContractKeys,
              const Coin::PubKeyHash & payeeFinalPkHash,
              const Coin::PublicKey & payorContractPk,
              const Coin::PubKeyHash & payorFinalPkHash,
              const Coin::Signature & lastValidPayorPaymentSignature);

        // Creates refund signature
        Coin::Signature generateRefundSignature() const;

        // Attempts to register payment if signature is valid
        // ==================================================
        // A valid signature will lead to an increase of _numberOfPaymentsMade,
        // and storing signature in _lastValidPayorPaymentSignature
        bool registerPayment(const Coin::Signature &);

        // Generates transaction for last payment
        Coin::Transaction lastPaymentTransaction() const;

        // Checks contract validitity
        // ==========================
        // 1) There is tx with _contractOutputPoint in longest chain
        // 2) tx has contract output point
        // 3) scriptSig of tx is controlled by given keys and has the correct quanitity of funds
        // 4) channel has to correct number of participants.
        bool isContractValid(const Coin::Transaction &) const;

        // Commitment
        Commitment commitment() const;

        // Refund
        Refund refund() const;

        // Settlement
        Settlement settlement(int) const;

        // Amount of funds paid
        quint64 amountPaid() const;

        // Getters and setters
        quint64 numberOfPaymentsMade() const;
        void setNumberOfPaymentsMade(quint64);

        quint32 lockTime() const;
        void setLockTime(quint32);

        quint64 price() const;
        void setPrice(quint64);

        quint64 funds() const;
        void setFunds(quint64);

        quint64 settlementFee() const;
        void setSettlementFee(quint64);

        quint64 refundFee() const;
        void setRefundFee(quint64 refundFee);

        Coin::typesafeOutPoint contractOutPoint() const;
        void setContractOutPoint(const Coin::typesafeOutPoint &);

        Coin::KeyPair payeeContractKeys() const;
        void setPayeeContractKeys(const Coin::KeyPair &);

        Coin::PubKeyHash payeeFinalPkHash() const;
        void setPayeeFinalPkHash(const Coin::PubKeyHash &);

        Coin::PublicKey payorContractPk() const;
        void setPayorContractPk(const Coin::PublicKey &);

        Coin::PubKeyHash payorFinalPkHash() const;
        void setPayorFinalPkHash(const Coin::PubKeyHash &);

        Coin::Signature lastValidPayorPaymentSignature() const;
        void setLastValidPayorPaymentSignature(const Coin::Signature &);

    private:

        // The number of payments which have been successfully made
        quint64 _numberOfPaymentsMade;

        // Payment channel lock time
        quint32 _lockTime;

        // Price (#satoshies) increment per payment
        quint64 _price;

        // Amount (#satoshies) assigned to contract output
        quint64 _funds;

        // Amount (#satoshies) used in fee for settlement
        quint64 _settlementFee;

        // Amount (#satoshies) used in fee for refund
        quint64 _refundFee;

        // Contract outpoint from which payments originate
        Coin::typesafeOutPoint _contractOutPoint;

        // Controls payee portion of contract output
        Coin::KeyPair _payeeContractKeys;

        // Controls payee output in payment _lastValidPaymentSignature
        Coin::PubKeyHash _payeeFinalPkHash;

        // Payor key in contract output
        Coin::PublicKey _payorContractPk;

        // Payor P2PKH output in refund/payment
        Coin::PubKeyHash _payorFinalPkHash;

        // The last valid payment signature received, corresponds to _numberOfPaymentsMade
        Coin::Signature _lastValidPayorPaymentSignature;

    };

}
}

#endif // JOYSTREAM_PAYMENT_CHANNEL_PAYEE_HPP
