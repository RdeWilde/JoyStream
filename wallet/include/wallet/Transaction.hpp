#ifndef TRANSACTION
#define TRANSACTION

#include <wallet/CoinWrappers.hpp>

#include <QDateTime>

class QSqlQuery;

class Transaction {

public:

    // Constructor from members
    Transaction(const Coin::TransactionId & transactionId, quint32 version, quint32 lockTime, QDateTime seen, const Coin::BlockId & blockId, quint64 fee);

    // Constructor from record
    // BlockHeader(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery();

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery();

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery();

    // Getters and seters

private:

    // Transaction id
    Coin::TransactionId _transactionId;

    // Version of transaction
    quint32 _version;

    // Transaction lock time
    quint32 _lockTime;

    // When transaction was first seen on network
    QDateTime _seen;

    // Block id of block to which transaction belongs
    Coin::BlockId _blockId;

    // Fee associated with transaction
    quint64 _fee;

};

#endif // TRANSACTION

