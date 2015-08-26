/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/BlockCypher.hpp>
#include <common/Network.hpp>

#include <QJsonArray>

namespace BlockCypher {

const char * endPoint(Coin::Network network) {

    if(network == Coin::Network::mainnet)
        return BLOCKCYPHER_MAINNET_ENDPOINT;
    else if(network == Coin::Network::testnet3)
        return BLOCKCYPHER_TESTNET3_ENDPOINT;

    Q_ASSERT(false);
}

Wallet::Wallet(const QString & token, const QString & name, const QList<Coin::P2PKHAddress> & addresses)
    : _token(token)
    , _name(name)
    , _addresses(addresses) {
}

Wallet::Wallet(const QJsonObject & json) {

    Q_ASSERT(json.contains("token"));
    QJsonValue tokenVal = json["token"];
    Q_ASSERT(tokenVal.isString());
    _token = tokenVal.toString();

    Q_ASSERT(json.contains("name"));
    QJsonValue nameVal = json["name"];
    Q_ASSERT(nameVal.isString());
    _name = nameVal.toString();

    Q_ASSERT(json.contains("addresses"));
    QJsonValue addressesVal = json["addresses"];
    Q_ASSERT(addressesVal.isArray());
    QJsonArray addressesArray = addressesVal.toArray();

    for(QJsonArray::const_iterator i = addressesArray.constBegin(),
        end = addressesArray.constEnd();
        i != end;
        i++) {

        QJsonValue elementVal = *i;

        Q_ASSERT(elementVal.isString());
        QString addressString = elementVal.toString();

        // Create addresss
        Coin::P2PKHAddress address = Coin::P2PKHAddress::fromBase58CheckEncoding(addressString);

        _addresses.push_back(address);
    }
}

QJsonObject Wallet::toJson() const {

    QJsonArray addressesArray;

    for(QList<Coin::P2PKHAddress>::const_iterator i = _addresses.constBegin(),
        end = _addresses.constEnd();
        i != end;
        i++) {

        Coin::P2PKHAddress address = *i;

        // Encode as (base58checkencoded) string, and add to array
        addressesArray.push_back(address.toBase58CheckEncoding());
    }

    return QJsonObject {
        {"token", _token},
        {"name", _name},
        {"addresses", addressesArray}
    };
}

void getWallet(QNetworkRequest * request, const QString & name) {

}

void addAddress(QNetworkRequest * request, const QString & name, const Coin::P2PKHAddress & address) {

}

void pushRawTransaction(const QString & rawTransaction) {

}

}
