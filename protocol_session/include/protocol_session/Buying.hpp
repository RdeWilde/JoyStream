/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_BUYING_HPP
#define JOYSTREAM_PROTOCOLSESSION_BUYING_HPP

#include <protocol_session/detail/SessionCoreImpl.hpp>
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

template <class ConnectionIdType>
class Selling;

class TorrentPieceInformation;

template <class ConnectionIdType>
class Buying {

public:

    struct State {

        enum class ClientState {

            // Closes all connections, and rejects all new connections.
            stopped,

            // Going through the motions.
            started,

            // Stops building contract, or requesting new pieces
            // if contract has been announced. In the latter scenario
            // the last payment is honored. New connections are
            // accepted, but only mode is announced.
            paused
        };

        enum class StartedState {

            // Inviting sellers
            sending_invitations,

            // Requesting and downloading pieces
            downloading,

            // Have all pieces
            download_completed
        };

        State()
            : _clientState(ClientState::stopped)
            , _startedState(StartedState::sending_invitations){
        }

        ClientState _clientState;

        StartedState _startedState;
    };

    class Policy {

    public:

        Policy()
            : _minTimeBeforeBuildingContract(0) {
        }

        Policy(double minTimeBeforeBuildingContract)
            : _minTimeBeforeBuildingContract(minTimeBeforeBuildingContract) {
        }

        // Take out of policy, should not be publicly visible
        bool okToBuildContract(const time_t started) const {

            // Get current time
            time_t now = time(0);

            // Return whether sufficient time has passed since start
            return difftime(now, started) >= _minTimeBeforeBuildingContract;
        }

    private:

        // 1
        // The minimum amount of time (s) required before
        // trying to buil a contract
        double _minTimeBeforeBuildingContract;

        // 2
        // time to wait, at very least,

        // 3
        // How long to wait before trying to ask someone else for the same piece
        // if a peer is not responding, or responding too slowly

        // 4
        // Ranking peers for invite:
        // if there are more peers than your desired max,
        // do you pick random subset, or minimize on price.

        // 5
        // Ranking peers for downloading

        // 6
        // Ranking piece for download order

        // 7
        //
        double _servicingPieceTimeOutLimit;

    };

    Buying(const RemovedConnectionCallbackHandler<ConnectionIdType> &,
           const GenerateKeyPairsCallbackHandler &,
           const GenerateP2PKHAddressesCallbackHandler &,
           const BroadcastTransaction &,
           const FullPieceArrived<ConnectionIdType> &,
           const Coin::UnspentP2PKHOutput &,
           const Policy &,
           const protocol_wire::BuyerTerms &,
           const TorrentPieceInformation &);

    //// Manage connections

    // Connection with givne id has been added (ex-post)
    uint addConnection(const ConnectionIdType &, const SendMessageOnConnection &);

    // Connection with given id has been removed (ex-post)
    void removeConnection(const ConnectionIdType &);

    // Process given message on given connection with given ID
    void processMessageOnConnection(const ConnectionIdType &, const protocol_wire::ExtendedMessagePayload &);

    // A valid piece was sent too us on given connection
    void validPieceReceivedOnConnection(const ConnectionIdType &, int index);

    // An invalid piece was sent too us on given connection
    // NB: Perhaps we should supply connection?
    void invalidPieceReceivedOnConnection(const ConnectionIdType &, int index);

    //// Manage mode

    // Update terms
    void updateTerms(const protocol_wire::BuyerTerms &);

    void toObserveMode();

    //
    // Caller owns returned object.
    Selling<ConnectionIdType> * toSellMode();

    //// Manage state

    // Starts a stopped session by becoming fully operational
    void start();

    // Immediately closes all existing connections
    void stop();

    // Accepts new connections, but only advertises mode.
    // All existing connections are gracefully paused so that all
    // incoming messages can be ignored. In particular

    // * Buying mode: stops creating new contracts, or for started contracts it
    // only honors last pending payment, but issues no new piece requests.

    void pause();

    //// Miscellenous

    // Time out processing hook
    // NB: Later give some indication of how to set timescale for this call
    void tick();

    // Piece with given index has been downloaded, but not through
    // a regitered connection. Could be non-joystream peers, or something out of bounds.
    void pieceDownloaded(int);

    //// Getters and setters

    Coin::UnspentP2PKHOutput getFunding() const;
    void setFunding(const Coin::UnspentP2PKHOutput &funding);

    Policy getPolicy() const;
    void setPolicy(const Policy &policy);

private:

    // Buying mode spesific callbacks

    BroadcastTransaction _broadcastTransaction;
    FullPieceArrived<ConnectionIdType> _fullPieceArrived;


    void setHooks(detail::Connection<ConnectionIdType> &);

    //// Handling callbacks from connections

    void peerAnnouncedModeAndTerms(const ConnectionIdType &, const protocol_statemachine::AnnouncedModeAndTerms &);
    void invitedToOutdatedContract(const ConnectionIdType &);
    void invitedToJoinContract(const ConnectionIdType &);
    void contractPrepared(const ConnectionIdType &, const Coin::typesafeOutPoint &);
    void pieceRequested(const ConnectionIdType & id, int i);
    void invalidPieceRequested(const ConnectionIdType & id);
    void paymentInterrupted(const ConnectionIdType & id);
    void receivedValidPayment(const ConnectionIdType & id, const Coin::Signature &);
    void receivedInvalidPayment(const ConnectionIdType & id, const Coin::Signature &);
    void sellerHasJoined(const ConnectionIdType &);
    void sellerHasInterruptedContract(const ConnectionIdType &);
    void receivedFullPiece(const ConnectionIdType &, const protocol_wire::PieceData &);

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

    //
    bool tryToAssignAndRequestPiece(detail::Seller<ConnectionIdType> & s);

    // ?
    int getNextUnassignedPiece() const;

    ////

    // Unassigns any seller which is too slow
    uint unAssignPiecesFromSlowSellers();

    //// Members

    // Reference to core of session
    detail::SessionCoreImpl<ConnectionIdType> _sessionCore;

    // Funding for buyer
    Coin::UnspentP2PKHOutput _funding;

    // Controls behaviour of session
    Policy _policy;

    // State
    State _state;

    // Terms for buying
    protocol_wire::BuyerTerms _terms;

    // Maps connection identifier to connection
    std::map<ConnectionIdType, detail::Seller<ConnectionIdType>> _sellers;

    // Contract transaction id
    // NB** Must be stored, as signatures are non-deterministic
    // contributions to the TxId, and hence discarding them
    // When segwit is enforced, this will no longer be neccessary.
    Coin::Transaction _contractTx;

    // Pieces in torrent file: Should really be array?
    std::vector<detail::Piece<ConnectionIdType>> _pieces;

    // Indexes of pieces which were assigned this session, but were
    // then unassigned for some reason. They are prioritized when assiging
    // new pieces
    std::queue<uint32_t> _deAssignedPieces;

    // Index before which we should never assign a piece unless all pieces
    // with a greater index have been assigned. Following this constraint
    // results in in-order downloading of pieces, e.g. for media streaming.
    // Will typically also be reset of client desires to set streaming of media to
    // some midway point
    uint32_t _assignmentLowerBound;

    /**
    // When session was started
    time_t _lastStart;
    */

    time_t _lastStartOfSendingInvitations;
};

}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/Buying.cpp>

#endif // JOYSTREAM_PROTOCOLSESSION_BUYING_HPP
