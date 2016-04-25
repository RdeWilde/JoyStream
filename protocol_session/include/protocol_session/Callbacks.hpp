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

// Removal of a connection from the session: c++11 alias declaration
template <class ConnectionIdType>
using RemovedConnectionCallbackHandler = std::function<void(const ConnectionIdType &)>;

// Generate set of key pairs
typedef std::function<std::vector<Coin::KeyPair>(int)> GenerateKeyPairsCallbackHandler;

// Generate set of p2pkh addresses
typedef std::function<std::vector<Coin::P2PKHAddress>(int)> GenerateP2PKHAddressesCallbackHandler;

// Send a message to be sent
typedef std::function<void(const protocol_wire::ExtendedMessagePayload &)> SendMessageOnConnection;

// Broadcasting a transaction
typedef std::function<bool(const Coin::Transaction &)> BroadCastTransactionCallbackHandler;

}
}

#endif // JOYSTREAM_PROTOCOLSESSION_CALLBACKS_HPP

