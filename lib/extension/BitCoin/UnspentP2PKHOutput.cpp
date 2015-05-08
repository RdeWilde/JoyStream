#include "UnspentP2PKHOutput.hpp"

#include <QJsonObject>

UnspentP2PKHOutput::UnspentP2PKHOutput() {
}

UnspentP2PKHOutput::UnspentP2PKHOutput(const KeyPair & keyPair, const OutPoint & output, quint64 value)
    : _keyPair(keyPair)
    , _outPoint(output)
    , _value(value) {
}

UnspentP2PKHOutput::UnspentP2PKHOutput(const QJsonObject & json) {

    QJsonValue keyPairValue = json["keyPair"];
    Q_ASSERT(keyPairValue.isObject());
    _keyPair = KeyPair(keyPairValue.toObject());

    QJsonValue outPointValue = json["outPoint"];
    Q_ASSERT(outPointValue.isObject());
    _outPoint = OutPoint(outPointValue.toObject());

    QJsonValue valueValue = json["value"];
    Q_ASSERT(valueValue.isDouble());
    _value = valueValue.toDouble();
}

bool UnspentP2PKHOutput::operator==(const UnspentP2PKHOutput & o) const {

    return _value == o.value() &&
           _outPoint == o.outPoint() &&
           _keyPair == o.keyPair();
}

QJsonObject UnspentP2PKHOutput::json() const {

    return QJsonObject{
        {"keyPair", _keyPair.json()},
        {"outPoint", _outPoint.json()},
        {"value", static_cast<int>(_value)}
    };
}

KeyPair UnspentP2PKHOutput::keyPair() const {
    return _keyPair;
}

void UnspentP2PKHOutput::setKeyPair(const KeyPair &keyPair) {
    _keyPair = keyPair;
}

OutPoint UnspentP2PKHOutput::outPoint() const {
    return _outPoint;
}

void UnspentP2PKHOutput::setOutPoint(const OutPoint &outPoint) {
    _outPoint = outPoint;
}

quint64 UnspentP2PKHOutput::value() const {
    return _value;
}

void UnspentP2PKHOutput::setValue(quint64 value) {
    _value = value;
}
