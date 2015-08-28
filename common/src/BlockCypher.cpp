/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <common/BlockCypher.hpp>

#include <QJsonArray>
#include <QNetworkRequest>

BlockCypher::Wallet::Wallet(const QString & token, const QString & name, const QList<Coin::P2PKHAddress> & addresses)
    : _token(token)
    , _name(name)
    , _addresses(addresses) {
}

BlockCypher::Wallet::Wallet(const QJsonObject & json) {

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

QJsonObject BlockCypher::Wallet::toJson() const {

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

void BlockCypher::getWallet(QNetworkRequest * request, const QString & name) {

    // Example get wallet with name "alice": curl https://api.blockcypher.com/v1/btc/main/wallets/alice?token=YOURTOKEN

    // Create url
    QString url = _endPoint + "wallets/" + name + "?token=" + QString(BLOCKCYPHER_TOKEN);

    // Set url on request
    request->setUrl(url);
}

void BlockCypher::addAddress(QNetworkRequest * request, const QString & name, const QList<Coin::P2PKHAddress> & addresses) {

    // Example get wallet with name "alice": curl https://api.blockcypher.com/v1/btc/main/wallets/alice?token=YOURTOKEN
    // /wallets/$NAME/addresses	POST	Wallet	Wallet

    // Create url
    QString url = _endPoint + "wallets/" + name + "/addresses?token=" + QString(BLOCKCYPHER_TOKEN);

    // Create wallet with
    Wallet wallet(_token, name, addresses);

    wallet.toJson()

            QByteArray payload = QJsonDocument(RPCJson).toJson();

    QByteArray payload()




    // Set url on request
    request->setUrl(url);
}

void BlockCypher::pushRawTransaction(const QString & rawTransaction) {

    // * Resource     Method	Request Object	Return Object
    // * /txs/push    POST	{“tx”:$TXHEX}	TX

}

QString BlockCypher::endPoint(Coin::Network network) {

    if(network == Coin::Network::mainnet)
        return QString(BLOCKCYPHER_MAINNET_ENDPOINT);
    else if(network == Coin::Network::testnet3)
        return QString(BLOCKCYPHER_TESTNET3_ENDPOINT);

    Q_ASSERT(false);
}

BlockCypher::BlockCypher(Coin::Network network, const QString & token)
    : _network(network)
    , _endPoint(endPoint(_network))
    , _token(token) {

}
