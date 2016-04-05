/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_HPP

#include <common/P2PKHAddress.hpp>
#include <common/Network.hpp>
#include <protocol/Mode.hpp>

#include <functional>
#include <map>
#include <vector>

namespace Coin {
    class KeyPair;
    class P2PKHAddress;
}

namespace joystream {
namespace wire {
    class ExtendedMessagePayload;
}

namespace protocol {

    class Connection;

    template <class T> // T is type of connection
    class Session {

    public:

        // Callback for handling the removal of a connection from the session
        typedef std::function<void(const std::string &)> RemovedConnectionCallbackHandler;
        typedef std::function<std::vector<Coin::KeyPair>(int)> GenerateKeyPairsCallbackHandler;
        typedef std::function<std::vector<Coin::P2PKHAddress>(int)> GenerateP2PKHAddressesCallbackHandler;

        // Callback for handling broadcasting a transaction
        //typedef std::function<bool(const Coin::Transaction &)> BroadCastTransactionCallbackHandler;

        // Callback for generating a key pair
        //typedef std::function< generate coin::keypair?

        // Callback for generating a receive address
        //typedef std::function generate address

        // Whether there is a connection with given id
        bool hasConnection(const std::string & name) const;

        // Remove connection: does not result in correspondnig callback
        virtual bool removeConnection(const std::string & name);

        // Process extended message: does not take ownership of message
        virtual void processMessageOnConnection(const std::string & name, const joystream::wire::ExtendedMessagePayload & message) = 0;

        // Perform non-reactive (that is not in response to message or control event) processing
        virtual void tick();

        // Getters
        Mode mode() const;

        Coin::Network network() const;

        std::map<std::string, T> connections() const;

        RemovedConnectionCallbackHandler removedConnectionCallbackHandler() const;

        GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler() const;

        GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler() const;

    protected:

        Session(Mode mode,
                Coin::Network network,
                const std::map<std::string, T> & connections,
                const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler);
        // Mode
        Mode _mode;

        // Network
        Coin::Network _network;

        // Connections
        std::map<std::string, T> _connections;

        // Callback for when connection has been removed from session
        RemovedConnectionCallbackHandler _removedConnectionCallbackHandler;

        // Callback for when key pairs have to be generated
        GenerateKeyPairsCallbackHandler _generateKeyPairsCallbackHandler;

        // Callback for when addresses have to be generated
        GenerateP2PKHAddressesCallbackHandler _generateP2PKHAddressesCallbackHandler;
    };

}
}

#include <protocol/../../src/Session.cpp>

#endif // JOYSTREAM_PROTOCOL_SESSION_HPPs
