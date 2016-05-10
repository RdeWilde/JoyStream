/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 26 2016
 */

#include <blockcypher/Event.hpp>
#include <common/TransactionId.hpp>

#include <QDebug>

namespace BlockCypher {

    Event::Type Event::stringToType(const QString &s) {

        if(s == "unconfirmed-tx")        return Event::Type::unconfirmed_tx;
        else if(s == "new-block")        return Event::Type::new_block;
        else if(s == "confirmed-tx")     return Event::Type::confirmed_tx;
        else if(s == "tx-confirmation")  return Event::Type::tx_confirmation;
        else if(s == "double-spend-tx")  return Event::Type::double_spend_tx;
        else if(s == "tx-confidence")    return Event::Type::tx_confidence;
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
            default: Q_ASSERT(false);
        }
    }

    Event::Event(Event::Type type)
    {
        _event = type;
    }

    Event Event::make(Event::Type type) {
        if(type == Event::Type::tx_confirmation) {
            throw std::runtime_error("wrong event factory. use makeTxConfirmation() instead");
        }

        if(type == Event::Type::tx_confidence) {
            throw std::runtime_error("wrong event factory. use makeTxConfidence instead");
        }

        Event ev(type);
        return ev;
    }

    Event Event::makeTxConfirmation(int confirmations)
    {
        Event ev(Event::Type::tx_confirmation);

        if(confirmations < 0) {
            ev.setConfirmations(0);
        } else if (confirmations > 10) {
            ev.setConfirmations(10);
        } else {
            ev.setConfirmations(confirmations);
        }

        return ev;
    }

    Event Event::makeTxConfirmation(int confirmations, Coin::TransactionId txid)
    {
        Event e = makeTxConfirmation(confirmations);
        e.setHash(txid.toHex());
        return e;
    }

    Event Event::makeTxConfirmation(int confirmations, QString address)
    {
        Event e = makeTxConfirmation(confirmations);
        e.setAddress(address);
        return e;
    }

    Event Event::makeTxConfidence(double confidence, QString address) {
        Event ev(Event::Type::tx_confidence);

        if(confidence > 1) {
            ev.setConfidence(0.99);
        } else if (confidence < 0 ) {
            ev.setConfidence(0.01);
        } else {
            ev.setConfidence(confidence);
        }

        ev.setAddress(address);
        return ev;
    }

    void Event::setAddress(const QString & address) {
        if(_address) return;
        _address = address;
    }

    void Event::setScriptType(ScriptType script) {
        if(_script) return;
        _script = script;
    }

    void Event::setHash(const QString & hash) {
        if(_hash) return;
        _hash = hash;
    }

    void Event::setWalletName(const QString & wallet_name) {
        if(_wallet_name) return;
        _wallet_name = wallet_name;
    }

    void Event::setToken(const QString & token) {
        if(_token) return;
        _token = token;
    }

    void Event::setConfirmations(const int & confirmations) {
        if(_confirmations) return;
        _confirmations = confirmations;
    }

    void Event::setConfidence(const double & confidence) {
        if(_confidence) return;
        _confidence = confidence;
    }

    Event::Type Event::type() const {
        return _event;
    }

    QJsonObject Event::toJson() const {
        QJsonObject obj;

        obj["event"] = typeToString(_event);
        if(_confirmations) obj["confirmations"] = *_confirmations;
        if(_confidence) obj["confidence"] = *_confidence;
        if(_hash) obj["hash"] = *_hash;
        if(_address) obj["address"] = *_address;
        if(_script) obj["script"] = fromScriptType(*_script);
        if(_wallet_name) obj["wallet_name"] = *_wallet_name;
        if(_token) obj["token"] = *_token;

        return obj;
    }

}
