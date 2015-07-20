/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/WalletKey.hpp>

WalletKey::WalletKey() {

}

quint8 WalletKey::encodePurpose(Purpose purpose) {

    switch(purpose) {
        case Purpose::Receive: return 0;
        case Purpose::ContractChange: return 1;
        case Purpose::PayerInContractOutput: return 2;
        case Purpose::ContractFinal: return 3;
        case Purpose::PayeeInContractOutput: return 4;
        case Purpose::ContractPayment: return 5;

        default:
            Q_ASSERT(false);
    }
}

WalletKey::Purpose WalletKey::decodePurpose(quint8 encoded) {

    switch(encoded) {
        case 0: return Purpose::Receive;
        case 1: return Purpose::ContractChange;
        case 2: return Purpose::PayerInContractOutput;
        case 3: return Purpose::ContractFinal;
        case 4: return Purpose::PayeeInContractOutput;
        case 5: return Purpose::ContractPayment;

        default:
            Q_ASSERT(false);
    }
}

quint64 WalletKey::walletSequenceNumber() const {
    return _walletSequenceNumber;
}

void WalletKey::setWalletSequenceNumber(quint64 walletSequenceNumber) {
    _walletSequenceNumber = walletSequenceNumber;
}

WalletKey::Purpose WalletKey::purpose() const {
    return _purpose;
}

void WalletKey::setPurpose(Purpose purpose) {
    _purpose = purpose;
}

QDateTime WalletKey::generated() const {
    return _generated;
}

void WalletKey::setGenerated(const QDateTime & generated) {
    _generated = generated;
}

QString WalletKey::description() const {
    return _description;
}

void WalletKey::setDescription(const QString & description) {
    _description = description;
}
