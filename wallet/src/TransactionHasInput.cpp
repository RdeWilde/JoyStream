
#include <wallet/TransactionHasInput.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

TransactionHasInput::TransactionHasInput(const Coin::TransactionId & transactionId, const Input & input)
    : _transactionId(transactionId)
    , _input(input) {
}

QSqlQuery TransactionHasInput::createTableQuery() {

    return QSqlQuery("\
    CREATE TABLE TransactionHasInput (\
        transactionId               BLOB        NOT NULL,\
        outPointTransactionId       BLOB        NOT NULL,\
        outPointOutputIndex         INTEGER     NOT NULL,\
        scriptSig                   BLOG        NOT NULL,\
        sequence                    INTEGER     NOT NULL,\
        PRIMARY KEY(transactionId, outPointTransactionId, outPointOutputIndex, scriptSig, sequence),\
        FOREIGN KEY transactionId REFERENCES Transaction(transactionId),\
        FOREIGN KEY (outPointTransactionId, outPointOutputIndex, scriptSig, sequence) REFERENCES Input(outPointTransactionId, outPointOutputIndex, scriptSig, sequence)\
    )");
}

QSqlQuery TransactionHasInput::unboundedInsertQuery() {

    return QSqlQuery("\
    INSERT INTO TransactionHasInput \
    (transactionId, outPointTransactionId, outPointOutputIndex, scriptSig, sequence)\
    VALUES\
    (:transactionId, :outPointTransactionId, :outPointOutputIndex, :scriptSig, :sequence)\
    ");
}

QSqlQuery TransactionHasInput::insertQuery() {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery();

    // Bind values to query fields
    query.bindValue(":transactionId", _transactionId.toByteArray());

    OutPoint outPoint = _input.outPoint();
    query.bindValue(":outPointTransactionId", outPoint.transactionId().toByteArray());
    query.bindValue(":outPointOutputIndex", outPoint.outputIndex());
    query.bindValue(":scriptSig",  _input.scriptSig());
    query.bindValue(":sequence", _input.sequence());

    return query;
}

Coin::TransactionId TransactionHasInput::transactionId() const {
    return _transactionId;
}

void TransactionHasInput::setTransactionId(const Coin::TransactionId & transactionId) {
    _transactionId = transactionId;
}

Input TransactionHasInput::input() const {
    return _input;
}

void TransactionHasInput::setInput(const Input & input) {
    _input = input;
}
