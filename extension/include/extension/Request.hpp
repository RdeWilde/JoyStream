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

    template <class T>
    struct Request {

        Request(const T & identifier)
            : identifier(identifier) {}

        virtual RequestTarget target() const = 0;

        T identifier;
    };

    template <class T>
    struct TorrentPluginRequest : Request<T> {

        template <class T>
        struct MissingTorrentPlugin {

            MissingTorrentPlugin(const TorrentPluginRequestType & type)
                : type(type) {
            }

            TorrentPluginRequestType type;
        };

        TorrentPluginRequest(const T & identifier, const libtorrent::sha1_hash & infoHash)
            : Request<T>(identifier)
            , infoHash(infoHash) {}

        RequestTarget target() const {
            return RequestTarget::TorrentPlugin;
        }

        virtual TorrentPluginRequestType type() const = 0;

        // info hash of torrent plugin target of request
        libtorrent::sha1_hash infoHash;
    };

    template <class T>
    struct Start : public TorrentPluginRequest<T> {

        enum Result {
            Success,
            StateIncompatibleOperation,
            SessionModeNotSet
        };

        Start(const T & identifier, const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest<T>(identifier, infoHash) {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::Start;
        }
    };

    template <class T>
    struct Stop : public TorrentPluginRequest<T> {

        enum Result {
            Success,
            StateIncompatibleOperation,
            SessionModeNotSet
        };

        Stop(const T & identifier, const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest<T>(identifier, infoHash) {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::Stop;
        }
    };

    template <class T>
    struct Pause : public TorrentPluginRequest<T> {

        enum Result {
            Success,
            StateIncompatibleOperation,
            SessionModeNotSet
        };

        Pause(const T & identifier, const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest<T>(identifier, infoHash) {}

        virtual TorrentPluginRequestType type() const {
            return TorrentPluginRequestType::Pause;
        }
    };

    template <class T>
    struct UpdateBuyerTerms : public TorrentPluginRequest<T> {

        enum Result {
            Success,
            SessionModeNotSet,
            ModeIncompatibleOperation
        };

        UpdateBuyerTerms(const T & identifier, const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest<T>(identifier, infoHash)
            , terms(terms) {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::UpdateBuyerTerms;
        }

        protocol_wire::BuyerTerms terms;
    };

    template <class T>
    struct UpdateSellerTerms : public TorrentPluginRequest<T> {

        enum class Result {
            Success,
            SessionModeNotSet,
            ModeIncompatibleOperation
        };

        UpdateSellerTerms(const T & identifier, const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest<T>(identifier, infoHash)
            , terms(terms) {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::UpdateSellerTerms;
        }

        protocol_wire::SellerTerms terms;
    };

    template <class T>
    struct ToObserveMode : public TorrentPluginRequest<T> {

        enum Result {
            Success,
            SessionAlreadyInThisMode
        };

        ToObserveMode(const T & identifier, const libtorrent::sha1_hash & infoHash)
            : TorrentPluginRequest<T>(identifier, infoHash) {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::ToObserveMode;
        }
    };

    template <class T>
    struct ToSellMode : public TorrentPluginRequest<T> {

        enum Result {
            Success,
            SessionAlreadyInThisMode
        };

        ToSellMode(const T & identifier,
                   const libtorrent::sha1_hash & infoHash,
                   const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                   const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                   const protocol_session::SellingPolicy & sellingPolicy,
                   const protocol_wire::SellerTerms & terms)
            : TorrentPluginRequest(identifier, infoHash)
            , generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
            , generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler)
            , sellingPolicy(sellingPolicy)
            , terms(terms){

        }

        virtual TorrentPluginRequestType type() const {
            return TorrentPluginRequestType::ToSellMode;
        }

        protocol_session::GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
        protocol_session::GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;
        protocol_session::SellingPolicy sellingPolicy;
        protocol_wire::SellerTerms terms;
    };

    template <class T>
    struct ToBuyMode : public TorrentPluginRequest<T> {

        enum Result {
            Success,
            SessionAlreadyInThisMode
        };

       ToBuyMode(const T & identifier,
                 const libtorrent::sha1_hash & infoHash,
                 const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                 const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                 const Coin::UnspentP2PKHOutput & funding,
                 const protocol_session::BuyingPolicy & policy,
                 const protocol_wire::BuyerTerms & terms)
            : TorrentPluginRequest(identifier, infoHash)
            , generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
            , generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler)
            , funding(funding)
            , policy(policy)
            , terms(terms) {

       }

       virtual TorrentPluginRequestType type() const{
           return TorrentPluginRequestType::ToBuyMode;
       }

       protocol_session::GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
       protocol_session::GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;
       Coin::UnspentP2PKHOutput funding;
       protocol_session::BuyingPolicy policy;
       protocol_wire::BuyerTerms terms;
    };

    template <class T>
    struct ChangeDownloadLocation : public TorrentPluginRequest<T> {

        enum Result {
            Success
            // missing more stuff
        };

        ChangeDownloadLocation(const T & identifier, const libtorrent::sha1_hash & infoHash, int pieceIndex)
            : TorrentPluginRequest(identifier, infoHash)
            , pieceIndex(pieceIndex) {
        }

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::ChangeDownloadLocation;
        }

        // Piece to start downloading from
        int pieceIndex;
    };
}
}
}

#endif // JOYSTREAM_EXTENSION_REQUEST_HPP
