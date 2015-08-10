/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/P2PKHTxOut.hpp>

//#include <QJsonObject>

namespace Coin {

P2PKHTxOut::P2PKHTxOut()
    : _value(0) {
}

P2PKHTxOut::P2PKHTxOut(quint64 value, const PublicKey & pk)
    : _value(value)
    , _pk(pk){
}

P2PKHTxOut::P2PKHTxOut(const P2PKHTxOut & p2pkhTxOut)
    : P2PKHTxOut(p2pkhTxOut.value(), p2pkhTxOut.pk()) {
}

P2PKHTxOut & P2PKHTxOut::operator=(const P2PKHTxOut & p2pkhTxOut) {

    _value = p2pkhTxOut.value();
    _pk = p2pkhTxOut.pk();

    return *this;
}

/**
QJsonObject P2PKHTxOut::json() const {

    return QJsonObject{
        {"value", static_cast<int>(_value)},
        {"pk", _pk.toString()}
    };
}
*/

quint64 P2PKHTxOut::value() const {
    return _value;
}

void P2PKHTxOut::setValue(quint64 value) {
    _value = value;
}

PublicKey P2PKHTxOut::pk() const {
    return _pk;
}

void P2PKHTxOut::setPk(const PublicKey & pk) {
    _pk = pk;
}

}
