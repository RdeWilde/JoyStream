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

namespace joystream {
namespace extension {
namespace request {

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

        struct MissingTorrentPlugin {

            MissingTorrentPlugin(const TorrentPluginRequest * request)
                : request(request) {}

            const TorrentPluginRequest * request;
        };

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

    struct Start : public TorrentPluginRequest {

        enum Outcome {
            StateIncompatibleOperation,
            SessionModeNotSet,
            Success
        };

        struct Response {

            Response(const Start * request, Outcome outcome)
                : request(request)
                , outcome(outcome) {}

            const Start * request;
            Outcome outcome;
        };

        Start(const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest(infoHash) {}

        virtual ~Start() {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::Start;
        }
    };

    struct Stop : public TorrentPluginRequest {

        enum Outcome {
            StateIncompatibleOperation,
            SessionModeNotSet,
            Success
        };

        struct Response {

            Response(const Stop * request, Outcome outcome)
                : request(request)
                , outcome(outcome) {}

            const Stop * request;
            Outcome outcome;
        };

        Stop(const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest(infoHash) {}

        virtual ~Stop() {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::Stop;
        }
    };

    struct Pause : public TorrentPluginRequest {

        enum class Outcome {
            StateIncompatibleOperation,
            SessionModeNotSet,
            Success
        };

        struct Response {

            Response(const Pause * request, Outcome outcome)
                : request(request)
                , outcome(outcome) {}

            const Pause * request;
            Outcome outcome;
        };

        Pause(const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest(infoHash) {}

        virtual ~Pause() {}

        virtual TorrentPluginRequestType type() const {
            return TorrentPluginRequestType::Pause;
        }
    };

    struct UpdateBuyerTerms : public TorrentPluginRequest {

        enum class Outcome {
            SessionModeNotSet,
            ModeIncompatibleOperation,
            Success
        };

        struct Response {

            Response(const UpdateBuyerTerms * request, Outcome outcome)
                : request(request)
                , outcome(outcome) {}

            const UpdateBuyerTerms * request;
            Outcome outcome;
        };

        UpdateBuyerTerms(const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest(infoHash)
            , terms(terms) {}

        virtual ~UpdateBuyerTerms() {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::UpdateBuyerTerms;
        }

        protocol_wire::BuyerTerms terms;
    };

    struct UpdateSellerTerms : public TorrentPluginRequest {

        enum class Outcome {
            SessionModeNotSet,
            ModeIncompatibleOperation,
            Success
        };

        struct Response {

            Response(const UpdateSellerTerms * request, Outcome outcome)
                : request(request)
                , outcome(outcome) {}

            const UpdateSellerTerms * request;
            Outcome outcome;
        };

        UpdateSellerTerms(const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest(infoHash)
            , terms(terms) {}

        virtual ~UpdateSellerTerms() {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::UpdateSellerTerms;
        }

        protocol_wire::SellerTerms terms;
    };

    struct ToObserveMode : public TorrentPluginRequest {

        enum Outcome {
            SessionAlreadyInThisMode,
            Success
        };

        struct Response {

            Response(const ToObserveMode * request, Outcome outcome)
                : request(request)
                , outcome(outcome) {}

            const ToObserveMode * request;
            Outcome outcome;
        };

        ToObserveMode(const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest(infoHash) {}

        virtual ~ToObserveMode() {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::ToObserveMode;
        }
    };

    struct ToSellMode : public TorrentPluginRequest {

        enum class Outcome {
            SessionAlreadyInThisMode,
            Success
        };

        struct Response {

            Response(const ToSellMode * request, Outcome outcome)
                : request(request)
                , outcome(outcome) {}

            const ToSellMode * request;
            Outcome outcome;
        };

        ToSellMode(const libtorrent::sha1_hash & infoHash,
                   const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                   const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                   const protocol_session::SellingPolicy & sellingPolicy,
                   const protocol_wire::SellerTerms & terms)
            : TorrentPluginRequest(infoHash)
            , generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
            , generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler)
            , sellingPolicy(sellingPolicy)
            , terms(terms) {}

        virtual ~ToSellMode() {}

        virtual TorrentPluginRequestType type() const {
            return TorrentPluginRequestType::ToSellMode;
        }

        protocol_session::GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
        protocol_session::GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;
        protocol_session::SellingPolicy sellingPolicy;
        protocol_wire::SellerTerms terms;
    };

    struct ToBuyMode : public TorrentPluginRequest {

        enum Outcome {
            SessionAlreadyInThisMode,
            Success
        };

        struct Response {

            Response(const ToBuyMode * request, Outcome outcome)
                : request(request)
                , outcome(outcome) {}

            const ToBuyMode * request;
            Outcome outcome;
        };

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

        virtual ~ToBuyMode() {}

        virtual TorrentPluginRequestType type() const {
           return TorrentPluginRequestType::ToBuyMode;
        }

        protocol_session::GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
        protocol_session::GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;
        Coin::UnspentP2PKHOutput funding;
        protocol_session::BuyingPolicy policy;
        protocol_wire::BuyerTerms terms;
    };

    struct ChangeDownloadLocation : public TorrentPluginRequest {

        enum Outcome {
            Success
            // missing more stuff
        };

        struct Response {

            Response(const ChangeDownloadLocation * request, Outcome outcome)
                : request(request)
                , outcome(outcome) {}

            const ChangeDownloadLocation * request;
            Outcome outcome;
        };

        ChangeDownloadLocation(const libtorrent::sha1_hash & infoHash, int pieceIndex)
            : TorrentPluginRequest(infoHash)
            , pieceIndex(pieceIndex) {
        }

        virtual ~ChangeDownloadLocation() {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::ChangeDownloadLocation;
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
}
}
}

#endif // JOYSTREAM_EXTENSION_REQUEST_HPP
