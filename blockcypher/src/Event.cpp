/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 26 2016
 */

#include <blockcypher/Event.hpp>

#include <QDebug>

namespace BlockCypher {

    Event::Event() {}

    Event::Event(const QString & event, const QString &token)
        : _event(toType(event)), _token(token) {

    }

    //tx-confidence event. Note, token is optional.
    Event::Event(const QString & event,const float &c, const QString &adr, const QString &token )
        : _event(toType(event)),
          _address(adr),
          _confidence(c),
          _token(token) {
    }

    //tx-confirmation event. Note, token is optional.
    Event::Event(const QString & event,const QString &adr,const unsigned short int &conf, const QString &token)
        : _event(toType(event)),
          _address(adr),
          _confirmations(conf),
          _token(token) {
    }


    Event::Type Event::toType(const QString &s) const {

        if(s == "unconfirmed-tx")        return Event::Type::unconfirmed_tx;
        else if(s == "new-block")        return Event::Type::new_block;
        else if(s == "confirmed-tx")     return Event::Type::confirmed_tx;
        else if(s == "tx-confirmation")  return Event::Type::tx_confirmation;
        else if(s == "double-spend-tx")  return Event::Type::double_spend_tx;
        else if(s == "tx-confidence")    return Event::Type::tx_confidence;
        else Q_ASSERT(false);

    }

    QString   Event::fromType(Event::Type type) const {

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

    Event::Type Event::getPayloadType(const QJsonObject & o) {
        throw std::runtime_error("not implemented");
    }

    //Converts Event object to JSON.
    QJsonObject Event::toJson() const{

        QJsonObject json_data;

        json_data["event"] = Event::fromType(_event);


        if(!_token.isEmpty())
            json_data["token"] = _token;

        //We only need event member
        if(_event == Event::Type::unconfirmed_tx ||
                _event == Event::Type::new_block ||
                _event == Event::Type::confirmed_tx ||
                _event == Event::Type::double_spend_tx) {
            return json_data;
        }

        //Remaining possible events tx-confirmation and tx-confidence.

        if(_event == Event::Type::tx_confirmation) {
            json_data["confirmations"] = _confirmations;
        }

        if(_event == Event::Type::tx_confidence) {
            json_data["confidence"] = _confidence;
        }

        json_data["address"] = _address;

        return json_data;
    }

}
