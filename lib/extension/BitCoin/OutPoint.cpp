#include "OutPoint.hpp"
#include "Utilities.hpp"

#include <QJsonObject>
#include <QJsonValue>
#include <QStringList>

OutPoint::OutPoint() {
}

OutPoint::OutPoint(const OutPoint& outputPoint) {
    _hash = outputPoint.hash();
    _index = outputPoint.index();
}

OutPoint & OutPoint::operator=(const OutPoint& outputPoint) {
    _hash = outputPoint.hash();
    _index = outputPoint.index();

    return *this;
}

OutPoint::OutPoint(const Hash & hash, quint32 index)
    : _hash(hash)
    , _index(index) {
}

OutPoint::OutPoint(const QJsonObject & json) {

    // _hash
    _hash = Hash(Utilities::GET_STRING(json, "_hash"));

    // _index
    _index = Utilities::GET_DOUBLE(json, "_index");
}

#include <QDebug>

bool operator<(const OutPoint & lhs, const OutPoint & rhs) {

    return (lhs.hash() < rhs.hash()) || ((lhs.hash() == rhs.hash()) && (lhs.index() < rhs.index()));

    /**
    bool r = lhs.hash().less(rhs.hash()) || (lhs.hash().equals(rhs.hash()) && (lhs.index() < rhs.index()));

    qDebug() << lhs.hash().toString() << " <? " << rhs.hash().toString();
    qDebug() << "? = " << r;


    return r;
    */
}

QJsonObject OutPoint::toJson() const {
    return QJsonObject {
        {"_hash", _hash.toString()},
        {"_index", static_cast<int>(_index)}
    };
}

OutPoint::OutPoint(const QString & string) {

    QStringList list = string.split("-");

    if(list.size() != 2)
        throw new std::exception("Only a single delimiter - should occur in string.");

    // Parse _hash
    _hash = Hash(list[0]);

    // Parse _index
    bool ok;
    QString rightToken = list[1];

    _index = rightToken.toInt(&ok);

    if(!ok)
        throw new std::exception("Could not convert second token.");
}

#include <QDebug>

QString OutPoint::toString() const {

    //qDebug() << _hash.toString() + "-" + _index;
    //qDebug() << _hash.toString() + "-" + QString::number(_index);
    return _hash.toString() + "-" + QString::number(_index);
}

Hash OutPoint::hash() const {
    return _hash;
}

void OutPoint::setHash(const Hash& hash){
    _hash = hash;
}

quint32 OutPoint::index() const {
    return _index;
}

void OutPoint::setIndex(const quint32& index) {
    _index = index;
}
