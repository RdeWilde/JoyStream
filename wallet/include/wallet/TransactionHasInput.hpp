#ifndef TRANSACTION_HAS_INPUT_HPP
#define TRANSACTION_HAS_INPUT_HPP

#include <wallet/CoinWrappers.hpp>
#include <wallet/Transaction.hpp>
#include <wallet/Input.hpp>

class TransactionHasInput {

public:

    // Construct from members
    TransactionHasInput(const Coin::TransactionId & transactionId, const Input & input);

    // Constructor from record
    // TransactionHasInput(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery();

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery();

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery();

    // Getters and setters
    Coin::TransactionId transactionId() const;
    void setTransactionId(const Coin::TransactionId & transactionId);

    Input input() const;
    void setInput(const Input & input);

private:

    // Id of transaction
    Coin::TransactionId _transactionId;

    // Input in transaction
    Input _input;
};

#endif // TRANSACTIONHASINPUT_HPP
