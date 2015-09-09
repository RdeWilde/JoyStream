/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <blockcypher/Client.hpp>
#include <blockcypher/Wallet.hpp>
#include <blockcypher/BlockCypher.hpp>
#include <blockcypher/CreateWallet.hpp>
#include <blockcypher/GetWallet.hpp>

#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QEventLoop>

namespace BlockCypher {

Client::Client(QNetworkAccessManager * manager, Coin::Network network, const QString & token)
    : _manager(manager)
    , _network(network)
    , _endPoint(endPoint(_network))
    , _token(token) {
}

CreateWallet::Reply * Client::createWalletAsync(const Wallet & requested) {

    // Make POST request
    QNetworkReply * reply = post("wallets?token=" + QString(BLOCKCYPHER_TOKEN), requested.toJson());

    // Return reply manager
    return new CreateWallet::Reply(reply, requested);
}

Wallet Client::createWallet(const Wallet & requested) {

    CreateWallet::Reply * reply = Client::createWalletAsync(requested);

    // Block until we have reply finished
    QEventLoop eventloop;
    QObject::connect(reply,
                     &CreateWallet::Reply::done,
                     &eventloop,
                     &QEventLoop::quit);
    eventloop.exec();

    CreateWallet::BlockCypherResponse r = reply->response();

    if(r == CreateWallet::BlockCypherResponse::Created)
        return reply->created();
    else
        throw std::runtime_error(""); // later add message here, e.g. for different scenarios
}

GetWallet::Reply * Client::getWalletAsync(const QString & name) {

    // Make GET request
    QNetworkReply * reply = get("wallets/" + name + "?token=" + QString(BLOCKCYPHER_TOKEN));

    // Return reply manager
    return new GetWallet::Reply(reply, name);

}

Wallet Client::getWallet(const QString & name) {

    GetWallet::Reply * reply = Client::getWalletAsync(name);

    // Block until we have reply finished
    QEventLoop eventloop;
    QObject::connect(reply,
                     &GetWallet::Reply::done,
                     &eventloop,
                     &QEventLoop::quit);
    eventloop.exec();

    GetWallet::BlockCypherResponse r = reply->response();

    if(r == GetWallet::BlockCypherResponse::Returned)
        return reply->wallet();
    else
        throw std::runtime_error(""); // later add message here, e.g. for different scenarios
}

void Client::addAddress(QNetworkRequest * request, const QString & name, const QList<Coin::P2PKHAddress> & addresses) {

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

QNetworkReply * Client::post(const QString & url, const QJsonObject & data) {

    // Create url and corresponding request
    QUrl qurl(_endPoint + url);
    QNetworkRequest request(qurl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Create and serialize payload
    QJsonDocument doc(data);
    QByteArray payload = doc.toJson();

    // Make post request
    return _manager->post(request, payload);
}

QNetworkReply * Client::get(const QString & url) {

    // Create url and corresponding request
    QUrl qurl(_endPoint + url);
    QNetworkRequest request(qurl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    return _manager->get(request);
}

}
