#ifndef JOYSTREAM_APPKIT_HELPER_FUNCTIONS_HPP
#define JOYSTREAM_APPKIT_HELPER_FUNCTIONS_HPP

#include <libtorrent/sha1_hash.hpp>
#include <core/TorrentIdentifier.hpp>
#include <protocol_session/SessionState.hpp>
#include <protocol_session/BuyingPolicy.hpp>
#include <protocol_session/SellingPolicy.hpp>
#include <protocol_wire/BuyerTerms.hpp>
#include <protocol_wire/SellerTerms.hpp>

#include <bitcoin/SPVWallet.hpp>
#include <paymentchannel/paymentchannel.hpp>

#include <QJsonValue>

namespace joystream{
namespace appkit {
namespace util {

// Utility functions
libtorrent::sha1_hash sha1_hash_from_hex_string(const char *);
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

uint64_t estimateRequiredFundsToBuyTorrent(boost::shared_ptr<const libtorrent::torrent_info> metadata, joystream::protocol_wire::BuyerTerms terms);

std::vector<joystream::paymentchannel::Commitment> outputsToOutboundPaymentChannelCommitments(const std::vector<bitcoin::Store::StoreControlledOutput>&);
std::vector<paymentchannel::Commitment> outputsToInboundPaymentChannelCommitments(const std::vector<bitcoin::Store::StoreControlledOutput>&);
std::vector<paymentchannel::Refund> outputsToRefunds(const std::vector<bitcoin::Store::StoreControlledOutput>&);

}
}
}

#endif // JOYSTREAM_APPKIT_HELPER_FUNCTIONS_HPP
