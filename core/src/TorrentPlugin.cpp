/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#include <core/TorrentPlugin.hpp>

namespace joystream {
namespace core {

/**
void TorrentPlugin::start(const extension::request::SubroutineHandler & handler) {
    plugin->submit(extension::request::Start(infoHash, handler));
}

void TorrentPlugin::stop(const extension::request::SubroutineHandler & handler) {
    plugin->submit(extension::request::Stop(infoHash, handler));
}

void TorrentPlugin::pause(const extension::request::SubroutineHandler & handler) {
   plugin->submit(extension::request::Pause(infoHash, handler));
}

void TorrentPlugin::updateTerms(const protocol_wire::BuyerTerms & terms,
                          const extension::request::SubroutineHandler & handler) {
    plugin->submit(extension::request::UpdateBuyerTerms(infoHash, terms, handler));
}

void TorrentPlugin::updateTerms(const protocol_wire::SellerTerms & terms,
                          const extension::request::SubroutineHandler & handler) {
    plugin->submit(extension::request::UpdateSellerTerms(infoHash, terms, handler));
}

void TorrentPlugin::toObserveMode(const extension::request::SubroutineHandler & handler) {
    plugin->submit(extension::request::ToObserveMode(infoHash, handler));
}

void TorrentPlugin::toSellMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                         const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                         const protocol_session::SellingPolicy & sellingPolicy,
                         const protocol_wire::SellerTerms & terms,
                         const extension::request::SubroutineHandler & handler) {

    plugin->submit(extension::request::ToSellMode(infoHash,
                                                   generateKeyPairsCallbackHandler,
                                                   generateP2PKHAddressesCallbackHandler,
                                                   sellingPolicy,
                                                   terms,
                                                   handler));
}

void TorrentPlugin::toBuyMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                        const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                        const Coin::UnspentP2PKHOutput & funding,
                        const protocol_session::BuyingPolicy & policy,
                        const protocol_wire::BuyerTerms & terms,
                        const extension::request::SubroutineHandler & handler) {

    plugin->submit(extension::request::ToBuyMode(infoHash,
                                                  generateKeyPairsCallbackHandler,
                                                  generateP2PKHAddressesCallbackHandler,
                                                  funding,
                                                  policy,
                                                  terms,
                                                  handler));
}
*/

}
}
