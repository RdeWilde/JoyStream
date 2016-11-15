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

libtorrent::sha1_hash jsonToSha1Hash(QJsonValue value) {

    if(!value.isString())
        throw std::runtime_error("expecting json string for infoHash");

    return util::sha1_hash_from_hex_string(value.toString().toStdString().data());
}

QJsonValue sha1HashToJson(libtorrent::sha1_hash infoHash) {
    std::string infoHashHex = libtorrent::to_hex(infoHash.to_string());
    return QJsonValue(QString::fromStdString(infoHashHex));
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
        default:
            assert(false);
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

    if(!terms["maxContractFeeRate"].isDouble())
        throw std::runtime_error("expecting json number for maxContractFeeRate");

    buyerTerms.setMaxContractFeePerKb(terms["maxContractFeeRate"].toDouble());

    if(!terms["maxLock"].isDouble())
        throw std::runtime_error("expecting json number for maxLock");

    buyerTerms.setMaxLock(terms["maxLock"].toDouble());

    if(!terms["maxPrice"].isDouble())
        throw std::runtime_error("expecting json number for maxPrice");

    buyerTerms.setMaxPrice(terms["maxPrice"].toDouble());

    if(!terms["minSellers"].isDouble())
        throw std::runtime_error("expecting json number for minSellers");

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

    if(!policy["minTimeBeforeBuildingContract"].isDouble())
        throw std::runtime_error("expected json number for minTimeBeforeBuildingContract");
    double minTimeBeforeBuildingContract = policy["minTimeBeforeBuildingContract"].toDouble();

    if(!policy["servicingPieceTimeOutLimit"].isDouble())
        throw std::runtime_error("expected json number for servicingPieceTimeOutLimit");

    double servicingPieceTimeOutLimit = policy["servicingPieceTimeOutLimit"].toDouble();

    if(!policy["sellerTermsOrderingPolicy"].isDouble())
        throw std::runtime_error("expected json number for sellerTermsOrderingPolicy");

    int sellerTermsOrderingPolicy = policy["sellerTermsOrderingPolicy"].toInt();

    return protocol_session::BuyingPolicy(minTimeBeforeBuildingContract,
                                          servicingPieceTimeOutLimit,
                                          (protocol_wire::SellerTerms::OrderingPolicy)sellerTermsOrderingPolicy);
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

    if(!terms["maxSellers"].isDouble())
        throw std::runtime_error("expecting json number for maxSellers");

    sellerTerms.setMaxSellers(terms["maxSellers"].toInt());

    if(!terms["minContractFeeRate"].isDouble())
        throw std::runtime_error("expecting json number for minContractFeeRate");

    sellerTerms.setMinContractFeePerKb(terms["minContractFeeRate"].toDouble());

    if(!terms["minLock"].isDouble())
        throw std::runtime_error("expecting json number for minLock");

    sellerTerms.setMinLock(terms["minLock"].toInt());

    if(!terms["minPrice"].isDouble())
        throw std::runtime_error("expecting json number for minPrice");

    sellerTerms.setMinPrice(terms["minPrice"].toDouble());

    if(!terms["settlementFee"].isDouble())
        throw std::runtime_error("expecting json number for settlementFee");

    sellerTerms.setSettlementFee(terms["settlementFee"].toDouble());

    return sellerTerms;
}

QJsonValue sellingPolicyToJson(const protocol_session::SellingPolicy &sellingPolicy) {
    return QJsonObject();
}

protocol_session::SellingPolicy jsonToSellingPolicy(const QJsonValue &value) {
    if(!value.isObject())
        throw std::runtime_error("expecting json object");

    return protocol_session::SellingPolicy();
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
