/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 25 2016
 */


#ifndef JOYSTREAM_PROTOCOLSESSION_CALLBACKS_HPP
#define JOYSTREAM_PROTOCOLSESSION_CALLBACKS_HPP

#include <common/P2SHAddress.hpp>
#include <common/P2PKHAddress.hpp>

#include <bitcoin/Common.hpp> // RedeemScriptGenerator

#include <functional>
#include <vector>

#include <QtGlobal> // quint64

namespace Coin {
    class typesafeOutPoint;
    class PublicKey;
}

namespace joystream {
namespace paymentchannel {
    class Payee;
}
namespace protocol_session {

// NB: In the future one can separete into two different callbacks,
// one for each mode, with its own set of DisconnectCauses.
enum class DisconnectCause {

    // session stops, removing connection
    client,

    //// buying

    seller_has_interrupted_contract,

    seller_servicing_piece_has_timed_out,

    seller_sent_invalid_piece,

    //// selling

    buyer_invited_with_bad_terms,

    buyer_requested_invalid_piece,

    buyer_interrupted_payment,

    buyer_sent_invalid_payment
};

// Removal of a connection from the session: c++11 alias declaration
template <class ConnectionIdType>
using RemovedConnectionCallbackHandler = std::function<void(const ConnectionIdType &, DisconnectCause)>;

// Generate set of key pairs (payment channel)
typedef std::function<std::vector<Coin::KeyPair>(int, const joystream::bitcoin::RedeemScriptGenerator&)> GenerateP2SHKeyPairsCallbackHandler;

// Generate set of receive p2pkh addresses
typedef std::function<std::vector<Coin::P2PKHAddress>(int)> GenerateReceiveAddressesCallbackHandler;

// Generate set of change p2pkh addresses
typedef std::function<std::vector<Coin::P2PKHAddress>(int)> GenerateChangeAddressesCallbackHandler;

// Send a message to be sent
typedef std::function<void(const protocol_wire::ExtendedMessagePayload *)> SendMessageOnConnection;

//// Buying

// Broadcasting a transaction
typedef std::function<bool(const Coin::Transaction &)> BroadcastTransaction;

// Process arrival of a full piece, with given index over peer connection with given id
template <class ConnectionIdType>
using FullPieceArrived = std::function<void(const ConnectionIdType &, const protocol_wire::PieceData &, int)>;

//// Selling

// Notification that buyer with given connection ids has one outstanding payment
//template <class ConnectionIdType>
//using HasOutstandingPayment = std::function<void(const ConnectionIdType &)>;

// Buyer with given connection id requires piece with given index
template <class ConnectionIdType>
using LoadPieceForBuyer = std::function<void(const ConnectionIdType &, unsigned int)>;

// Buyer with given connection id caused invitedToOutdatedContract event
//template <class ConnectionIdType>
//using StaleContractInvitation = std::function<void(const ConnectionIdType &)>;

// Buyer with given connection id requires piece with given index
template <class ConnectionIdType>
using ClaimLastPayment = std::function<void(const ConnectionIdType &, const joystream::paymentchannel::Payee &)>;

// Buyer with given connection id announced anchor
template <class ConnectionIdType>
using AnchorAnnounced = std::function<void(const ConnectionIdType &, quint64, const Coin::typesafeOutPoint &, const Coin::PublicKey &, const Coin::PubKeyHash &)>;

}
}

#endif // JOYSTREAM_PROTOCOLSESSION_CALLBACKS_HPP

