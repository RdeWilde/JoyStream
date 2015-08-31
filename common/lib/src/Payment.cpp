/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#include <common/Payment.hpp>

namespace Coin {

Payment::Payment()
    : _value(0) {
}

Payment::Payment(quint64 value, const PublicKey & pk)
    : _value(value)
    , _pk(pk){
}

Payment::Payment(const Payment & o)
    : Payment(o.value(), o.pk()) {
}

Payment & Payment::operator=(const Payment & o) {

    _value = o.value();
    _pk = o.pk();

    return *this;
}
quint64 Payment::value() const {
    return _value;
}

void Payment::setValue(quint64 value) {
    _value = value;
}

PublicKey Payment::pk() const {
    return _pk;
}

void Payment::setPk(const PublicKey & pk) {
    _pk = pk;
}

}
