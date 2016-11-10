#include <app_kit/HelperFunctions.hpp>
#include <core/TorrentIdentifier.hpp>
#include <paymentchannel/paymentchannel.hpp>

#include <QJsonObject>
#include <libtorrent/torrent_info.hpp>
#include <numeric>

namespace joystream {
namespace appkit {
namespace util {

libtorrent::sha1_hash sha1_hash_from_hex_string(const char * hex) {
  char buf[21];

  if(!libtorrent::from_hex(hex, 40, buf)){
    throw std::runtime_error("Invalid info hash string");
  }
  return libtorrent::sha1_hash(buf);
}

core::TorrentIdentifier* makeTorrentIdentifier(const std::string torrentId)
{
    core::TorrentIdentifier* ti;

    // Test if the identifier is path to a torrent file
    ti = core::TorrentIdentifier::fromTorrentFilePath(torrentId);

    if(ti)
        return ti;

    // Test if the identifier is a plain info hash string
    ti = core::TorrentIdentifier::fromHashString(torrentId);

    if(ti)
        return ti;

    // Test if the identifier is a magner link
    return core::TorrentIdentifier::fromMagnetLinkString(torrentId);
}

QJsonValue sessionStateToJson(const protocol_session::SessionState &state) {
    switch(state) {
        case protocol_session::SessionState::paused :
            return QJsonValue("paused");
        case protocol_session::SessionState::started :
            return QJsonValue("started");
        case protocol_session::SessionState::stopped :
            return QJsonValue("stopped");
    }
}

protocol_session::SessionState jsonToSessionState(const QJsonValue &value) {
    const QString state = value.toString();

    if(state == "started")
        return protocol_session::SessionState::started;

    if(state == "stopped")
        return protocol_session::SessionState::stopped;

    if(state == "paused")
        return protocol_session::SessionState::paused;

    throw std::runtime_error("invalid state value");
}

QJsonValue buyerTermsToJson(const protocol_wire::BuyerTerms &buyerTerms) {
    QJsonObject value;

    value["maxContractFeeRate"] = QJsonValue((double)buyerTerms.maxContractFeePerKb());
    value["maxLock"] = QJsonValue((double)buyerTerms.maxLock());
    value["maxPrice"] = QJsonValue((double)buyerTerms.maxPrice());
    value["minSellers"] = QJsonValue((double)buyerTerms.minNumberOfSellers());

    return value;
}

protocol_wire::BuyerTerms jsonToBuyerTerms(const QJsonValue & value) {
    protocol_wire::BuyerTerms buyerTerms;

    if(!value.isObject())
        throw std::runtime_error("expected json object value");

    QJsonObject terms = value.toObject();

    buyerTerms.setMaxContractFeePerKb(terms["maxContractFeeRate"].toDouble());
    buyerTerms.setMaxLock(terms["maxLock"].toDouble());
    buyerTerms.setMaxPrice(terms["maxPrice"].toDouble());
    buyerTerms.setMinNumberOfSellers(terms["minSellers"].toDouble());

    return buyerTerms;
}

QJsonValue buyingPolicyToJson(const protocol_session::BuyingPolicy &buyingPolicy) {
    QJsonObject policy;
    policy["minTimeBeforeBuildingContract"] = QJsonValue((double)buyingPolicy.minTimeBeforeBuildingContract().count());
    policy["servicingPieceTimeOutLimit"] = QJsonValue((double)buyingPolicy.servicingPieceTimeOutLimit().count());
    //safer to conver to a string?
    policy["sellerTermsOrderingPolicy"] = QJsonValue((int)buyingPolicy.sellerTermsOrderingPolicy());
    return policy;
}

protocol_session::BuyingPolicy jsonToBuyingPolicy(const QJsonValue &value) {
    if(!value.isObject())
        throw std::runtime_error("expected json object value");

    QJsonObject policy = value.toObject();

    return protocol_session::BuyingPolicy(policy["minTimeBeforeBuildingContract"].toDouble(),
                                          policy["servicingPieceTimeOutLimit"].toDouble(),
                                          (protocol_wire::SellerTerms::OrderingPolicy)policy["sellerTermsOrderingPolicy"].toInt());
}

QJsonValue sellerTermsToJson(const protocol_wire::SellerTerms &sellerTerms) {
    QJsonObject terms;
    terms["minContractFeeRate"] = QJsonValue((double)sellerTerms.minContractFeePerKb());
    terms["minLock"] = QJsonValue(sellerTerms.minLock());
    terms["minPrice"] = QJsonValue((double)sellerTerms.minPrice());
    terms["maxSellers"] = QJsonValue((double)sellerTerms.maxSellers());
    terms["settlementFee"] = QJsonValue((double)sellerTerms.settlementFee());
    return terms;
}

protocol_wire::SellerTerms jsonToSellerTerms(const QJsonValue &value) {
    if(!value.isObject())
        throw std::runtime_error("expected json object value");

    QJsonObject terms = value.toObject();

    protocol_wire::SellerTerms sellerTerms;

    sellerTerms.setMaxSellers(terms["maxSellers"].toInt());
    sellerTerms.setMinContractFeePerKb(terms["minContractFeeRate"].toDouble());
    sellerTerms.setMinLock(terms["minLock"].toInt());
    sellerTerms.setMinPrice(terms["minPrice"].toDouble());
    sellerTerms.setSettlementFee(terms["settlementFee"].toDouble());

    return sellerTerms;
}

QJsonValue sellingPolicyToJson(const protocol_session::SellingPolicy &sellingPolicy) {
    return QJsonValue(true);
}

protocol_session::SellingPolicy jsonToSellingPolicy(const QJsonValue &value) {
    return protocol_session::SellingPolicy();
}

uint64_t estimateRequiredFundsToBuyTorrent(boost::shared_ptr<const libtorrent::torrent_info> metadata, joystream::protocol_wire::BuyerTerms terms) {
    // This routine tries to make a reasonable over estimation of the funds that would be required to
    // to pay for to download the full torrent, taking into account the maximum fee rate set by the buyer

    // Estimate maximum price for the torrent
    // It may be more reasonable to determine how pieces are remaining to be downloaded instead
    uint64_t maximumTorrentvalue = metadata->num_pieces() * terms.maxPrice();

    // The number of utxos used to actually fund a contract can only be determined at the point
    // when we actually lock the funds in a wallet.. this is just a high estimate (assuming the utxo
    // selection algorithm used by a wallet will try to minimize the number of inputs used)
    const int numberOfInputs = 5;

    // Similarly we might get a greater number of sellers than the buyer's minimum requirement (seller's maximum value)
    const int numberOfSellers = terms.minNumberOfSellers() * 2;

    uint64_t estimatedContractFee = paymentchannel::Contract::fee(numberOfSellers, true, terms.maxContractFeePerKb(), numberOfInputs);

    return maximumTorrentvalue + estimatedContractFee;
}

std::vector<joystream::paymentchannel::Commitment> outputsToOutboundPaymentChannelCommitments(const std::vector<bitcoin::Store::StoreControlledOutput> &nonStandardOutputs) {
    using namespace joystream::bitcoin;

    std::vector<joystream::paymentchannel::Commitment> channels;

    for (const Store::StoreControlledOutput &output : nonStandardOutputs) {
        try{
            paymentchannel::RedeemScript paychanScript = paymentchannel::RedeemScript::deserialize(output.redeemScript);
            if(paychanScript.isPayorPublicKey(output.keyPair.pk())) {
                channels.push_back(paymentchannel::Commitment(output.value, output.redeemScript));
            }
        }catch(std::exception &e) {
            //not a payment channel
        }
    }

    return channels;
}

std::vector<paymentchannel::Commitment> outputsToInboundPaymentChannelCommitments(const std::vector<bitcoin::Store::StoreControlledOutput> &nonStandardOutputs) {
    using namespace joystream::bitcoin;

    std::vector<paymentchannel::Commitment> channels;

    for (const Store::StoreControlledOutput &output : nonStandardOutputs) {
        try{
            paymentchannel::RedeemScript paychanScript = paymentchannel::RedeemScript::deserialize(output.redeemScript);
            if(paychanScript.isPayeePublicKey(output.keyPair.pk())){
                channels.push_back(paymentchannel::Commitment(output.value, output.redeemScript));
            }
        }catch(std::exception &e) {
            //not a payment channel
        }
    }

    return channels;
}

std::vector<paymentchannel::Refund> outputsToRefunds(const std::vector<bitcoin::Store::StoreControlledOutput> &nonStandardOutputs) {
    using namespace joystream::bitcoin;

    std::vector<paymentchannel::Refund> refunds;

    for (const Store::StoreControlledOutput &output : nonStandardOutputs) {
        try{
            paymentchannel::RedeemScript paychanScript = paymentchannel::RedeemScript::deserialize(output.redeemScript);

            auto pk = output.keyPair.pk();
            if(paychanScript.isPayorPublicKey(pk)) {
                refunds.push_back(paymentchannel::Refund(output.outPoint, paymentchannel::Commitment(output.value, output.redeemScript), output.keyPair));
            }
        }catch(std::exception &e) {
            //not a payment channel
        }
    }

    return refunds;
}

}}}
