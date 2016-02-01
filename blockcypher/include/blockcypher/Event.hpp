/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 26 2016
 */

#ifndef BLOCKCYPHER_EVENT_HPP
#define BLOCKCYPHER_EVENT_HPP

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

#include <blockcypher/ScriptType.hpp>

namespace BlockCypher {


#define MAX_CONFIRMATIONS_ALLOWED 10
#define MIN_CONFIDENCE 0
#define MAX_CONFIDENCE 1

class Event : public QJsonObject {

public:

    // event types.
    enum class Type {
        unconfirmed_tx,
        new_block,
        confirmed_tx,
        tx_confirmation,
        double_spend_tx,
        tx_confidence,
        pong //we only receive pong events, we should not send them
    };

    // Determine event Type from payload
    static Type getPayloadType(const QJsonObject & o);

    // Convernt between enum Type and string representation
    static Event::Type stringToType(const QString &s);
    static const char * typeToString(Event::Type type);

    // default constructor sets event type to confirmed_tx
    Event();

    // double_spend_tx, confirmed_tx, unconfirmed_tx, new_block,
    // tx_confidence (with default confidence level 0.99)
    // tx_confirmation (with default confirmations = 2)
    Event (Event::Type type);

    // tx_confidence
    Event (Event::Type type, const float & confidence);

    // tx_confirmation
    Event(Event::Type type, const int & confirmations);


    // add optional keys with setters

    // null string removes the key
    void setAddress(const QString & address);

    void setHash(const QString & hash);

    void setWalletName(const QString & wallet_name);

    // pass in ScriptType::none to remove
    void setScriptType(ScriptType script);

    void removeScriptType();

    // return event type
    Event::Type type() const;

};

}

#endif // BLOCKCYPHER_EVENT_HPP
