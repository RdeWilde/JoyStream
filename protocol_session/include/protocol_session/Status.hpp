/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 21 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_STATUS
#define JOYSTREAM_PROTOCOLSESSION_STATUS

#include <protocol_session/PieceState.hpp>
#include <protocol_session/SellerState.hpp>
#include <protocol_session/BuyingPolicy.hpp>
#include <protocol_session/BuyingState.hpp>
#include <protocol_session/SellingPolicy.hpp>
#include <protocol_session/SessionMode.hpp>
#include <protocol_session/SessionState.hpp>
#include <protocol_statemachine/protocol_statemachine.hpp>

#include <CoinCore/CoinNodeData.h> // Coin::Transaction

#include <queue>

namespace joystream {
namespace protocol_session {
namespace status {

    class CBStateMachine {

    public:

        CBStateMachine() {}

        CBStateMachine(const protocol_statemachine::AnnouncedModeAndTerms & announcedModeAndTermsFromPeer,
                       const paymentchannel::Payor & payor,
                       const paymentchannel::Payee & payee)
            : _announcedModeAndTermsFromPeer(announcedModeAndTermsFromPeer)
            , _payor(payor)
            , _payee(payee) {
        }

        protocol_statemachine::AnnouncedModeAndTerms announcedModeAndTermsFromPeer() const { return _announcedModeAndTermsFromPeer; }

        paymentchannel::Payor payor() const { return _payor; }

        paymentchannel::Payee payee() const { return _payee; }

    private:

        //// Peer state

        protocol_statemachine::AnnouncedModeAndTerms _announcedModeAndTermsFromPeer;

        //// Buyer Client state

        // Payor side of payment channel interaction
        paymentchannel::Payor _payor;

        //// Seller Client state

        // Payee side of payment channel interaction
        paymentchannel::Payee _payee;

    };

    template <class ConnectionIdType>
    class Connection {

    public:

        Connection() {}

        Connection(const ConnectionIdType & connectionId,
                   const CBStateMachine & machine,
                   const std::queue<uint32_t> & downloadedValidPieces)
            : _connectionId(connectionId)
            , _machine(machine)
            , _downloadedValidPieces(downloadedValidPieces) {
        }

        ConnectionIdType connectionId() const { return _connectionId; }

        CBStateMachine machine() const { return _machine; }

        std::queue<uint32_t> downloadedValidPieces() const { return _downloadedValidPieces; }

    private:

        // Connection id
        ConnectionIdType _connectionId;

        // State machine for this connection
        CBStateMachine _machine;

        //// Buyer

        // Indexes of valid piecesm, in the order they were downloaded
        // NB: The reason this is not in Seller, is because
        // any peer can potentially provide valid pieces
        std::queue<uint32_t> _downloadedValidPieces;

        //// Selling
    };

    template <class ConnectionIdType>
    class Piece {

    public:

        Piece() {}

        Piece(int index, PieceState state, const ConnectionIdType & connectionId, unsigned int size)
            : _index(index)
            , _state(state)
            , _connectionId(connectionId)
            , _size(size) {}

        int index() const { return _index; }

        PieceState state() const { return _state; }

        ConnectionIdType connectionId() const { return _connectionId; }

        unsigned int size() const { return _size; }

    private:

        // Index of piece
        int _index;

        // Piece state
        PieceState _state;

        // Id of connectionto which piece is assigned, when _state == State::assigned_to_peer_for_download
        ConnectionIdType _connectionId;

        // Byte length of piece (should be the same for all but last piece)
        unsigned int _size;
    };

    template <class ConnectionIdType>
    class Seller {

        Seller() {}

        Seller(SellerState state, ConnectionIdType connection)
            : _state(state)
            , _connection(connection)
        {}

        SellerState state() const { return _state; }

        ConnectionIdType connection() const { return _connection; }

    private:

        // State of this seller
        SellerState _state;

        // Connection identifier for seller
        ConnectionIdType _connection;
    };

    template <class ConnectionIdType>
    class Buying {

    public:

        Buying() {}

        Buying(const Coin::UnspentP2PKHOutput & funding,
               const BuyingPolicy & policy,
               const BuyingState state,
               const protocol_wire::BuyerTerms & terms,
               const std::map<ConnectionIdType, Seller<ConnectionIdType>> & sellers,
               const Coin::Transaction & contractTx,
               const std::vector<Piece<ConnectionIdType>> & pieces)
            : _funding(funding)
            , _policy(policy)
            , _terms(terms)
            , _sellers(sellers)
            , _contractTx(contractTx)
            , _pieces(pieces) {}

        Coin::UnspentP2PKHOutput funding() const { return _funding;}

        BuyingPolicy policy() const { return _policy; }

        BuyingState state() const { return _state; }

        protocol_wire::BuyerTerms terms() const { return _terms; }

        std::map<ConnectionIdType, Seller<ConnectionIdType>> sellers() const { return _sellers; }

        Coin::Transaction contractTx() const { return _contractTx; }

        std::vector<Piece<ConnectionIdType>> pieces() const {return _pieces;}

    private:

        // Funding for buyer
        Coin::UnspentP2PKHOutput _funding;

        // Controls behaviour of session
        BuyingPolicy _policy;

        // State
        BuyingState _state;

        // Terms for buying
        protocol_wire::BuyerTerms _terms;

        // Maps connection identifier to connection
        std::map<ConnectionIdType, Seller<ConnectionIdType>> _sellers;

        // Contract transaction id
        Coin::Transaction _contractTx;

        // Pieces in torrent file
        std::vector<Piece<ConnectionIdType>> _pieces;
    };

    class Selling {

    public:

        Selling() {}

        Selling(const SellingPolicy & policy,const protocol_wire::SellerTerms & terms)
            : _policy(policy)
            , _terms(terms) {}

        SellingPolicy policy() const { return _policy; }
        protocol_wire::SellerTerms terms() const { return _terms; }

    private:

        // Controls behaviour of session
        SellingPolicy _policy;

        // Terms for selling
        protocol_wire::SellerTerms _terms;

    };

    template <class ConnectionIdType>
    class Session {

        Session() {}

        Session(SessionMode mode, SessionState state, const std::map<ConnectionIdType, Connection<ConnectionIdType>> & connections, const Selling & selling, const Buying<ConnectionIdType>  & buying)
            : _mode(mode)
            , _state(state)
            , _connections(connections)
            , _selling(selling)
            , _buying(buying) {
        }

        SessionMode mode() const { return _mode; }

        SessionState state() const { return _state; }

        std::map<ConnectionIdType, Connection<ConnectionIdType> > connections() const { return _connections; }

        Selling selling() const { return _selling; }

        Buying<ConnectionIdType> buying() const { return _buying; }

    public:

        // Session mode
        SessionMode _mode;

        // Current state of session
        SessionState _state;

        // Connections
        std::map<ConnectionIdType, Connection<ConnectionIdType>> _connections;

        //// Substates

        // Seller
        Selling _selling;

        // Buyer
        Buying<ConnectionIdType> _buying;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSESSION_STATUS
