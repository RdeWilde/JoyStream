/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_SLOT_HPP
#define WALLET_SLOT_HPP

//#include <common/CoinWrappers.hpp>

#include <common/PublicKey.hpp>
#include <common/Signature.hpp>

#include <QtGlobal>

class QSqlQuery;
class QSqlDatabase;

class Slot
{

public:

    /**
     * @brief States of channel.
     */
    enum class State {
        unassigned,
        assigned,
        refund_signed
    };

    static quint8 encodeState(State state);
    static State decodeState(quint8 state);

    // Constructor from members
    Slot(quint64 payerId,
         quint32 index,
         State state,
         quint64 price,
         quint64 numberOfPaymentsMade,
         quint64 funds,
         quint64 payerContractWalletKeyId,
         quint64 payerFinalWalletKeyId,
         const Coin::PublicKey & payeeContractPublicKey,
         const Coin::PublicKey & payeeFinalPublicKey,
         const Coin::Signature & refundSignature,
         const Coin::Signature & lastPaymentSignature,
         quint64 refundFee,
         quint64 paymentFee,
         quint32 refundLockTime);

    // Constructor from record
    // Slot(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery(QSqlDatabase db);

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery(QSqlDatabase db);

    // Getters and setters
    quint64 payerId() const;
    void setPayerId(quint64 payerId);

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

    quint64 payerContractWalletKeyId() const;
    void setPayerContractWalletKeyId(quint64 payerContractWalletKeyId);

    quint64 payerFinalWalletKeyId() const;
    void setPayerFinalWalletKeyId(quint64 payerFinalWalletKeyId);

    Coin::PublicKey payeeContractPublicKey() const;
    void setPayeeContractPublicKey(const Coin::PublicKey & payeeContractPublicKey);

    Coin::PublicKey payeeFinalPublicKey() const;
    void setPayeeFinalPublicKey(const Coin::PublicKey & payeeFinalPublicKey);

    Coin::Signature refundSignature() const;
    void setRefundSignature(const Coin::Signature & refundSignature);

    Coin::Signature lastPaymentSignature() const;
    void setLastPaymentSignature(const Coin::Signature & lastPaymentSignature);

    quint64 refundFee() const;
    void setRefundFee(quint64 refundFee);

    quint64 paymentFee() const;
    void setPaymentFee(quint64 paymentFee);

    quint32 refundLockTime() const;
    void setRefundLockTime(quint32 refundLockTime);

private:

    // Payer to which this slot correponds.
    quint64 _payerId;

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

    // Id of private key which controls payer portion of corresponding contract multisig output.
    quint64 _payerContractWalletKeyId;

    // Id of private key which controls refund, change payment of corresponding contract multisig output.
    quint64 _payerFinalWalletKeyId;

    // Controlls payee portion of corresponding contract output, with raw encoding.
    Coin::PublicKey _payeeContractPublicKey;

    // Controlls payee payment, with raw encoding.
    Coin::PublicKey _payeeFinalPublicKey;

    // Refund signature provided by payee
    Coin::Signature _refundSignature;

    // Last payment signature provided by payee
    Coin::Signature _lastPaymentSignature;

    // Fee used in refund transaction, in satoshis.
    quint64 _refundFee;

    // Fee used in payment transaction, in satoshis.
    quint64 _paymentFee;

    // Transaction id of refund transaction
    quint32 _refundLockTime;
};

#endif // WALLET_SLOT_HPP
