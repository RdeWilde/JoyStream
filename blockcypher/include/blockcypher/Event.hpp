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

namespace Coin {
    class TransactionId;
}

namespace BlockCypher {


#define MAX_CONFIRMATIONS_ALLOWED 10
#define MIN_CONFIDENCE 0
#define MAX_CONFIDENCE 1

class Event : public QJsonObject {

public:

    // event types.
    enum class Type {
        //we can optinally filter by hash, address or script type
        unconfirmed_tx,
        new_block,
        confirmed_tx,
        double_spend_tx,

        //require hash or address
        tx_confirmation, //address or transaction hash
        tx_confidence, //requires address

        pong //we only receive pong events, we should not send them
    };

    // Convernt between enum Type and string representation
    static Event::Type stringToType(const QString &s);
    static const char * typeToString(Event::Type type);

    Event();

    // double_spend_tx, confirmed_tx, unconfirmed_tx, new_block,
    Event (Event::Type type);

    static Event makeTxConfirmation(int confirmations, Coin::TransactionId txid);
    static Event makeTxConfirmation(int confirmations, QString address);
    static Event makeTxConfidence(double confidence, QString address);

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

private:
    static Event makeTxConfirmation(int confirmations);
};

}

#endif // BLOCKCYPHER_EVENT_HPP
