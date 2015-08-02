#ifndef TRANSACTION
#define TRANSACTION

#include <wallet/CoinWrappers.hpp>

#include <QDateTime>

class QSqlQuery;

class Transaction {

public:

    Transaction();

    // Getters and seters
private:

    // Transaction id
    Coin::TxId _transactionId;

    // Version of transaction
    quint32 _version;

    // Transaction lock time
    quint32 _lockTime;

    //
    QDateTime _seen;

    // Block id of block to which transaction belongs
    Coin::BlockId _blockId;

    // Fee associated with transaction
    quint64 _fee;

};

#endif // TRANSACTION

