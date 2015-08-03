
#include <wallet/OutPoint.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

OutPoint::OutPoint(const Coin::TxId & transactionId, quint32 outputIndex)
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
