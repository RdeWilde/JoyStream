/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 7 2016
 */

#ifndef JOYSTREAM_EXTENSION_REQUEST_HPP
#define JOYSTREAM_EXTENSION_REQUEST_HPP

#include <protocol_wire/protocol_wire.hpp>
#include <protocol_session/protocol_session.hpp>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/socket.hpp>

#include <exception>
#include <functional>

namespace joystream {
namespace extension {
namespace request {

class MissingTorrent : public std::runtime_error {

public:

    MissingTorrent()
        : std::runtime_error("Torrent missing") {
    }

};

/**
 * All request classes must have at least two types
 * defined in class scope
 * ::Result: Represents result of a request, must have default and cpy-constructor.
 * ::ResultHandler: Represents a callback which handles a ::Result instance, must be of type support
 * operator(const ::Result &) const, e.g. std::function<void(const Result &)>
 */

// T = request in this namespace
template <class T>
class SubroutineResult {

public:

    SubroutineResult() {
    }

    SubroutineResult(const std::exception_ptr & exception)
        : exception(exception) {
    }

    void throwSetException() const {

        if(exception)
            std::rethrow_exception(exception);
    }

private:

    // Exception result
    std::exception_ptr exception;
};

// T = request in this namespace
// T2 = result from request, must be (copy+default)constructible
template <class T, class T2>
class MethodResult : public SubroutineResult<T> {

public:

    MethodResult() {
    }

    MethodResult(const T & request,
                 const std::exception_ptr exception,
                 const T2 & result)
        : SubroutineResult<T>(request, exception)
        , _result(result) {
    }

    void throwSetException() const {

        SubroutineResult<T>::throwSetException();
    }

    T2 result() const {
        return _result;
    }

private:

    // Method result
    T2 _result;
};

struct TorrentPluginRequest {

    TorrentPluginRequest() {}
    TorrentPluginRequest(const libtorrent::sha1_hash & infoHash)
        : infoHash(infoHash) {
    }

    virtual ~TorrentPluginRequest() { }

    libtorrent::sha1_hash infoHash;
};

struct Start : public TorrentPluginRequest {

    typedef SubroutineResult<Start> Result;
    typedef std::function<void(const Result &)> ResultHandler;

    Start() {}
    Start(const libtorrent::sha1_hash & infoHash,
          const ResultHandler & handler = ResultHandler())
        : TorrentPluginRequest(infoHash)
        , handler(handler) {
    }

    ResultHandler handler;
};

struct Stop : public TorrentPluginRequest {

    typedef SubroutineResult<Stop> Result;
    typedef std::function<void(const Result &)> ResultHandler;

    Stop() {}
    Stop(const libtorrent::sha1_hash & infoHash,
         const ResultHandler & handler = ResultHandler())
        : TorrentPluginRequest(infoHash)
        , handler(handler){
    }

    ResultHandler handler;
};

struct Pause : public TorrentPluginRequest {

    typedef SubroutineResult<Pause> Result;
    typedef std::function<void(const Result &)> ResultHandler;

    Pause() {}
    Pause(const libtorrent::sha1_hash & infoHash,
          const ResultHandler & handler = ResultHandler())
        : TorrentPluginRequest(infoHash)
        , handler(handler) {
    }

    ResultHandler handler;
};

struct UpdateBuyerTerms : public TorrentPluginRequest {

    typedef SubroutineResult<UpdateBuyerTerms> Result;
    typedef std::function<void(const Result &)> ResultHandler;

    UpdateBuyerTerms() {}
    UpdateBuyerTerms(const libtorrent::sha1_hash & infoHash,
                     const protocol_wire::BuyerTerms & terms,
                     const ResultHandler & handler = ResultHandler())
        : TorrentPluginRequest(infoHash)
        , terms(terms)
        , handler(handler) {
    }

    protocol_wire::BuyerTerms terms;
    ResultHandler handler;
};

struct UpdateSellerTerms : public TorrentPluginRequest {

    typedef SubroutineResult<UpdateSellerTerms> Result;
    typedef std::function<void(const Result &)> ResultHandler;

    UpdateSellerTerms() {}
    UpdateSellerTerms(const libtorrent::sha1_hash & infoHash,
                      const protocol_wire::SellerTerms & terms,
                      const ResultHandler & handler = ResultHandler())
        : TorrentPluginRequest(infoHash)
        , terms(terms)
        , handler(handler) {
    }

    protocol_wire::SellerTerms terms;

    ResultHandler handler;
};

struct ToObserveMode : public TorrentPluginRequest {

    typedef SubroutineResult<ToObserveMode> Result;
    typedef std::function<void(const Result &)> ResultHandler;

    ToObserveMode() {}
    ToObserveMode(const libtorrent::sha1_hash & infoHash,
                  const ResultHandler & handler = ResultHandler())
        : TorrentPluginRequest(infoHash)
        , handler(handler) {
    }

    ResultHandler handler;
};

struct ToSellMode : public TorrentPluginRequest {

    typedef SubroutineResult<ToSellMode> Result;
    typedef std::function<void(const Result &)> ResultHandler;

    ToSellMode() {}
    ToSellMode(const libtorrent::sha1_hash & infoHash,
               const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
               const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
               const protocol_session::SellingPolicy & sellingPolicy,
               const protocol_wire::SellerTerms & terms,
               const ResultHandler & handler = ResultHandler())
        : TorrentPluginRequest(infoHash)
        , generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
        , generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler)
        , sellingPolicy(sellingPolicy)
        , terms(terms)
        , handler(handler) {
    }

    protocol_session::GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
    protocol_session::GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;
    protocol_session::SellingPolicy sellingPolicy;
    protocol_wire::SellerTerms terms;

    ResultHandler handler;
};

struct ToBuyMode : public TorrentPluginRequest {

    typedef SubroutineResult<ToBuyMode> Result;
    typedef std::function<void(const Result &)> ResultHandler;

    ToBuyMode() {}
    ToBuyMode(const libtorrent::sha1_hash & infoHash,
             const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
             const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
             const Coin::UnspentP2PKHOutput & funding,
             const protocol_session::BuyingPolicy & policy,
             const protocol_wire::BuyerTerms & terms,
             const ResultHandler & handler = ResultHandler())
        : TorrentPluginRequest(infoHash)
        , generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
        , generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler)
        , funding(funding)
        , policy(policy)
        , terms(terms)
        , handler(handler) {
    }

    protocol_session::GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
    protocol_session::GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;
    Coin::UnspentP2PKHOutput funding;
    protocol_session::BuyingPolicy policy;
    protocol_wire::BuyerTerms terms;

    ResultHandler handler;
};

/**
struct ChangeDownloadLocation : public TorrentPluginRequest {

    typedef SubroutineResult<ChangeDownloadLocation> Result;

    ChangeDownloadLocation(const libtorrent::sha1_hash & infoHash, int pieceIndex)
        : TorrentPluginRequest(infoHash)
        , pieceIndex(pieceIndex) {
    }

    // Piece to start downloading from
    int pieceIndex;
};

//// PeerPluginRequest

struct PeerPluginRequest : public Request {
    virtual RequestTarget target() const {
        return RequestTarget::PeerPlugin;
    }
};
*/

}
}
}

#endif // JOYSTREAM_EXTENSION_REQUEST_HPP
