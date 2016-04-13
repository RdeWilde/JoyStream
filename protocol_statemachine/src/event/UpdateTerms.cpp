/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 19 2016
 */

#include <protocol_statemachine/event/UpdateTerms.hpp>

namespace joystream {
namespace protocol_statemachine {
namespace event {


    template <class T>
    UpdateTerms<T>::UpdateTerms() {
    }

    template <class T>
    UpdateTerms<T>::UpdateTerms(const T & terms)
        : _terms(terms) {
    }

    template <class T>
    T UpdateTerms<T>::terms() const {
        return _terms;
    }

    template <class T>
    void UpdateTerms<T>::setTerms(const T & terms) {
        _terms = terms;
    }
}
}
}
