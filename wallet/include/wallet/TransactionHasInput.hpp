/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef TRANSACTION_HAS_INPUT_HPP
#define TRANSACTION_HAS_INPUT_HPP

#include <common/CoinWrappers.hpp>
#include <wallet/Transaction.hpp>
#include <wallet/Input.hpp>

class QSqlDatabase;

class TransactionHasInput {

public:

    // Construct from members
    TransactionHasInput(const Coin::TransactionId & transactionId, const Input & input);

    // Constructor from record
    // TransactionHasInput(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery(QSqlDatabase db);

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery(QSqlDatabase db);

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery(QSqlDatabase db);

    // Getters and setters
    Coin::TransactionId transactionId() const;
    void setTransactionId(const Coin::TransactionId & transactionId);

    quint32 index() const;
    void setIndex(quint32 index);

    Input input() const;
    void setInput(const Input & input);

private:

    // Id of transaction
    Coin::TransactionId _transactionId;

    // Index of input in transaction
    quint32 _index;

    // Input in transaction
    Input _input;
};

#endif // TRANSACTIONHASINPUT_HPP
