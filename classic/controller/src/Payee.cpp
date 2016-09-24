/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2016
 */

#include <controller/Payee.hpp>
#include <core/Payee.hpp>

namespace joystream {
namespace classic {
namespace controller {

Payee::Payee(core::Payee * payee)
    : _payee(payee){

}

void Payee::setNumberOfPaymentsMade(quint64) {

}

void Payee::setLockTime(quint32) {

}

void Payee::setPrice(quint64) {

}

void Payee::setFunds(quint64) {

}

void Payee::setSettlementFee(quint64) {

}

void Payee::setAnchor(const Coin::typesafeOutPoint & anchor) {

}

}
}
}
