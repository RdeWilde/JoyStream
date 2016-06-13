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

    enum class TorrentPluginRequestType {
        TorrentPluginRequest,
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

    struct Request {
        virtual RequestTarget target() const = 0;
    };

    struct TorrentPluginRequest : Request {

        TorrentPluginRequest(const libtorrent::sha1_hash & infoHash)
            : infoHash(infoHash) {}

        RequestTarget target() const {
            return RequestTarget::TorrentPlugin;
        }

        virtual TorrentPluginRequestType type() const = 0;

        // info hash of torrent plugin target of request
        libtorrent::sha1_hash infoHash;
    };

    struct Start : public TorrentPluginRequest {

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::Start;
        }
    };

    struct Stop : public TorrentPluginRequest {

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::Stop;
        }
    };

    struct Pause : public TorrentPluginRequest {

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::Pause;
        }
    };

    struct UpdateBuyerTerms : public TorrentPluginRequest {

        UpdateBuyerTerms(const libtorrent::sha1_hash & infoHash, const protocol_wire::BuyerTerms & terms)
            : TorrentPluginRequest(infoHash)
            , terms(terms) {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::UpdateBuyerTerms;
        }

        protocol_wire::BuyerTerms terms;
    };

    struct UpdateSellerTerms : public TorrentPluginRequest {

        UpdateSellerTerms(const libtorrent::sha1_hash & infoHash, const protocol_wire::SellerTerms & terms)
            : TorrentPluginRequest(infoHash)
            , terms(terms) {}

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::UpdateSellerTerms;
        }

        protocol_wire::SellerTerms terms;
    };

    struct ToObserveMode : public TorrentPluginRequest {

        virtual TorrentPluginRequestType type() const{
            return TorrentPluginRequestType::ToObserveMode;
        }
    };

    struct ToSellMode : public TorrentPluginRequest {

        ToSellMode(const libtorrent::sha1_hash & infoHash,
                   const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                   const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                   const protocol_session::SellingPolicy & sellingPolicy,
                   const protocol_wire::SellerTerms & terms)
            : TorrentPluginRequest(infoHash)
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

    struct ToBuyMode : public TorrentPluginRequest {

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

    struct ChangeDownloadLocation : public TorrentPluginRequest {

        ChangeDownloadLocation(const libtorrent::sha1_hash & infoHash, int pieceIndex)
            : TorrentPluginRequest(infoHash)
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
