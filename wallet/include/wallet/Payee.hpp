/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef PAYEE_HPP
#define PAYEE_HPP

#include <wallet/CoinWrappers.hpp>

#include <QtGlobal>

class QSqlQuery;

class Payee
{
public:

    /**
     * @brief The payee state
     */
    enum class State {

        // Information, provided by payee, required for receiving payments has not been set (using registerPayeeInformation)
        // WHY DO WE EVEN NEED THIS, WE CONTROL THIS INFORMATION ANYWAY
        // waiting_for_payee_information,

        // Information, provided by payor, required for receiving payments has not been set (using registerPayorInformation)
        waiting_for_payor_information,

        // All information required to sign refunds and validate payments has been provided
        has_all_information_required,

        /**
        //
        final_payment_spent,

        //
        refund_spent,
        */
    };

    // State to encoding bijection
    static quint8 encodeState(State state);
    static State decodeState(quint8 state);

    Payee(quint64 id,
          State state,
          quint64 numberOfPaymentsMade,
          const Coin::Signature & lastValidPayerPaymentSignature,
          quint64 price,
          quint64 funds,
          quint32 maximumNumberOfSellers,
          quint64 payeeContractWalletKeyId,
          quint64 payeePaymentWalletKeyId,
          const Coin::TransactionId & contractTransactionId,
          quint32 contractOutput,
          const Coin::PublicKey & payerContractPublicKey,
          const Coin::PublicKey & payerFinalPublicKey,
          const Coin::TransactionId & paymentTransactionId);

    // Constructor from record
    // Payee(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery();

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery();

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery();

    // Getters and setters

private:

    // Payee id
    quint64 _id;

    // State of payee
    State _state;

    // The number of valid payments made to payee
    quint64 _numberOfPaymentsMade;

    // Signature corresponding to last valid payment
    Coin::Signature _lastValidPayerPaymentSignature;

    // Exchange price in satoshis
    quint64 _price;

    // Number of satoshies locked up in channel
    quint64 _funds;

    // The maximum number of sellers in the contract
    quint32 _maximumNumberOfSellers;

    // Id of private key controlling payee portion of contract output
    quint64 _payeeContractWalletKeyId;

    // Id of private key controlling payments
    quint64 _payeePaymentWalletKeyId;

    // Transaction id of contract
    Coin::TransactionId _contractTransactionId;

    // Output in contract
    quint32 _contractOutput;

    // Controlling payer portion of contract multisig output
    Coin::PublicKey _payerContractPublicKey;

    // Controlling change in final payment spending contract output
    Coin::PublicKey _payerFinalPublicKey;

    // Transactin id of refund transaction for slot
    Coin::TransactionId _paymentTransactionId;
};

#endif // PAYEE_HPP
