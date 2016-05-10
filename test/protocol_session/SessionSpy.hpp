/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, May 9 2016
 */

#ifndef SESSIONSPY_HPP
#define SESSIONSPY_HPP

#include <protocol_session/protocol_session.hpp>

#include <map>
#include <cstdint>

namespace joystream {
namespace protocol_session {

    enum class DisconnectCause;

    template <class ConnectionIdType>
    class Session;
}
namespace protocol_wire {
    class ExtendedMessagePayload;
}
}

using namespace joystream;
using namespace joystream::protocol_session;

template <class ConnectionIdType>
class ConnectionSpy {

public:

    ConnectionSpy(const ConnectionIdType &);

    void reset();

    SendMessageOnConnection sendHook();

    bool messageSent() const;
    protocol_wire::ExtendedMessagePayload * message() const;

private:

    void hook(const protocol_wire::ExtendedMessagePayload &);

    ConnectionIdType _id;

    bool _messageSent;
    protocol_wire::ExtendedMessagePayload * _message;

};

template <class ConnectionIdType>
class SessionSpy {

public:

    SessionSpy();

    Session<ConnectionIdType> * createMonitoredSession();

    ConnectionSpy<ConnectionIdType> * addMonitoredConnection(const ConnectionIdType &);

    void reset();

private:

    Session<ConnectionIdType> * _session;

    std::map<ConnectionIdType, ConnectionSpy<ConnectionIdType>> _connectionSpies;

    //// General

    // RemovedConnectionCallbackHandler
    bool _connectionRemoved;
    DisconnectCause _casue;

    // GenerateKeyPairsCallbackHandler
    bool _keyPairsGenerated;
    int _numberOfKeyPairsGenerated;

    // GenerateP2PKHAddressesCallbackHandler
    bool _p2pkhAddressGenerated;
    int _numberOfGenerateP2PKHAddresses;

    // SendMessageOnConnection
    // Inside ConnectionSpy object

    //// Buying

    // BroadcastTransaction
    bool _broadcastTransaction;
    Coin::Transaction _transaction;

    // FullPieceArrived
    bool _fullPieceArrived;
    uint fullPieceArrivedId;
    const protocol_wire::PieceData data;
    int index;

    //// Selling

    // LoadPieceForBuyer

    // ClaimLastPayment

    // AnchorAnnounced

};

#endif // SESSIONSPY_HPP
