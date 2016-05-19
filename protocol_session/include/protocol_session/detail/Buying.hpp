/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_BUYING_HPP
#define JOYSTREAM_PROTOCOLSESSION_BUYING_HPP

#include <protocol_session/Session.hpp>
#include <protocol_session/BuyingPolicy.hpp>
#include <protocol_session/BuyingState.hpp>
#include <protocol_session/detail/Piece.hpp>
#include <protocol_session/detail/Seller.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <CoinCore/CoinNodeData.h>

#include <vector>

namespace joystream {
namespace protocol_statemachine {
    class AnnouncedModeAndTerms;
}
namespace protocol_session {

class TorrentPieceInformation;

namespace detail {

template <class ConnectionIdType>
class Selling;

template <class ConnectionIdType>
class Observing;

template <class ConnectionIdType>
class Buying {

public:

    Buying(Session<ConnectionIdType> *,
           const RemovedConnectionCallbackHandler<ConnectionIdType> &,
           const GenerateKeyPairsCallbackHandler &,
           const GenerateP2PKHAddressesCallbackHandler &,
           const BroadcastTransaction &,
           const FullPieceArrived<ConnectionIdType> &,
           const Coin::UnspentP2PKHOutput &,
           const BuyingPolicy &,
           const protocol_wire::BuyerTerms &,
           const TorrentPieceInformation &);

    //// Connection level client events

    // Adds connection, and return the current number of connections
    uint addConnection(const ConnectionIdType &, const SendMessageOnConnection &);

    // Remove connection
    void removeConnection(const ConnectionIdType &);

    // A valid piece was sent too us on given connection
    void validPieceReceivedOnConnection(const ConnectionIdType &, int index);

    // An invalid piece was sent too us on given connection
    // Should not be called when session is stopped.
    void invalidPieceReceivedOnConnection(const ConnectionIdType &, int index);

    //// Connection level state machine events

    void peerAnnouncedModeAndTerms(const ConnectionIdType &, const protocol_statemachine::AnnouncedModeAndTerms &);
    void sellerHasJoined(const ConnectionIdType &);
    void sellerHasInterruptedContract(const ConnectionIdType &);
    void receivedFullPiece(const ConnectionIdType &, const protocol_wire::PieceData &);

    //// Change mode

    void leavingState();

    //// Change state

    // Starts a stopped session by becoming fully operational
    void start();

    // Immediately closes all existing connections
    void stop();

    // Pause session
    // Accepts new connections, but only advertises mode.
    // All existing connections are gracefully paused so that all
    // incoming messages can be ignored. In particular it
    // honors last pending payment, but issues no new piece requests.
    void pause();

    //// Miscellenous

    // Time out processing hook
    // NB: Later give some indication of how to set timescale for this call
    void tick();

    // Piece with given index has been downloaded, but not through
    // a regitered connection. Could be non-joystream peers, or something out of bounds.
    void pieceDownloaded(int);

    // Update terms
    void updateTerms(const protocol_wire::BuyerTerms &);

    //// Getters and setters

    Coin::UnspentP2PKHOutput funding() const;

    BuyingPolicy policy() const;
    void setPolicy(const BuyingPolicy & policy);

    protocol_wire::BuyerTerms terms() const;

private:

    //// Routines for initiation contract

    // Tries start downloading if possible
    bool tryToStartDownloading();

    // Pick an appropriate subset of connections as sellers
    std::vector<detail::Connection<ConnectionIdType> *> selectSellers() const;

    // Decide a distribution of funds among given sellers
    std::vector<uint64_t> distributeFunds(const std::vector<protocol_wire::SellerTerms> &) const;

    // Determine if there should be a change output, and if so, how much funds it should have
    uint64_t determineChangeAmount(uint32_t numberOfSellers, uint64_t totalComitted, uint64_t contractFeePerKb) const;

    //// Assigning pieces

    // Tries to assign an unassigned piece to given seller
    bool tryToAssignAndRequestPiece(detail::Seller<ConnectionIdType> &);

    // Tries to find next unassigned piece
    int getNextUnassignedPiece() const;

    // Removes given seller
    void removeSeller(detail::Seller<ConnectionIdType> &);

    //// Utility routines

    void politeSellerCompensation();

    // Unguarded
    void _start();

    //// Members

    // Reference to core of session
    Session<ConnectionIdType> * _session;

    // Callback handlers
    RemovedConnectionCallbackHandler<ConnectionIdType> _removedConnection;
    GenerateKeyPairsCallbackHandler _generateKeyPairs;
    GenerateP2PKHAddressesCallbackHandler _generateP2PKHAddresses;
    BroadcastTransaction _broadcastTransaction;
    FullPieceArrived<ConnectionIdType> _fullPieceArrived;

    // Funding for buyer
    Coin::UnspentP2PKHOutput _funding;

    // Controls behaviour of session
    BuyingPolicy _policy;

    // State
    BuyingState _state;

    // Terms for buying
    protocol_wire::BuyerTerms _terms;

    // Maps connection identifier to connection
    std::map<ConnectionIdType, detail::Seller<ConnectionIdType>> _sellers;

    // Contract transaction id
    // NB** Must be stored, as signatures are non-deterministic
    // contributions to the TxId, and hence discarding them
    // ***When segwit is enforced, this will no longer be neccessary.***
    Coin::Transaction _contractTx;

    // Pieces in torrent file
    std::vector<detail::Piece<ConnectionIdType>> _pieces;

    // The number of pieces not yet downloaded.
    // Is used to detect when we are done.
    uint32_t _numberOfMissingPieces;

    // Indexes of pieces which were assigned, but then later
    // unassigned for some reason (e.g. seller left, timed out, etc).
    // They are prioritized when assiging new pieces.
    // NB: using std::deque over std::queue, since latter
    std::deque<uint32_t> _deAssignedPieces;

    // Index before which we should never assign a piece unless all pieces
    // with a greater index have been assigned. Following this constraint
    // results in in-order downloading of pieces, e.g. for media streaming.
    // Will typically also be reset of client desires to set streaming of media to
    // some midway point
    uint32_t _assignmentLowerBound;

    // When we started sending out invitations
    // (i.e. entered state StartedState::sending_invitations).
    // Is used to figure out when to start trying to build the contract
    time_t _lastStartOfSendingInvitations;
};

}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/detail/Buying.cpp>

#endif // JOYSTREAM_PROTOCOLSESSION_BUYING_HPP
