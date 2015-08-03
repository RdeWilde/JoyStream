#ifndef OUTPOINT
#define OUTPOINT

#include <wallet/CoinWrappers.hpp>

#include <QtGlobal> // quint32

class QSqlQuery;

class OutPoint {

public:

    // Constructor from members
    OutPoint(const Coin::TxId & transactionId, quint32 outputIndex);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery();

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery();

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery();

    // Getters and seters

private:

    // Transaction id
    Coin::TxId _transactionId;

    // Index of transaction output
    quint32 _outputIndex;
};

#endif // OUTPOINT

