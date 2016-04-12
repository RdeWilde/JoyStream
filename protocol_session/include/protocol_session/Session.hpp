/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_SESSION_HPP
#define JOYSTREAM_PROTOCOLSESSION_SESSION_HPP

#include <protocol_session/SessionMode.hpp>
#include <protocol_session/Connection.hpp>
#include <protocol_session/Buying.hpp>
#include <protocol_session/Selling.hpp>

namespace joystream {
namespace protocol_wire {
    class ExtendedMessagePayload;
}
namespace protocol_session {

    // NOTE:
    // ConnectionIdType: type for identifying connections, must
    // be possible to use as key in std::map, and also have
    // std::string ConnectionIdType::toString() const

    //// Callback types

    // Callback for handling the removal of a connection from the session: c++11 alias declaration
    template <class ConnectionIdType>
    using RemovedConnectionCallbackHandler = std::function<void(const ConnectionIdType &)>;

    typedef std::function<std::vector<Coin::KeyPair>(int)> GenerateKeyPairsCallbackHandler;
    typedef std::function<std::vector<Coin::P2PKHAddress>(int)> GenerateP2PKHAddressesCallbackHandler;

    // Client requires a message to be sent
    typedef std::function<void(const protocol_wire::ExtendedMessagePayload *)> SendMessageOnConnection;

    // Callback for handling broadcasting a transaction
    //typedef std::function<bool(const Coin::Transaction &)> BroadCastTransactionCallbackHandler;

    // Callback for generating a key pair
    //typedef std::function< generate coin::keypair?

    // Callback for generating a receive address
    //typedef std::function generate address

    //// Session core implementation

    namespace detail {

        template <class ConnectionIdType>
        struct SessionCoreImpl {

            SessionCoreImpl(RemovedConnectionCallbackHandler<ConnectionIdType> removedConnectionCallbackHandler,
                            GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler,
                            GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler)
                : _mode(SessionMode::NotSet)
                , _removedConnectionCallbackHandler(removedConnectionCallbackHandler)
                , _generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
                , _generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler) {

                // Note starting time
                time(&_started);
            }

            uint addConnection(const Connection<ConnectionIdType> * c) {

                // Insert in connectin map and return number of connections
                _connections[c->_connectionId] = c;

                return _connections.size();
            }

            bool hasConnection(const ConnectionIdType & id) const {
                return _connections.find(id) != _connections.cend();
            }

            // Mode of session
            SessionMode _mode;

            // Connections
            std::map<ConnectionIdType, Connection<ConnectionIdType> *> _connections;

            // Callback for when connection has been removed from session
            RemovedConnectionCallbackHandler<ConnectionIdType> _removedConnectionCallbackHandler;

            // Callback for when key pairs have to be generated
            GenerateKeyPairsCallbackHandler _generateKeyPairsCallbackHandler;

            // Callback for when addresses have to be generated
            GenerateP2PKHAddressesCallbackHandler _generateP2PKHAddressesCallbackHandler;

            // When session was started
            time_t _started;
        };

    }

    template <class ConnectionIdType>
    class Session {

    public:

        // Construct session
        Session(const RemovedConnectionCallbackHandler<ConnectionIdType> &,
                const GenerateKeyPairsCallbackHandler &,
                const GenerateP2PKHAddressesCallbackHandler &);

        // Time out processing hook
        // NB: Later give some indication of how to set timescale for this call
        void tick();

        //// Manage mode

        void toObserve();
        void toSellMode();
        void toBuyMode();

        //// Manage connections

        // Adds connection, and return the current number of connections
        uint addConnection(const ConnectionIdType &, const SendMessageOnConnection &);

        // Whether there is a connection with given id
        bool hasConnection(const ConnectionIdType &) const;

        // Remove connection if one exists with given id, otherwise returns false.
        // NB:does not result in correspondnig callback ??!?!
        bool removeConnection(const ConnectionIdType &);

        // Either we paid for it, or it just came in.
        //void markPieceAsDownloadedAndValid(int index);

        void processMessageOnConnection(const ConnectionIdType &, const protocol_wire::ExtendedMessagePayload *);

        ////

        //static quint64 minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB, quint64 paychanSettlementFee);

        SessionMode mode() const;

    private:

        // Session core
        detail::SessionCoreImpl<ConnectionIdType> _core;

        // Observer

        // Seller
        Selling<ConnectionIdType> _selling;

        // Buyer
        Buying<ConnectionIdType> _buying;
    };

}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/Session.cpp>

#endif // JOYSTREAM_PROTOCOLSESSION_SESSION_HPP

