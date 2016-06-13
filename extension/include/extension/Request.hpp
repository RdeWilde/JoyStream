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

    struct TorrentPluginRequest {

        TorrentPluginRequest(const libtorrent::sha1_hash & infoHash)
            : infoHash(infoHash) {}

        // info hash of torrent plugin target of request
        libtorrent::sha1_hash infoHash;
    };

    struct Start : public TorrentPluginRequest { };

    struct Stop : public TorrentPluginRequest { };

    struct Pause : public TorrentPluginRequest { };

    struct UpdateBuyerTerms : public TorrentPluginRequest {

        UpdateBuyerTerms(const libtorrent::sha1_hash & infoHash, const protocol_wire::BuyerTerms & terms)
            : TorrentPluginRequest(infoHash)
            , terms(terms) {}

        protocol_wire::BuyerTerms terms;
    };

    struct UpdateSellerTerms : public TorrentPluginRequest {

        UpdateSellerTerms(const libtorrent::sha1_hash & infoHash, const protocol_wire::SellerTerms & terms)
            : TorrentPluginRequest(infoHash)
            , terms(terms) {}

        protocol_wire::SellerTerms terms;
    };

    struct ToObserveMode : public TorrentPluginRequest { };

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

       protocol_session::GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
       protocol_session::GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;
       Coin::UnspentP2PKHOutput funding;
       protocol_session::BuyingPolicy policy;
       protocol_wire::BuyerTerms terms;
    };

    struct ChangeDownloadLocation : public TorrentPluginRequest {

        ChangeDownloadLocation(const libtorrent::sha1_hash & infoHash, int pieceIndex)
            : TorrentPluginRequest(infoHash)
            , pieceIndex(pieceIndex){
        }

        // Piece to start downloading from
        int pieceIndex;
    };
}
}
}

#endif // JOYSTREAM_EXTENSION_REQUEST_HPP
