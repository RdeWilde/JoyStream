/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 7 2016
 */

#ifndef JOYSTREAM_EXTENSION_REQUEST_HPP
#define JOYSTREAM_EXTENSION_REQUEST_HPP

#include <extension/request/ChangeDownloadLocation.hpp>
#include <extension/request/PluginRequest.hpp>
#include <extension/request/PluginRequestType.hpp>
#include <extension/request/StartBuyerTorrentPlugin.hpp>
#include <extension/request/StartObserverTorrentPlugin.hpp>
#include <extension/request/StartSellerTorrentPlugin.hpp>
#include <extension/request/TorrentPluginRequest.hpp>
#include <extension/request/TorrentPluginRequestType.hpp>

/**
// Change session to observe mode
void toObserveMode();

// Change session to sell mode
void toSellMode(const RemovedConnectionCallbackHandler<ConnectionIdType> &,
                const GenerateKeyPairsCallbackHandler &,
                const GenerateP2PKHAddressesCallbackHandler &,
                const LoadPieceForBuyer<ConnectionIdType> &,
                const ClaimLastPayment<ConnectionIdType> &,
                const AnchorAnnounced<ConnectionIdType> &,
                const SellingPolicy &,
                const protocol_wire::SellerTerms &);

// Change session to buy mode
void toBuyMode(const RemovedConnectionCallbackHandler<ConnectionIdType> &,
               const GenerateKeyPairsCallbackHandler &,
               const GenerateP2PKHAddressesCallbackHandler &,
               const BroadcastTransaction &,
               const FullPieceArrived<ConnectionIdType> &,
               const Coin::UnspentP2PKHOutput &,
               const BuyingPolicy &,
               const protocol_wire::BuyerTerms &,
               const TorrentPieceInformation &);

//// Manage state

// Can only be called when state has been set.

void start();

void stop();

void pause();

// Update terms
void updateTerms(const protocol_wire::BuyerTerms &);

// Update terms when selling
void updateTerms(const protocol_wire::SellerTerms &);
*/

#endif // JOYSTREAM_EXTENSION_REQUEST_HPP
