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

    Event::Event() {}

    Event::Event(Event::Type type)
    {
        (*this)["event"] = typeToString(type);
    }

    Event Event::makeTxConfirmation(int confirmations)
    {
        Event ev(Event::Type::tx_confirmation);

        if(confirmations < 1) {
            ev["confirmations"] = 2;
        } else if (confirmations > 10) {
            ev["confirmations"] = 10;
        } else {
            ev["confirmations"] = confirmations;
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
            ev["confidence"] = 0.99;
        } else if (confidence < 0 ) {
            ev["confidence"] = 0.01;
        } else {
            ev["confidence"] = confidence;
        }

        ev.setAddress(address);
        return ev;
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
