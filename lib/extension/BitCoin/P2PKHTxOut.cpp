#include "P2PKHTxOut.hpp"

#include <QJsonObject>

P2PKHTxOut::P2PKHTxOut() {
}

P2PKHTxOut::P2PKHTxOut(quint64 value, const PublicKey& pk)
    : _value(value)
    , _pk(pk){
}

P2PKHTxOut::P2PKHTxOut(const P2PKHTxOut& p2pkhTxOut)
    : P2PKHTxOut(p2pkhTxOut.value(), p2pkhTxOut.pk()) {
}

P2PKHTxOut & P2PKHTxOut::operator=(const P2PKHTxOut& p2pkhTxOut) {

    _value = p2pkhTxOut.value();
    _pk = p2pkhTxOut.pk();

    return *this;
}

QJsonObject P2PKHTxOut::json() const {

    return QJsonObject{
        {"value", static_cast<int>(_value)},
        {"pk", _pk.toString()}
    };
}

quint64 P2PKHTxOut::value() const {
    return _value;
}

void P2PKHTxOut::setValue(const quint64 &value) {
    _value = value;
}

PublicKey P2PKHTxOut::pk() const {
    return _pk;
}

void P2PKHTxOut::setPk(const PublicKey &pk) {
    _pk = pk;
}
