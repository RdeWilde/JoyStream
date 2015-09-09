/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <blockcypher/Client.hpp>
#include <blockcypher/Wallet.hpp>
#include <blockcypher/CreateWallet.hpp>
#include <blockcypher/BlockCypher.hpp>

#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QEventLoop>

namespace BlockCypher {

CreateWallet::Reply * Client::createWalletAsync(const Wallet & requested) const {

    // Create url and corresponding request
    QUrl url(_endPoint + "wallets?token=" + QString(BLOCKCYPHER_TOKEN));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Create and serialize payload
    QJsonObject requestedJSON(requested.toJson());

    // Serialize
    QJsonDocument doc(requestedJSON);
    QByteArray payload = doc.toJson();

    // Make post request
    QNetworkReply * reply = _manager->post(request, payload);

    // Return reply manager
    return new CreateWallet::Reply(reply, requested);
}

Wallet Client::createWallet(const Wallet & requested) const {

    CreateWallet::Reply * reply = Client::createWalletAsync(requested);

    // Block until we have reply finished
    block(reply);

    if(reply->reply()->error() != QNetworkReply::NetworkError::NoError)
        throw std::runtime_error("There was some error");

    // Return response wallet
    return reply->response();
}

void Client::getWallet(QNetworkRequest * request, const QString & name) const {

    // Create url
    QString url = _endPoint + "wallets/" + name + "?token=" + QString(BLOCKCYPHER_TOKEN);

    // Set url on request
    request->setUrl(url);
}

Wallet Client::getWallet(const QString & name) const {

}

void Client::addAddress(QNetworkRequest * request, const QString & name, const QList<Coin::P2PKHAddress> & addresses) {

    // Example get wallet with name "alice":
    // curl https://api.blockcypher.com/v1/btc/main/wallets/alice/addresses?token=YOURTOKEN
    // /wallets/$NAME/addresses     POST	Wallet	Wallet

    // Create url
    QString url = _endPoint + "wallets/" + name + "/addresses?token=" + QString(BLOCKCYPHER_TOKEN);

    // Create wallet with
    Wallet wallet(_token, name, addresses);

    // Turn into wallet into json,
    QJsonObject json = wallet.toJson();

    // Turn json into string encoded payload
    QByteArray payload = QJsonDocument(json).toJson();

    // Set url on request
    request->setUrl(url);
    //request->se
}

void Client::pushRawTransaction(const QString & rawTransaction) {

    // * Resource     Method	Request Object	Return Object
    // * /txs/push    POST	{“tx”:$TXHEX}	TX

}

QString Client::endPoint(Coin::Network network) {

    if(network == Coin::Network::mainnet)
        return QString(BLOCKCYPHER_MAINNET_ENDPOINT);
    else if(network == Coin::Network::testnet3)
        return QString(BLOCKCYPHER_TESTNET3_ENDPOINT);

    Q_ASSERT(false);
}

Client::Client(QNetworkAccessManager * manager, Coin::Network network, const QString & token)
    : _manager(manager)
    , _network(network)
    , _endPoint(endPoint(_network))
    , _token(token) {
}

void Client::block(const BlockCypher::Reply * reply) const {

    QEventLoop eventloop;

    QObject::connect(reply->reply(),
                     SIGNAL(error(QNetworkReply::NetworkError)),
                     &eventloop,
                     SLOT(quit()));

    QObject::connect(reply->reply(),
                     SIGNAL(finished()),
                     &eventloop,
                     SLOT(quit()));

    // block until one of those signals are emitted
    eventloop.exec();
}

}
