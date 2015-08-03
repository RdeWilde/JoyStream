#ifndef OUTPOINT
#define OUTPOINT

#include <wallet/CoinWrappers.hpp>

#include <QtGlobal> // quint32

class QSqlQuery;

class OutPoint {

public:

    // Constructor from members
    OutPoint(const Coin::TransactionId & transactionId, quint32 outputIndex);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery();

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery();

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery();

    // Getters and setters
    Coin::TransactionId transactionId() const;
    void setTransactionId(const Coin::TransactionId & transactionId);

    quint32 outputIndex() const;
    void setOutputIndex(quint32 outputIndex);

private:

    // Transaction id
    Coin::TransactionId _transactionId;

    // Index of transaction output
    quint32 _outputIndex;
};

#endif // OUTPOINT

