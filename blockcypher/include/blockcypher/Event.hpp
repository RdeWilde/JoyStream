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
#include <boost/optional.hpp>

namespace Coin {
    class TransactionId;
}

namespace BlockCypher {

class Event {

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
        tx_confidence //requires address
    };

    // Convernt between enum Type and string representation
    static Event::Type stringToType(const QString &s);
    static const char * typeToString(Event::Type type);

    // double_spend_tx, confirmed_tx, unconfirmed_tx, new_block,
    static Event make(Event::Type type);

    // tx_confidence and tx_confirmation
    static Event makeTxConfirmation(int confirmations, Coin::TransactionId txid);
    static Event makeTxConfirmation(int confirmations, QString address);
    static Event makeTxConfidence(double confidence, QString address);

    // add optional keys with setters, can only be set once
    void setAddress(const QString & address);
    boost::optional<QString> getAddress() const { return _address; }

    void setHash(const QString & hash);
    boost::optional<QString> getHash() const { return _hash; }

    void setWalletName(const QString & wallet_name);
    boost::optional<QString> getWalletName() const { return _wallet_name; }

    void setScriptType(ScriptType script);
    boost::optional<ScriptType> getScriptType() const { return _script; }

    void setToken(const QString & token);
    boost::optional<QString> getToken() const { return _token; }

    // return event type
    Event::Type type() const;

    QJsonObject toJson() const;

private:
    //only factories can instantiate objects
    Event () {}
    Event (Event::Type type);

    static Event makeTxConfirmation(int confirmations);

    Event::Type _event;
    boost::optional<double> _confidence;
    boost::optional<int> _confirmations;
    boost::optional<QString> _address;
    boost::optional<QString> _hash;
    boost::optional<QString> _wallet_name;
    boost::optional<ScriptType> _script;
    boost::optional<QString> _token;

    // only applicable to tx_confidence and tx_confirmation events
    // used internall by event factory
    void setConfirmations(const int & confirmations);
    void setConfidence(const double & confidence);
};

}

#endif // BLOCKCYPHER_EVENT_HPP
