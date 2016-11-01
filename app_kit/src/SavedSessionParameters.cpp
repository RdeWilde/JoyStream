#include <app_kit/SavedSessionParameters.hpp>
#include <app_kit/HelperFunctions.hpp>

#include <core/Session.hpp>
#include <core/Buying.hpp>
#include <core/Selling.hpp>

namespace joystream {
namespace appkit {

SavedSessionParameters::SavedSessionParameters()
{

}

SavedSessionParameters::SavedSessionParameters(const core::TorrentPlugin* plugin)
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

QJsonValue SavedSessionParameters::toJson() const {
    QJsonObject state;

    state["state"] = util::sessionStateToJson(_state);

    switch(_mode) {
        case protocol_session::SessionMode::buying: {
            QJsonObject buying;
            buying["terms"] = util::buyerTermsToJson(_buyerTerms);
            buying["policy"] = util::buyingPolicyToJson(_buyingPolicy);

            state["buying"] = buying;
            break;
        }
        case protocol_session::SessionMode::selling: {
            QJsonObject selling;
            selling["terms"] = util::sellerTermsToJson(_sellerTerms);
            selling["policy"] = util::sellingPolicyToJson(_sellingPolicy);

            state["selling"] = selling;
            break;
        }
        case protocol_session::SessionMode::observing: {
            // simple empty object (not null)
            state["observing"] = QJsonObject();
            break;
        }
    case protocol_session::SessionMode::not_set: {
            // do not add any key to the state object
        }

    }

    return state;
}

SavedSessionParameters::SavedSessionParameters(const QJsonValue &value) {
    if(!value.isObject())
        throw std::runtime_error("expecting json object value");

    QJsonObject state = value.toObject();

    _state = util::jsonToSessionState(state["state"]);

    QJsonValue buying = state["buying"];
    QJsonValue selling = state["selling"];
    QJsonValue observing = state["observing"];

    if(!buying.isNull()) {
        if(!buying.isObject())
            throw std::runtime_error("expecting json object value");

        _mode = protocol_session::SessionMode::buying;
        _buyerTerms = util::jsonToBuyerTerms(buying.toObject()["terms"]);
        _buyingPolicy = util::jsonToBuyingPolicy(buying.toObject()["policy"]);

    } else if(!selling.isNull()) {
        if(!selling.isObject())
            throw std::runtime_error("expecting json object value");

        _mode = protocol_session::SessionMode::selling;
        _sellerTerms = util::jsonToSellerTerms(selling.toObject()["terms"]);
        _sellingPolicy = util::jsonToSellingPolicy(selling.toObject()["policy"]);

    } else if(!observing.isNull()) {
        _mode = protocol_session::SessionMode::observing;
    } else {
        _mode = protocol_session::SessionMode::not_set;
    }
}

protocol_session::SessionMode SavedSessionParameters::mode() const {
    return _mode;
}

protocol_session::SessionState SavedSessionParameters::state() const {
    return _state;
}

protocol_wire::BuyerTerms SavedSessionParameters::buyerTerms() const {
    if(_mode == protocol_session::SessionMode::buying)
        return _buyerTerms;

    throw protocol_session::exception::ModeIncompatibleOperation();
}

protocol_wire::SellerTerms SavedSessionParameters::sellerTerms() const {
    if(_mode == protocol_session::SessionMode::selling)
        return _sellerTerms;

    throw protocol_session::exception::ModeIncompatibleOperation();
}

protocol_session::BuyingPolicy SavedSessionParameters::buyingPolicy() const {
    if(_mode == protocol_session::SessionMode::buying)
        return _buyingPolicy;

    throw protocol_session::exception::ModeIncompatibleOperation();
}

protocol_session::SellingPolicy SavedSessionParameters::sellingPolicy() const {
    if(_mode == protocol_session::SessionMode::selling)
        return _sellingPolicy;

    throw protocol_session::exception::ModeIncompatibleOperation();
}

}
}
