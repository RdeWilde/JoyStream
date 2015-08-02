
#include <wallet/Transaction.hpp>

Transaction::Transaction(const Coin::TxId & transactionId,
                         quint32 version,
                         quint32 lockTime,
                         QDateTime seen,
                         const Coin::BlockId & blockId,
                         quint64 fee)
        : _transactionId(transactionId)
        , _version(version)
        , _lockTime(lockTime)
        , _seen(seen)
        , _blockId(blockId)
        , _fee(fee) {
}

QSqlQuery Transaction::createTableQuery() {

    return QSqlQuery("\
    CREATE TABLE Transaction (\
        transactionId       BLOB,\
        version             INTEGER     NOT NULL,\
        lockTime            INTEGER     NOT NULL,\
        seen                INTEGER     NOT NULL,\
        blockId             BLOB,\
        fee                 INTEGER     NOT NULL,\
        PRIMARY KEY(transactionId)\
        FOREIGN KEY blockId REFERENCES BlockHeader(bockId)\
    )");
}

QSqlQuery Transaction::unboundedInsertQuery() {

    return QSqlQuery("\
    INSERT INTO Transaction \
    (transactionId, version, lockTime, seen, blockId, fee)\
    VALUES\
    (:transactionId, :version, :lockTime, :seen, :blockId, :fee)\
    ");
}

QSqlQuery Transaction::insertQuery() {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery();

    // Bind values to query fields
    query.bindValue(":transactionId", _transactionId <== add some stuff here);
    query.bindValue(":version", _version);
    query.bindValue(":lockTime", _lockTime);
    query.bindValue(":seen", _seen.toMSecsSinceEpoch());
    query.bindValue(":blockId", _blockId <== add stuff here);
    query.bindValue(":fee", _fee);

    return query;
}
