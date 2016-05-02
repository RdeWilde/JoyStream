/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 29 2016
 */

#include <protocol_session/Session.hpp>
#include <protocol_session/detail/Observing.hpp>
#include <protocol_session/detail/Buying.hpp>
#include <protocol_session/detail/Selling.hpp>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    Observing<ConnectionIdType>::Observing(Session<ConnectionIdType> * session)
        : _session(session) {
    }

    template<class ConnectionIdType>
    Selling<ConnectionIdType> * Observing<ConnectionIdType>::toSellMode() {

    }

    template<class ConnectionIdType>
    Buying<ConnectionIdType> * Observing<ConnectionIdType>::toBuyMode() {

    }

}
}
}
