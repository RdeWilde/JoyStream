/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

#include <app_kit/SavedSessionParameters.hpp>
#include <app_kit/HelperFunctions.hpp>

#include <core/Session.hpp>
#include <core/Buying.hpp>
#include <core/Selling.hpp>

namespace joystream {
namespace appkit {

SavedSessionParameters::SavedSessionParameters()
    : _mode(protocol_session::SessionMode::not_set),
      _state(protocol_session::SessionState::stopped)
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
    QJsonObject parameters;

    parameters["state"] = util::sessionStateToJson(_state);

    switch(_mode) {
        case protocol_session::SessionMode::buying: {
            QJsonObject buying;
            buying["terms"] = util::buyerTermsToJson(_buyerTerms);
            buying["policy"] = util::buyingPolicyToJson(_buyingPolicy);

            parameters["buying"] = buying;
            break;
        }
        case protocol_session::SessionMode::selling: {
            QJsonObject selling;
            selling["terms"] = util::sellerTermsToJson(_sellerTerms);
            selling["policy"] = util::sellingPolicyToJson(_sellingPolicy);

            parameters["selling"] = selling;
            break;
        }
        case protocol_session::SessionMode::observing: {
            // simple empty object (not null)
            parameters["observing"] = QJsonObject();
            break;
        }
        case protocol_session::SessionMode::not_set: {
            // do not add any key to the parameters object
        }

    }

    return parameters;
}

SavedSessionParameters::SavedSessionParameters(const QJsonValue &value) {
    if(!value.isObject())
        throw std::runtime_error("expecting json object for saved session parameters");

    QJsonObject parameters = value.toObject();

    if(!parameters["state"].isString())
        throw std::runtime_error("expecting json string for session state");

    _state = util::jsonToSessionState(parameters["state"]);

    QJsonValue buying = parameters["buying"];
    QJsonValue selling = parameters["selling"];
    QJsonValue observing = parameters["observing"];

    if(!buying.isNull()) {

        if(!selling.isNull() || !observing.isNull())
            throw std::runtime_error("cannot have more than one session state for torrent");

        if(!buying.isObject())
            throw std::runtime_error("expecting json object for buying");

        QJsonValue terms = buying.toObject()["terms"];
        QJsonValue policy = buying.toObject()["policy"];

        if(terms.isNull() || policy.isNull()) {
            throw std::runtime_error("missing terms or policy in buying parameters");
        }
        _mode = protocol_session::SessionMode::buying;
        _buyerTerms = util::jsonToBuyerTerms(terms);
        _buyingPolicy = util::jsonToBuyingPolicy(policy);

    } else if(!selling.isNull()) {

        if(!buying.isNull() || !observing.isNull())
            throw std::runtime_error("cannot have more than one session state for torrent");

        if(!selling.isObject())
            throw std::runtime_error("expecting json object for selling");

        QJsonValue terms = selling.toObject()["terms"];
        QJsonValue policy = selling.toObject()["policy"];

        if(terms.isNull() || policy.isNull()) {
            throw std::runtime_error("missing terms or policy in selling parameters");
        }

        _mode = protocol_session::SessionMode::selling;
        _sellerTerms = util::jsonToSellerTerms(terms);
        _sellingPolicy = util::jsonToSellingPolicy(policy);

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
