/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Message/JoiningContract.hpp>
#include <core/extension/Message/MessageType.hpp>

#include <QDataStream>
#include <common/PublicKey.hpp>

JoiningContract::JoiningContract() {
}

JoiningContract::JoiningContract(const Coin::PublicKey & contractPk, const Coin::PublicKey & finalPk)
    : _contractPk(contractPk)
    , _finalPk(finalPk) {
}

JoiningContract::JoiningContract(QDataStream & stream) {

    // DOESN'T LINK: stream >> _contractPk >> _finalPk;
    Coin::operator >> (stream, _contractPk);
    Coin::operator >> (stream, _finalPk);
}

Coin::PublicKey JoiningContract::contractPk() const {
    return _contractPk;
}

Coin::PublicKey JoiningContract::finalPk() const {
    return _finalPk;
}

MessageType JoiningContract::messageType() const {
    return MessageType::joining_contract;
}

quint32 JoiningContract::length() const {
    return Coin::PublicKey::length() + Coin::PublicKey::length();
}

void JoiningContract::write(QDataStream & stream) const {

    // DOESN'T LINK: stream << _contractPk << _finalPk;
    Coin::operator << (stream, _contractPk);
    Coin::operator << (stream, _finalPk);
}
