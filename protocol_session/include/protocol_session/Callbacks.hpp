/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 25 2016
 */


#ifndef JOYSTREAM_PROTOCOLSESSION_CALLBACKS_HPP
#define JOYSTREAM_PROTOCOLSESSION_CALLBACKS_HPP

#include <functional>
#include <vector>

namespace joystream {
namespace protocol_session {

// NB: In the future one can separete into two different callbacks,
// one for each mode, with its own set of DisconnectCauses.
enum class DisconnectCause {

    seller_has_interrupted_contract,

    servicing_piece_has_timed_out,

    sent_invalid_piece,

    stopping_session
};

// Removal of a connection from the session: c++11 alias declaration
template <class ConnectionIdType>
using RemovedConnectionCallbackHandler = std::function<void(const ConnectionIdType &, DisconnectCause)>;

// Generate set of key pairs
typedef std::function<std::vector<Coin::KeyPair>(int)> GenerateKeyPairsCallbackHandler;

// Generate set of p2pkh addresses
typedef std::function<std::vector<Coin::P2PKHAddress>(int)> GenerateP2PKHAddressesCallbackHandler;

// Send a message to be sent
typedef std::function<void(const protocol_wire::ExtendedMessagePayload &)> SendMessageOnConnection;

// Broadcasting a transaction
typedef std::function<bool(const Coin::Transaction &)> BroadcastTransaction;

// Process arrival of a full piece, with given index over peer connection with given id
template <class ConnectionIdType>
using FullPieceArrived = std::function<void(const ConnectionIdType &, const protocol_wire::PieceData &, int)>;


}
}

#endif // JOYSTREAM_PROTOCOLSESSION_CALLBACKS_HPP

