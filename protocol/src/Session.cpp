/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

//#include <protocol/Session.hpp>
#include <protocol/Connection.hpp>

namespace joystream {
namespace protocol {

    template <class T>
    Session<T>::Session(Mode mode,
                        Coin::Network network,
                        const std::map<std::string, T> & connections,
                        const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                        const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                        const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler)
        : _mode(mode)
        , _network(network)
        , _connections(connections)
        , _removedConnectionCallbackHandler(removedConnectionCallbackHandler)
        , _generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
        , _generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler) {
    }

    template<class T>
    bool Session<T>::hasConnection(const std::string & name) const {
        return _connections.find(name) != _connections.cend();
    }

    template<class T>
    bool Session<T>::removeConnection(const std::string & name) {

        // Number of connections prior to erase
        typename std::map<std::string, T>::size_type size = _connections.size();

        // Removal was successfull iff size decreased
        return size < _connections.erase(name);
    }

    template <class T>
    void Session<T>::tick() {
    }

    template <class T>
    Mode Session<T>::mode() const {
        return _mode;
    }

    template <class T>
    Coin::Network Session<T>::network() const {
        return _network;
    }

    template <class T>
    std::map<std::string, T> Session<T>::connections() const {
        return _connections;
    }

    template <class T>
    RemovedConnectionCallbackHandler Session<T>::removedConnectionCallbackHandler() const {
        return _removedConnectionCallbackHandler;
    }

    template <class T>
    GenerateKeyPairsCallbackHandler Session<T>::generateKeyPairsCallbackHandler() const {
        return _generateKeyPairsCallbackHandler;
    }

    template <class T>
    GenerateP2PKHAddressesCallbackHandler Session<T>::generateP2PKHAddressesCallbackHandler() const {
        return _generateP2PKHAddressesCallbackHandler;
    }
}
}
