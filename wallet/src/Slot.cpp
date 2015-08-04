/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/Slot.hpp>

#include <QSqlQuery>
#include <QVariant> // QSqlQuery::bind needs it

quint8 Slot::encodeState(State state) {

    switch(state) {
        case State::unassigned: return 0;
        case State::assigned: return 1;
        case State::refund_signed: return 2;

        default:
            Q_ASSERT(false);
    }
}

Slot::State Slot::decodeState(quint8 state) {

    switch(state) {
        case 0: return State::unassigned;
        case 1: return State::assigned;
        case 2: return State::refund_signed;

        default:
            Q_ASSERT(false);
    }
}

Slot::Slot() {

}

