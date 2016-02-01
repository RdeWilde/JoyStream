/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 26 2016
 */

#include <blockcypher/Event.hpp>

#include <QDebug>

namespace BlockCypher {

    Event::Type Event::stringToType(const QString &s) {

        if(s == "unconfirmed-tx")        return Event::Type::unconfirmed_tx;
        else if(s == "new-block")        return Event::Type::new_block;
        else if(s == "confirmed-tx")     return Event::Type::confirmed_tx;
        else if(s == "tx-confirmation")  return Event::Type::tx_confirmation;
        else if(s == "double-spend-tx")  return Event::Type::double_spend_tx;
        else if(s == "tx-confidence")    return Event::Type::tx_confidence;
        else if(s == "pong")             return Event::Type::pong;
        else Q_ASSERT(false);

    }

    const char *Event::typeToString(Event::Type type) {

        switch(type) {
            case Event::Type::unconfirmed_tx: return "unconfirmed-tx";
            case Event::Type::new_block: return "new-block";
            case Event::Type::confirmed_tx: return "confirmed-tx";
            case Event::Type::tx_confirmation: return "tx-confirmation";
            case Event::Type::double_spend_tx: return "double-spend-tx";
            case Event::Type::tx_confidence: return "tx-confidence";
            case Event::Type::pong: return "pong";
            default: Q_ASSERT(false);
        }
    }

    Event::Type Event::getPayloadType(const QJsonObject & o) {

        if(o.contains("event")){
           if(o.value("event").toString() == "pong") return Event::Type::pong;
           throw std::runtime_error("unkown event type in payload");
        }

        // Block has 'height', 'depth', and 'chain' properties (TX doesn't)
        // if any of these properties is in the response then it must be a Block
        if(o.contains("height")) {
            return Event::Type::new_block;
        }

        int confirmations = o.value("confirmations").toInt();

        if(confirmations > 0) {
            return (confirmations == 1 ? Event::Type::confirmed_tx : Event::Type::tx_confirmation);
        } else {
            //all TX responses contain a confidence property
            //even if we don't filter on tx_confidence event type so there isn't really a way to determine
            //if this response was due specifically to a tx_confidence event
            if(o.value("double_spend").toBool() || o.contains("double_spend_tx")) {
                return Event::Type::double_spend_tx;
            }

            return Event::Type::unconfirmed_tx;
        }
    }

    Event::Event() {
        (*this)["event"] = typeToString(Event::Type::confirmed_tx);
    }

    Event::Event(Event::Type type) {
        (*this)["event"] = typeToString(type);

        //set default confirmations
        if(type == Event::Type::tx_confirmation) {
            (*this)["confirmations"] = 2;
        }

        //set default confidence level
        if(type == Event::Type::tx_confidence) {
            (*this)["confidence"] = 0.99;
        }
    }

    Event::Event(Event::Type type, const int & confirmations)
    {
        if(type != Event::Type::tx_confirmation) {
            throw std::runtime_error("expected tx_confirmation event type");
        }

        (*this)["event"] = typeToString(type);

        if(confirmations < 1) {
            (*this)["confirmations"] = 2;
        } else if (confirmations > 10) {
            (*this)["confirmations"] = 10;
        } else {
            (*this)["confirmations"] = confirmations;
        }
    }

    Event::Event(Event::Type type, const float & confidence) {
        if(type != Event::Type::tx_confirmation) {
            throw std::runtime_error("expected tx_confidence event type");
        }

        (*this)["event"] = typeToString(type);

        if(confidence > 1) {
            (*this)["confidence"] = 0.99;
        } else if (confidence < 0 ) {
            (*this)["confidence"] = 0.01;
        } else {
            (*this)["confidence"] = confidence;
        }
    }

    void Event::setAddress(const QString & address) {
        if(!address.isNull()) {
            (*this)["address"] = address;
        } else {
            this->remove("address");
        }
    }

    void Event::setScriptType(ScriptType script) {
        (*this)["script"] = fromScriptType(script);
    }

    void Event::removeScriptType() {
        this->remove("script");
    }

    void Event::setHash(const QString & hash) {
        if(!hash.isNull()) {
            (*this)["hash"] = hash;
        }else {
            this->remove("hash");
        }
    }

    void Event::setWalletName(const QString & wallet_name) {
        if(!wallet_name.isNull()) {
            (*this)["wallet_name"] = wallet_name;
        } else {
            this->remove("wallet_name");
        }
    }

    Event::Type Event::type() const {
        return stringToType((*this)["event"].toString());
    }
}
