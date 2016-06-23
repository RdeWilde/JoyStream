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

namespace joystream {
namespace extension {
namespace request {

class MissingTorrent : public std::runtime_error {

public:

    MissingTorrent()
        : std::runtime_error("Torrent missing") {
    }

};

// T = request in this namespace
template <class T>
class SubroutineResult {

public:

    SubroutineResult() {
    }

    SubroutineResult(const T * request)
        : _request(request) {
    }

    SubroutineResult(const T * request,
                     const std::exception_ptr & exception)
        : _request(request)
        , exception(exception) {
    }

    void result() const {

        if(exception)
            std::rethrow_exception(exception);
    }

    const T * request() const {
        return _request;
    }

private:

    friend class Plugin;

    // Initial request
    const T * _request;

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

    MethodResult(const T * request,
                 const std::exception_ptr exception,
                 const T2 & result)
        : SubroutineResult<T>(request, exception)
        , _result(result) {
    }

    T2 result() const {

        SubroutineResult<T>::result();

        return _result;
    }

private:

    // Method result
    T2 _result;
};

/**
enum class RequestTarget {
    Plugin,
    TorrentPlugin,
    PeerPlugin
};

// Base class for all requests
struct Request {
    virtual ~Request() {}
    virtual RequestTarget target() const = 0;
};

//// PluginRequest

struct PluginRequest : public Request {

    virtual RequestTarget target() const {
        return RequestTarget::Plugin;
    }

};

//// TorrentPluginRequest

// NB: **Not sure this enum is really required?**
enum class TorrentPluginRequestType {
    Start,
    Stop,
    Pause,
    UpdateBuyerTerms,
    UpdateSellerTerms,
    ToObserveMode,
    ToSellMode,
    ToBuyMode,
    ChangeDownloadLocation
};

struct TorrentPluginRequest : public Request {

    TorrentPluginRequest(const libtorrent::sha1_hash & infoHash)
        : infoHash(infoHash) {}

    virtual ~TorrentPluginRequest() {}

    RequestTarget target() const {
        return RequestTarget::TorrentPlugin;
    }

    virtual TorrentPluginRequestType type() const = 0;

    // info hash of torrent plugin target of request
    libtorrent::sha1_hash infoHash;
};
*/

struct TorrentPluginRequest {

    TorrentPluginRequest(const libtorrent::sha1_hash & infoHash)
        : infoHash(infoHash) {
    }

    virtual ~TorrentPluginRequest() {
    }

    libtorrent::sha1_hash infoHash;
};

struct Start : public TorrentPluginRequest {

    typedef SubroutineResult<Start> Result;

    Start(const libtorrent::sha1_hash & infoHash)
        : TorrentPluginRequest(infoHash) {
    }
};

struct Stop : public TorrentPluginRequest {

    typedef SubroutineResult<Stop> Result;

    Stop(const libtorrent::sha1_hash & infoHash)
        : TorrentPluginRequest(infoHash) {
    }
};

struct Pause : public TorrentPluginRequest {

    typedef SubroutineResult<Pause> Result;

    Pause(const libtorrent::sha1_hash & infoHash)
        : TorrentPluginRequest(infoHash) {
    }
};

struct UpdateBuyerTerms : public TorrentPluginRequest {

    typedef SubroutineResult<UpdateBuyerTerms> Result;

    UpdateBuyerTerms(const libtorrent::sha1_hash & infoHash, const protocol_wire::BuyerTerms & terms)
        : TorrentPluginRequest(infoHash)
        , terms(terms) {
    }

    protocol_wire::BuyerTerms terms;
};

struct UpdateSellerTerms : public TorrentPluginRequest {

    typedef SubroutineResult<UpdateSellerTerms> Result;

    UpdateSellerTerms(const libtorrent::sha1_hash & infoHash, const protocol_wire::SellerTerms & terms)
        : TorrentPluginRequest(infoHash)
        , terms(terms) {
    }

    protocol_wire::SellerTerms terms;
};

struct ToObserveMode : public TorrentPluginRequest {

    typedef SubroutineResult<ToObserveMode> Result;

    ToObserveMode(const libtorrent::sha1_hash & infoHash)
        : TorrentPluginRequest(infoHash) {
    }
};

struct ToSellMode : public TorrentPluginRequest {

    typedef SubroutineResult<ToSellMode> Result;

    ToSellMode(const libtorrent::sha1_hash & infoHash,
               const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
               const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
               const protocol_session::SellingPolicy & sellingPolicy,
               const protocol_wire::SellerTerms & terms)
        : TorrentPluginRequest(infoHash)
        , generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
        , generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler)
        , sellingPolicy(sellingPolicy)
        , terms(terms) {
    }

    protocol_session::GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
    protocol_session::GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;
    protocol_session::SellingPolicy sellingPolicy;
    protocol_wire::SellerTerms terms;
};

struct ToBuyMode : public TorrentPluginRequest {

    typedef SubroutineResult<ToBuyMode> Result;

    ToBuyMode(const libtorrent::sha1_hash & infoHash,
             const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
             const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
             const Coin::UnspentP2PKHOutput & funding,
             const protocol_session::BuyingPolicy & policy,
             const protocol_wire::BuyerTerms & terms)
        : TorrentPluginRequest(infoHash)
        , generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
        , generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler)
        , funding(funding)
        , policy(policy)
        , terms(terms) {}

    protocol_session::GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
    protocol_session::GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;
    Coin::UnspentP2PKHOutput funding;
    protocol_session::BuyingPolicy policy;
    protocol_wire::BuyerTerms terms;
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
