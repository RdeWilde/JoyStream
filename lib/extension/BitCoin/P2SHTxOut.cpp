#include "P2SHTxOut.hpp"

P2SHTxOut::P2SHTxOut() {
}

P2SHTxOut::P2SHTxOut(const P2SHTxOut& p2shTxOut) {
    _value = p2shTxOut.value();
    _firstPk = p2shTxOut.firstPk();
    _secondPk =  p2shTxOut.secondPk();
}

P2SHTxOut & P2SHTxOut::operator=(const P2SHTxOut& p2shTxOut) {
    _value = p2shTxOut.value();
    _firstPk = p2shTxOut.firstPk();
    _secondPk =  p2shTxOut.secondPk();
}

P2SHTxOut::P2SHTxOut(quint64 value, const PublicKey & firstPk, const PublicKey & secondPk)
    : _value(value)
    , _firstPk(firstPk)
    , _secondPk(secondPk) {

}
quint64 P2SHTxOut::value() const {
    return _value;
}

void P2SHTxOut::setValue(const quint64 &value) {
    _value = value;
}

PublicKey P2SHTxOut::firstPk() const {
    return _firstPk;
}

void P2SHTxOut::setFirstPk(const PublicKey &firstPk) {
    _firstPk = firstPk;
}

PublicKey P2SHTxOut::secondPk() const {
    return _secondPk;
}

void P2SHTxOut::setSecondPk(const PublicKey &secondPk) {
    _secondPk = secondPk;
}
