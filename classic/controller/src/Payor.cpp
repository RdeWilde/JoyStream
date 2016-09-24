/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2016
 */

#include <controller/Payor.hpp>

namespace joystream {
namespace classic {
namespace controller {

Payor::Payor(core::Payor * payor)
    : _payor(payor) {
}

void Payor::setPrice(quint64) {

}

void Payor::setNumberOfPaymentsMade(quint64) {

}

void Payor::setFunds(quint64) {

}

void Payor::setSettlementFee(quint64) {

}

void Payor::setRefundLockTime(quint32) {

}

void Payor::setAnchorChanged(const Coin::typesafeOutPoint &) {

}

}
}
}
