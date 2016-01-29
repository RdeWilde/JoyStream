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

    Event::Event(Event::Type type,
      const QString & token,
      const QString & address,
      const ScriptType & script,
      const QString & hash,
      const QString & wallet_name)
        : _event(type), _hash(hash),
          _wallet_name(wallet_name), _token(token),
          _address(address), _script(script)
    {}

    Event::Event(Event::Type type,
      const int & confirmations,
      const QString & token,
      const QString & address,
      const ScriptType & script,
      const QString & hash,
      const QString & wallet_name)
        : _event(type), _hash(hash),
          _wallet_name(wallet_name), _token(token),
          _address(address), _script(script),
          _confirmations(confirmations)
    {
        if(_confirmations < 1) _confirmations = 2;
        if(_confirmations > 10) _confirmations = 10;
    }

    Event::Event(Event::Type type,
      const float & confidence,
      const QString & token,
      const QString & address,
      const ScriptType & script,
      const QString & hash,
      const QString & wallet_name)
        : _event(type), _hash(hash),
          _wallet_name(wallet_name), _token(token),
          _address(address), _script(script),
          _confidence(confidence)
    {
        if(_confidence > 1) _confidence = 0.99;
        if(_confidence < 0) _confidence = 0.01;
    }

    Event::Type Event::getPayloadType(const QJsonObject & o) {

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

    //Converts Event object to JSON.
    QJsonObject Event::toJson() const {

        if(_event == Event::Type::none) throw std::runtime_error("Event type not set");

        QJsonObject json_data;

        json_data["event"] = Event::typeToString(_event);

        if(!_token.isEmpty()) {
            json_data["token"] = _token;
        }

        if(!_wallet_name.isEmpty()) {
            json_data["wallet_name"] = _wallet_name;
        }

        if(!_hash.isEmpty()) {
            json_data["hash"] = _hash;
        }

        if(_script != ScriptType::none) {
            json_data["script"] = fromScriptType(_script);
        }

        if(_event == Event::Type::tx_confirmation) {
            json_data["confirmations"] = _confirmations;
        }

        if(_event == Event::Type::tx_confidence) {
            json_data["confidence"] = _confidence;
        }

        if(!_address.isEmpty()) {
            json_data["address"] = _address;
        }

        return json_data;
    }

}
