/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#include <protocol/Session.hpp>
#include <protocol/wire/ExtendedMessagePayload.hpp>

namespace joystream {
namespace protocol {

    /**
    Session::Session(Mode mode, uint32_t numberOfPiecesInTorrent, const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler)
        : _mode(mode)
        , _numberOfPiecesInTorrent(numberOfPiecesInTorrent)
        , _removedConnectionCallbackHandler(removedConnectionCallbackHandler) {
    }

    void Session::setSellTerms(const sell::Terms & sellTerms) {
        _sellTerms = sellTerms;
    }

    void Session::setBuyTerms(const buy::Terms & buyTerms) {
        _buyTerms = buyTerms;
    }
    */

    Session::Session(Mode mode,
            uint32_t numberOfPiecesInTorrent,
            const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
            const std::map<std::string, Connection> & connections,
            const buy::SessionState & buyModeState,
            const buy::Terms & buyTerms,
            const joystream::paymentchannel::Payor & payor,
            const std::vector<Piece> & pieces,
            const sell::Terms & sellTerms)
        : _mode(mode)
        , _numberOfPiecesInTorrent(numberOfPiecesInTorrent)
        , _removedConnectionCallbackHandler(removedConnectionCallbackHandler)
        , _connections(connections)
        , _buyTerms(buyTerms)
        , _payor(payor)
        , _pieces(pieces)
        , _sellTerms(sellTerms) {
    }

    Session Session::buyer(uint32_t numberOfPiecesInTorrent, const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler, buy::Terms terms) {

        return Session(Mode::buy, numberOfPiecesInTorrent, removedConnectionCallbackHandler);
    }

    Session Session::seller(uint32_t numberOfPiecesInTorrent, const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler, sell::Terms terms) {

        return Session(Mode::sell, numberOfPiecesInTorrent, removedConnectionCallbackHandler);
    }

    Session Session::observer(uint32_t numberOfPiecesInTorrent, const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler) {

        return Session(Mode::observe, numberOfPiecesInTorrent, removedConnectionCallbackHandler);
    }

    void Session::addConnection(const std::string & name, const Connection::SendMessageCallbackHandler & connectionSendCallback) {

    }

    void Session::removeConnection(const std::string & name) {

    }

    void Session::processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload * message) {

    }

    void Session::switchToBuyMode(const buy::Terms & terms) {
        throw std::runtime_error("implement later");
    }

    void Session::switchToSellMode(const sell::Terms & terms) {
        throw std::runtime_error("implement later");
    }

    void Session::switchToObserveMode() {
        throw std::runtime_error("implement later");
    }

    void Session::updateTerms(const buy::Terms & terms) {

        // throw exception if we are not actually in buy mode
        // throw std::runtime_error

        throw std::runtime_error("implement later");
    }

    void Session::updateTerms(const sell::Terms & terms) {

        // throw exception if we are not actually in sell mode
        // throw std::runtime_error

        throw std::runtime_error("implement later");
    }

    void Session::markPieceAsDownloadedAndValid(int index) {

        //
    }

}
}
