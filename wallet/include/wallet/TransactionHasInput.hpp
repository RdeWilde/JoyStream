/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_TRANSACTION_HAS_INPUT_HPP
#define WALLET_TRANSACTION_HAS_INPUT_HPP

//#include <common/CoinWrappers.hpp>
#include <wallet/Transaction.hpp>
#include <wallet/Input.hpp>

class QSqlDatabase;

namespace Wallet {
namespace TransactionHasInput {

    class Record {

    public:

        // Construct from members
        Record(const Coin::TransactionId & transactionId, const Input::Record & input);

        // Constructor from record
        // Record(const QSqlRecord & record);

        // Query inserting this wallet key into corresponding table
        QSqlQuery insertQuery(QSqlDatabase db);

        // Getters and setters
        Coin::TransactionId transactionId() const;
        void setTransactionId(const Coin::TransactionId & transactionId);

        quint32 index() const;
        void setIndex(quint32 index);

        Input::Record input() const;
        void setInput(const Input::Record & input);

    private:

        // Id of transaction
        Coin::TransactionId _transactionId;

        // Index of input in transaction
        quint32 _index;

        // Input in transaction
        Input::Record _input;
    };


    // Query which creates table corresponding to entity
    QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    QSqlQuery unBoundedInsertQuery(QSqlDatabase db);

}
}

#endif // WALLET_TRANSACTION_HAS_INPUT_HPP
