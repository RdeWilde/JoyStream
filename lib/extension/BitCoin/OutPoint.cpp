#include "OutPoint.hpp"
#include "Utilities.hpp"

#include <QJsonObject>
#include <QJsonValue>
#include <QStringList>
#include <QHash>

OutPoint::OutPoint() {
}

OutPoint::OutPoint(const TxId & txId, quint32 index)
    : _txId(txId)
    , _index(index) {
}

OutPoint::OutPoint(const OutPoint& o)
    : OutPoint(o.txId(), o.index()) {
}

OutPoint & OutPoint::operator=(const OutPoint& o) {
    _txId = o.txId();
    _index = o.index();

    return *this;
}

OutPoint::OutPoint(const QJsonObject & json) {

    // _hash
    _txId = TxId(Utilities::GET_STRING(json, "hash"));

    // _index
    _index = Utilities::GET_DOUBLE(json, "index");
}

/*
bool OutPoint::operator==(const OutPoint & o) {
    return _txId == o.txId() && _index == o.index();
}
*/

bool operator==(const OutPoint & lhs, const OutPoint & rhs) {
    return (lhs.txId() < rhs.txId()) || ((lhs.txId() == rhs.txId()) && (lhs.index() < rhs.index()));
}

/*
bool OutPoint::operator<(const OutPoint & o) {
    return (_txId < o.txId()) || ((_txId == o.txId()) && (_index < o.index()));
}
*/

bool operator<(const OutPoint & lhs, const OutPoint & rhs) {
    return (lhs.txId() < rhs.txId()) || ((lhs.txId() == rhs.txId()) && (lhs.index() < rhs.index()));
}

QJsonObject OutPoint::json() const {
    return QJsonObject {
        {"hash", _txId.toString()},
        {"index", static_cast<int>(_index)}
    };
}

OutPoint::OutPoint(const QString & string) {

    QStringList list = string.split("-");

    if(list.size() != 2)
        throw new std::exception("Only a single delimiter - should occur in string.");

    // Parse _hash
    _txId = TxId(list[0]);

    // Parse _index
    bool ok;
    QString rightToken = list[1];

    _index = rightToken.toInt(&ok);

    if(!ok)
        throw new std::exception("Could not convert second token.");
}

QString OutPoint::toString() const {
    return _txId.toString() + "-" + QString::number(_index);
}

TxId OutPoint::txId() const {
    return _txId;
}

void OutPoint::setTxId(const TxId & txId){
    _txId = txId;
}

quint32 OutPoint::index() const {
    return _index;
}

void OutPoint::setIndex(quint32 index) {
    _index = index;
}

uint qHash(const OutPoint & o) {
    return qHash(o.toString());
}
