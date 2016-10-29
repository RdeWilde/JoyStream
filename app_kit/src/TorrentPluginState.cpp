#include <app_kit/TorrentPluginState.hpp>
#include <core/Session.hpp>
#include <core/Buying.hpp>
#include <core/Selling.hpp>

namespace joystream {
namespace appkit {

TorrentPluginState::TorrentPluginState()
{

}

TorrentPluginState::TorrentPluginState(const core::TorrentPlugin* plugin)
    : _mode(plugin->session()->mode()),
      _state(plugin->session()->state())
{
    core::Session *session = plugin->session();
    core::Buying *buying = session->buyingSet() ? session->buying() : nullptr;
    core::Selling *selling = session->sellingSet() ? session->selling() : nullptr;

    if (buying) {
        _buyerTerms = buying->terms();
        _buyingPolicy = buying->policy();
    } else if(selling) {
        _sellerTerms = selling->terms();
        _sellingPolicy = selling->policy();
    }
}

QJsonValue TorrentPluginState::toJson() const {
    QJsonObject state;

    switch(_state) {
        case protocol_session::SessionState::paused :
            state["state"] = QJsonValue("paused");
            break;
        case protocol_session::SessionState::started :
            state["state"] = QJsonValue("started");
            break;
        case protocol_session::SessionState::stopped :
            state["state"] = QJsonValue("stopped");
            break;
    }

    switch(_mode) {
        case protocol_session::SessionMode::buying: {
            QJsonObject buying;
            QJsonObject terms;
            QJsonObject policy;

            terms["maxContractFeeRate"] = QJsonValue((int64_t)_buyerTerms.maxContractFeePerKb());
            terms["maxLock"] = QJsonValue((int64_t)_buyerTerms.maxLock());
            terms["maxPrice"] = QJsonValue((int64_t)_buyerTerms.maxPrice());
            terms["minSellers"] = QJsonValue((int64_t)_buyerTerms.minNumberOfSellers());
            buying["terms"] = terms;

            policy["minTimeBeforeBuildingContract"] = QJsonValue((int64_t)_buyingPolicy.minTimeBeforeBuildingContract().count());
            policy["servicingPieceTimeOutLimit"] = QJsonValue((int64_t)_buyingPolicy.servicingPieceTimeOutLimit().count());
            policy["sellerTermsOrderingPolicy"] = QJsonValue((int64_t)_buyingPolicy.sellerTermsOrderingPolicy());//safer to conver to a string?
            buying["policy"] = policy;

            state["buying"] = buying;
            break;
        }
        case protocol_session::SessionMode::selling: {
            QJsonObject selling;
            QJsonObject terms;

            terms["minContractFeeRate"] = QJsonValue((int64_t)_sellerTerms.minContractFeePerKb());
            terms["minLock"] = QJsonValue(_sellerTerms.minLock());
            terms["minPrice"] = QJsonValue((int64_t)_sellerTerms.minPrice());
            terms["maxSellers"] = QJsonValue((int64_t)_sellerTerms.maxSellers());
            terms["settlementFee"] = QJsonValue((int64_t)_sellerTerms.settlementFee());
            selling["terms"] = terms;

            selling["policy"] = QJsonObject();

            state["selling"] = selling;

            break;
        }
        case protocol_session::SessionMode::observing: {
            state["observing"] = QJsonValue(true);
            break;
        }

    }

    return state;
}

TorrentPluginState::TorrentPluginState(const QJsonValue &value) {
    if(!value.isObject())
        throw std::runtime_error("expecting json object value");

    QJsonObject state = value.toObject();

    if(!state["state"].isString()) {
        throw std::runtime_error("invalid state value");
    } else {
        const QString stateString = state["state"].toString();
        if(stateString == "started")
            _state = protocol_session::SessionState::started;
        if(stateString == "stopped")
            _state = protocol_session::SessionState::stopped;
        if(stateString == "paused")
            _state = protocol_session::SessionState::paused;
    }

    _mode = protocol_session::SessionMode::not_set;

    QJsonValue buying = state["buying"];
    QJsonValue selling = state["selling"];
    QJsonValue observing = state["observing"];

    if(!buying.isNull() && buying.isObject()) {

        _mode = protocol_session::SessionMode::buying;
        QJsonObject objBuying = buying.toObject();

        QJsonObject terms = objBuying["terms"].toObject();
        _buyerTerms.setMaxContractFeePerKb(terms["maxContractFeeRate"].toDouble());
        _buyerTerms.setMaxLock(terms["maxLock"].toDouble());
        _buyerTerms.setMaxPrice(terms["maxPrice"].toDouble());
        _buyerTerms.setMinNumberOfSellers(terms["minSellers"].toDouble());

        QJsonObject policy = objBuying["policy"].toObject();
        _buyingPolicy = protocol_session::BuyingPolicy(policy["minTimeBeforeBuildingContract"].toDouble(),
                policy["servicingPieceTimeOutLimit"].toDouble(), (protocol_wire::SellerTerms::OrderingPolicy)policy["sellerTermsOrderingPolicy"].toInt());

    } else if(!selling.isNull() && selling.isObject()) {

        _mode = protocol_session::SessionMode::selling;
        QJsonObject objSelling = selling.toObject();
        QJsonObject terms = objSelling["terms"].toObject();
        _sellerTerms.setMaxSellers(terms["maxSellers"].toInt());
        _sellerTerms.setMinContractFeePerKb(terms["minContractFeeRate"].toDouble());
        _sellerTerms.setMinLock(terms["minLock"].toInt());
        _sellerTerms.setMinPrice(terms["minPrice"].toDouble());
        _sellerTerms.setSettlementFee(terms["settlementFee"].toDouble());

    } else if(!observing.isNull()) {
        _mode = protocol_session::SessionMode::observing;
    }
}

protocol_session::SessionMode TorrentPluginState::mode() const {
    return _mode;
}

protocol_session::SessionState TorrentPluginState::state() const {
    return _state;
}

// Use the internal state information and attempt to transition a plugin to that state
void TorrentPluginState::applyTo(core::TorrentPlugin* plugin /*, callback */) const {
    //....
}

protocol_wire::BuyerTerms TorrentPluginState::buyerTerms() const {
    if(_mode == protocol_session::SessionMode::buying)
        return _buyerTerms;

    assert(false);
}

protocol_wire::SellerTerms TorrentPluginState::sellerTerms() const {
    if(_mode == protocol_session::SessionMode::selling)
        return _sellerTerms;

    assert(false);
}

protocol_session::BuyingPolicy TorrentPluginState::buyingPolicy() const {
    if(_mode == protocol_session::SessionMode::buying)
        return _buyingPolicy;

    assert(false);
}

protocol_session::SellingPolicy TorrentPluginState::sellingPolicy() const {
    if(_mode == protocol_session::SessionMode::selling)
        return _sellingPolicy;

    assert(false);
}

}
}
