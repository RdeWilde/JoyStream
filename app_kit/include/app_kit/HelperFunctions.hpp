#ifndef JOYSTREAM_APPKIT_HELPER_FUNCTIONS_HPP
#define JOYSTREAM_APPKIT_HELPER_FUNCTIONS_HPP

#include <libtorrent/sha1_hash.hpp>
#include <core/TorrentIdentifier.hpp>
#include <protocol_session/SessionState.hpp>
#include <protocol_session/BuyingPolicy.hpp>
#include <protocol_session/SellingPolicy.hpp>
#include <protocol_wire/BuyerTerms.hpp>
#include <protocol_wire/SellerTerms.hpp>

/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

#include <bitcoin/SPVWallet.hpp>
#include <paymentchannel/paymentchannel.hpp>

#include <QJsonValue>

namespace joystream{
namespace appkit {
namespace util {

// Utility functions
libtorrent::sha1_hash sha1_hash_from_hex_string(const char *);
libtorrent::sha1_hash jsonToSha1Hash(QJsonValue);
QJsonValue sha1HashToJson(libtorrent::sha1_hash);

core::TorrentIdentifier* makeTorrentIdentifier(const std::string arg);

QJsonValue sessionStateToJson(const protocol_session::SessionState&);
protocol_session::SessionState jsonToSessionState(const QJsonValue&);

QJsonValue buyerTermsToJson(const protocol_wire::BuyerTerms&);
protocol_wire::BuyerTerms jsonToBuyerTerms(const QJsonValue&);

QJsonValue buyingPolicyToJson(const protocol_session::BuyingPolicy&);
protocol_session::BuyingPolicy jsonToBuyingPolicy(const QJsonValue&);

QJsonValue sellerTermsToJson(const protocol_wire::SellerTerms&);
protocol_wire::SellerTerms jsonToSellerTerms(const QJsonValue&);

QJsonValue sellingPolicyToJson(const protocol_session::SellingPolicy&);
protocol_session::SellingPolicy jsonToSellingPolicy(const QJsonValue&);

std::vector<joystream::paymentchannel::Commitment> outputsToOutboundPaymentChannelCommitments(const std::vector<bitcoin::Store::StoreControlledOutput>&);
std::vector<paymentchannel::Commitment> outputsToInboundPaymentChannelCommitments(const std::vector<bitcoin::Store::StoreControlledOutput>&);
std::vector<paymentchannel::Refund> outputsToRefunds(const std::vector<bitcoin::Store::StoreControlledOutput>&);

}
}
}

#endif // JOYSTREAM_APPKIT_HELPER_FUNCTIONS_HPP
