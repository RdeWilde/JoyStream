/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 7 2016
 */

#ifndef JOYSTREAM_EXTENSION_REQUEST_HPP
#define JOYSTREAM_EXTENSION_REQUEST_HPP

#include <extension/TorrentPlugin.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <protocol_session/protocol_session.hpp>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/socket.hpp>

#include <functional>

namespace joystream {
namespace extension {
namespace request {


// Identifies a spesific request, of any type, made to the plugin
typedef uint32_t RequestIdentifier;

/// All request classes must satisfy requirements for use with boost::variant.
/// http://www.boost.org/doc/libs/1_55_0/doc/html/variant.html

// A standard handler which handles requests without an explicit result and no possibility of an exception being thrown
typedef std::function <void(void)> NoExceptionSubroutineHandler;

// A standard handler which handles requests without an explicit result, i.e. subroutine
typedef std::function<void(const std::exception_ptr &)> SubroutineHandler;

// A standard handler which handles requests with and explicit result, i.e. a function
//template<typename... Args>
//using FunctionHandler = std::function<void(const std::exception_ptr &, Args... args)>;

/// Libtorrent requests

struct AddTorrent {

    typedef std::function<void(libtorrent::error_code &, libtorrent::torrent_handle &)> AddTorrentHandler;

    AddTorrent(const libtorrent::add_torrent_params & params, const AddTorrentHandler & handler)
        : params(params)
        , handler(handler) {
    }

    libtorrent::add_torrent_params params;
    AddTorrentHandler handler;
};

struct RemoveTorrent {

    RemoveTorrent(const libtorrent::sha1_hash & infoHash, const SubroutineHandler & handler)
        : infoHash(infoHash)
        , handler(handler) {
    }

    libtorrent::sha1_hash infoHash;
    SubroutineHandler handler;
};

struct PauseTorrent {

    PauseTorrent(const libtorrent::sha1_hash & infoHash, bool graceful, const SubroutineHandler & handler)
        : infoHash(infoHash)
        , graceful(graceful)
        , handler(handler) {
    }

    libtorrent::sha1_hash infoHash;
    bool graceful;
    SubroutineHandler handler;
};

struct ResumeTorrent {

    ResumeTorrent(const libtorrent::sha1_hash & infoHash, const SubroutineHandler & handler)
        : infoHash(infoHash)
        , handler(handler) {
    }

    libtorrent::sha1_hash infoHash;
    SubroutineHandler handler;
};

/// Plugin requests

struct PostTorrentPluginStatusUpdates {

    PostTorrentPluginStatusUpdates() {}
};

struct PostPeerPluginStatusUpdates {

    PostPeerPluginStatusUpdates(const libtorrent::sha1_hash & infoHash)
        : _infoHash(infoHash) {}

    libtorrent::sha1_hash _infoHash;
};

struct PauseLibtorrent {

    PauseLibtorrent() {}

    PauseLibtorrent(const NoExceptionSubroutineHandler & handler)
        : handler(handler) {
    }

    NoExceptionSubroutineHandler handler;
};

struct StopAllTorrentPlugins {

    StopAllTorrentPlugins() {}

    StopAllTorrentPlugins(const NoExceptionSubroutineHandler & handler)
        : handler(handler) {
    }

    NoExceptionSubroutineHandler handler;
};

struct UpdateLibtorrentUploadBlocking {

    UpdateLibtorrentUploadBlocking() {}
    UpdateLibtorrentUploadBlocking(const libtorrent::sha1_hash & infoHash,
                                   TorrentPlugin::LibtorrentInteraction newInteraction,
                                   const SubroutineHandler & handler)
        : infoHash(infoHash)
        , newInteraction(newInteraction)
        , handler(handler) {
    }

    libtorrent::sha1_hash infoHash;
    TorrentPlugin::LibtorrentInteraction newInteraction;
    SubroutineHandler handler;
};

struct Start {

    Start() {}
    Start(const libtorrent::sha1_hash & infoHash)
        : infoHash(infoHash){
    }

    libtorrent::sha1_hash infoHash;
};

struct Stop {

    Stop() {}
    Stop(const libtorrent::sha1_hash & infoHash,
         const SubroutineHandler & handler)
        : infoHash(infoHash)
        , handler(handler){
    }

    libtorrent::sha1_hash infoHash;
    SubroutineHandler handler;
};

struct Pause {

    Pause() {}
    Pause(const libtorrent::sha1_hash & infoHash,
          const SubroutineHandler & handler)
        : infoHash(infoHash)
        , handler(handler) {
    }

    libtorrent::sha1_hash infoHash;
    SubroutineHandler handler;
};

struct UpdateBuyerTerms {

    UpdateBuyerTerms() {}
    UpdateBuyerTerms(const libtorrent::sha1_hash & infoHash,
                     const protocol_wire::BuyerTerms & terms,
                     const SubroutineHandler & handler)
        : infoHash(infoHash)
        , terms(terms)
        , handler(handler) {
    }

    libtorrent::sha1_hash infoHash;
    protocol_wire::BuyerTerms terms;
    SubroutineHandler handler;
};

struct UpdateSellerTerms {

    UpdateSellerTerms() {}
    UpdateSellerTerms(const libtorrent::sha1_hash & infoHash,
                      const protocol_wire::SellerTerms & terms,
                      const SubroutineHandler & handler)
        : infoHash(infoHash)
        , terms(terms)
        , handler(handler) {
    }

    libtorrent::sha1_hash infoHash;
    protocol_wire::SellerTerms terms;
    SubroutineHandler handler;
};

struct ToObserveMode {

    ToObserveMode() {}
    ToObserveMode(const libtorrent::sha1_hash & infoHash,
                  const SubroutineHandler & handler)
        : infoHash(infoHash)
        , handler(handler) {
    }

    libtorrent::sha1_hash infoHash;
    SubroutineHandler handler;
};

struct ToSellMode {

    ToSellMode() {}
    ToSellMode(const libtorrent::sha1_hash & infoHash,
               const protocol_session::GenerateP2SHKeyPairCallbackHandler & generateKeyPairCallbackHandler,
               const protocol_session::GenerateReceiveAddressesCallbackHandler & genReceiveAddressesCallbackHandler,
               const protocol_session::SellingPolicy & sellingPolicy,
               const protocol_wire::SellerTerms & terms,
               const SubroutineHandler & handler)
        : infoHash(infoHash)
        , generateKeyPairCallbackHandler(generateKeyPairCallbackHandler)
        , generateReceiveAddressesCallbackHandler(genReceiveAddressesCallbackHandler)
        , sellingPolicy(sellingPolicy)
        , terms(terms)
        , handler(handler) {
    }

    libtorrent::sha1_hash infoHash;
    protocol_session::GenerateP2SHKeyPairCallbackHandler generateKeyPairCallbackHandler;
    protocol_session::GenerateReceiveAddressesCallbackHandler generateReceiveAddressesCallbackHandler;
    protocol_session::SellingPolicy sellingPolicy;
    protocol_wire::SellerTerms terms;
    SubroutineHandler handler;
};

struct ToBuyMode {

    ToBuyMode() {}
    ToBuyMode(const libtorrent::sha1_hash & infoHash,
              const protocol_session::GenerateP2SHKeyPairCallbackHandler & generateKeyPairCallbackHandler,
              const protocol_session::GenerateReceiveAddressesCallbackHandler & genReceiveAddressesCallbackHandler,
              const protocol_session::GenerateChangeAddressesCallbackHandler & genChangeAddressesCallbackHandler,
              const Coin::UnspentOutputSet & funding,
              const protocol_session::BuyingPolicy & policy,
              const protocol_wire::BuyerTerms & terms,
              const SubroutineHandler & handler)
        : infoHash(infoHash)
        , generateKeyPairCallbackHandler(generateKeyPairCallbackHandler)
        , generateReceiveAddressesCallbackHandler(genReceiveAddressesCallbackHandler)
        , generateChangeAddressesCallbackHandler(genChangeAddressesCallbackHandler)
        , funding(funding)
        , policy(policy)
        , terms(terms)
        , handler(handler) {
    }

    libtorrent::sha1_hash infoHash;
    protocol_session::GenerateP2SHKeyPairCallbackHandler generateKeyPairCallbackHandler;
    protocol_session::GenerateReceiveAddressesCallbackHandler generateReceiveAddressesCallbackHandler;
    protocol_session::GenerateChangeAddressesCallbackHandler generateChangeAddressesCallbackHandler;
    Coin::UnspentOutputSet funding;
    protocol_session::BuyingPolicy policy;
    protocol_wire::BuyerTerms terms;
    SubroutineHandler handler;
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
*/
}
}
}

#endif // JOYSTREAM_EXTENSION_REQUEST_HPP
