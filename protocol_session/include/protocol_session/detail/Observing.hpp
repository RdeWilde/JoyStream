/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 29 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_OBSERVING_HPP
#define JOYSTREAM_PROTOCOLSESSION_OBSERVING_HPP

#include <protocol_session/Session.hpp>

namespace joystream {
namespace protocol_session {
namespace detail {

template <class ConnectionIdType>
class Selling;

template <class ConnectionIdType>
class Buying;

template <class ConnectionIdType>
class Observing {

public:

    Observing(Session<ConnectionIdType> *);

    //// Change mode

    // Turn into session in sell mode
    // Caller owns returned object.
    Selling<ConnectionIdType> * toSellMode();

    // Turn into session in observe mode
    // Caller owns returne object.
    Buying<ConnectionIdType> * toBuyMode();

private:

    //// Members

    // Reference to core of session
    Session<ConnectionIdType> * _session;

};

}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/detail/Observing.cpp>
#endif // JOYSTREAM_PROTOCOLSESSION_OBSERVING_HPP
