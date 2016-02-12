/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_HPP

#include <common/KeyPair.hpp>
#include <common/P2PKHAddress.hpp>

#include <protocol/Mode.hpp>
#include <functional>
#include <vector>

namespace joystream {
namespace protocol {

    namespace wire {
        class ExtendedMessagePayload;
    }

    class Connection;

    class Session {

    public:

        // Callback for handling the removal of a connection from the session
        typedef std::function<void(const std::string &)> RemovedConnectionCallbackHandler;
        typedef std::function<std::vector<Coin::KeyPair>(uint)> GenerateKeyPairsCallbackHandler;
        typedef std::function<std::vector<Coin::P2PKHAddress>(uint)> GenerateP2PKHAddressesCallbackHandler;

        // Callback for handling broadcasting a transaction
        //typedef std::function<bool(const Coin::Transaction &)> BroadCastTransactionCallbackHandler;

        // Callback for generating a key pair
        //typedef std::function< generate coin::keypair?

        // Callback for generating a receive address
        //typedef std::function generate address

        // Add connection with peer where only extended handshake has been sent
        virtual void addConnection(const Connection & connection) = 0;

        // Remove connection: does not result in correspondnig callback
        virtual void removeConnection(const std::string & name) = 0;

        // Process extended message: does not take ownership of message
        virtual void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message) = 0;

        // Getters
        Mode mode() const;

    protected:

        Session(Mode mode,
                const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler);

        // Mode
        Mode _mode;

        // Callback for when connection has been removed from session
        RemovedConnectionCallbackHandler _removedConnectionCallbackHandler;

        // Callback for when key pairs have to be generated
        GenerateKeyPairsCallbackHandler _generateKeyPairsCallbackHandler;

        // Callback for when addresses have to be generated
        GenerateP2PKHAddressesCallbackHandler _generateP2PKHAddressesCallbackHandler;
    };
}
}

#endif // JOYSTREAM_PROTOCOL_SESSION_HPPs
