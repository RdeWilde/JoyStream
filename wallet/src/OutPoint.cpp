
#include <wallet/OutPoint.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

OutPoint::OutPoint(const Coin::TransactionId & transactionId, quint32 outputIndex)
    : _transactionId(transactionId)
    , _outputIndex(outputIndex) {
}

QSqlQuery OutPoint::createTableQuery() {

    return QSqlQuery("\
    CREATE TABLE OutPoint (\
        transactionId       BLOB,\
        outputIndex         INTEGER,\
        PRIMARY KEY(transactionId, outputIndex)\
    )");
}

QSqlQuery OutPoint::unboundedInsertQuery() {

    return QSqlQuery("\
    INSERT INTO OutPoint \
    (transactionId, outputIndex)\
    VALUES\
    (:transactionId, :outputIndex)\
    ");
}

QSqlQuery OutPoint::insertQuery() {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery();

    // Bind values to query fields
    query.bindValue(":transactionId", _transactionId.toByteArray());
    query.bindValue(":outputIndex", _outputIndex);

    return query;
}

Coin::TransactionId OutPoint::transactionId() const {
    return _transactionId;
}

void OutPoint::setTransactionId(const Coin::TransactionId &transactionId) {
    _transactionId = transactionId;
}

quint32 OutPoint::outputIndex() const {
    return _outputIndex;
}

void OutPoint::setOutputIndex(quint32 outputIndex) {
    _outputIndex = outputIndex;
}


