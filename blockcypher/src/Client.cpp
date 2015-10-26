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
#include <blockcypher/DeleteWallet.hpp>
#include <blockcypher/AddressEndPoint.hpp>
#include <blockcypher/PushRawTransaction.hpp>
#include <blockcypher/FundWalletFromFaucet.hpp>

#include <QJsonArray>
#include <QJsonObject>
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
    QNetworkReply * reply = post("wallets?token=" + _token, requested.toJson());

    // Return reply manager
    return new CreateWallet::Reply(reply, requested);
}

Wallet Client::createWallet(const Wallet & requested) {

    if(requested._addresses.isEmpty())
        throw std::runtime_error("Wallet object must have atleast one address when used to create wallets.");

    if(requested._token != _token)
        throw std::runtime_error("Token in wallet is not compatible client");

    CreateWallet::Reply * reply = Client::createWalletAsync(requested);

    // Block until we have reply finished
    QEventLoop eventloop;
    QObject::connect(reply,
                     &CreateWallet::Reply::done,
                     &eventloop,
                     &QEventLoop::quit);
    eventloop.exec();

    CreateWallet::BlockCypherResponse r = reply->response();

    if(r == CreateWallet::BlockCypherResponse::Created) {

        Q_ASSERT(reply->created() == requested);

        return reply->created();
    } else {

        // Create full error mesasge
        QString s = "Client::createWallet - " + reply->errorReport();

        // ADD PROPER EXCEPTION TYPE LATER
        throw std::runtime_error(s.toStdString());
    }
}

DeleteWallet::Reply * Client::deleteWalletAsync(const QString & name) {

    // Make GET request
    QNetworkReply * reply = deleteResource("wallets/" + name + "?token=" + _token);

    // Return reply manager
    return new DeleteWallet::Reply(reply, name);
}

void Client::deleteWallet(const QString & name) {

    DeleteWallet::Reply * reply = Client::deleteWalletAsync(name);

    // Block until we have reply finished
    QEventLoop eventloop;
    QObject::connect(reply,
                     &DeleteWallet::Reply::done,
                     &eventloop,
                     &QEventLoop::quit);
    eventloop.exec();

    DeleteWallet::BlockCypherResponse r = reply->response();

    if(r != DeleteWallet::BlockCypherResponse::Deleted) {

        // Create full error mesasge
        QString s = "Client::deleteWalletAsync - " + reply->errorReport();

        // ADD PROPER EXCEPTION TYPE LATER
        throw std::runtime_error(s.toStdString());
    }
}

GetWallet::Reply * Client::getWalletAsync(const QString & name) {

    // Make GET request
    QNetworkReply * reply = get("wallets/" + name + "?token=" + _token);

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

    if(r == GetWallet::BlockCypherResponse::Returned) {

        Q_ASSERT(reply->wallet()._name == name);

        return reply->wallet();
    } else {

        // Create full error mesasge
        QString s = "Client::getWallet - " + reply->errorReport();

        // ADD PROPER EXCEPTION TYPE LATER
        throw std::runtime_error(s.toStdString());
    }
}

bool Client::walletDoesNotExist(const QString & name){
    GetWallet::Reply * reply = Client::getWalletAsync(name);

    // Block until we have reply finished
    QEventLoop eventloop;
    QObject::connect(reply,
                     &GetWallet::Reply::done,
                     &eventloop,
                     &QEventLoop::quit);
    eventloop.exec();

    return (reply->response() == GetWallet::BlockCypherResponse::DoesNotExist);
}

AddAddressToWallet::Reply * Client::addAddressToWalletAsync(const Wallet & requested) {

    // Make GET request
    QNetworkReply * reply = post("wallets/" + requested._name + "/addresses?token=" + _token, requested.toJson());

    // Return reply manager
    return new AddAddressToWallet::Reply(reply, requested);
}


Wallet Client::addAddressToWallet(const Wallet & requested) {

    AddAddressToWallet::Reply * reply = Client::addAddressToWalletAsync(requested);

    // Block until we have reply finished
    QEventLoop eventloop;
    QObject::connect(reply,
                     &AddAddressToWallet::Reply::done,
                     &eventloop,
                     &QEventLoop::quit);
    eventloop.exec();

    AddAddressToWallet::BlockCypherResponse r = reply->response();

    if(r == AddAddressToWallet::BlockCypherResponse::Created) {

        Wallet created = reply->created();

        // assert that what was requested is inside what was created
        Q_ASSERT(requested._token == created._token);
        Q_ASSERT(requested._name == created._name);
        //Q_ASSERT(requested._addresses.toSet().intersect(created).contains())

        return created;
    } else {

        // Create full error mesasge
        QString s = "Client::addAddressToWallet - " + reply->errorReport();

        // ADD PROPER EXCEPTION TYPE LATER
        throw std::runtime_error(s.toStdString());
    }
}

AddressEndPoint::Reply * Client::addressEndPointAsync(const QString & walletName, bool unspentOnly, uint limit, uint confirmations) {

    QString basic_request_uri = "addrs/" + walletName + "?token=" + _token;

    if(unspentOnly)
        basic_request_uri += "&unspentOnly=true";

    if(limit != 50) // default value
        basic_request_uri += "&limit=" + limit;

    if(confirmations != -1) // indicates we don care
        basic_request_uri += "&confirmations=" + confirmations;

    // Make GET request
    QNetworkReply * reply = get(basic_request_uri);

    // Return reply manager
    return new AddressEndPoint::Reply(reply, walletName, unspentOnly, limit, confirmations);
}

Address Client::addressEndPoint(const QString & walletName, bool unspentOnly, uint limit, uint confirmations) {

    AddressEndPoint::Reply * reply = addressEndPointAsync(walletName, unspentOnly, limit, confirmations);

    // Block until we have reply finished
    QEventLoop eventloop;
    QObject::connect(reply,
                     &AddressEndPoint::Reply::done,
                     &eventloop,
                     &QEventLoop::quit);
    eventloop.exec();

    AddressEndPoint::BlockCypherResponse r = reply->response();

    if(r == AddressEndPoint::BlockCypherResponse::Fetched) {

        Q_ASSERT(reply->error() == QNetworkReply::NoError);

        Address fetched = reply->address();

        return fetched;
    }  else {

        // Create full error mesasge
        QString s = "Client::addressEndPoint - " + reply->errorReport();

        // ADD PROPER EXCEPTION TYPE LATER
        throw std::runtime_error(s.toStdString());
    }

}

PushRawTransaction::Reply * Client::pushRawTransactionAsync(const Coin::Transaction & toBeBroadcasted) {

    std::string txString = toBeBroadcasted.getSerialized().getHex();

    qDebug() << QString::fromStdString(txString);

    QJsonObject jsonifiedTx {
        {"tx", QString::fromStdString(txString)}
    };

    // Make GET request
    QNetworkReply * reply = post("txs/push?token=" + _token, jsonifiedTx);

    // Return reply manager
    return new PushRawTransaction::Reply(reply, toBeBroadcasted);
}

bool Client::pushRawTransaction(const Coin::Transaction & toBeBroadcasted) {

    PushRawTransaction::Reply * reply = pushRawTransactionAsync(toBeBroadcasted);

    // Block until we have reply finished
    QEventLoop eventloop;
    QObject::connect(reply,
                     &PushRawTransaction::Reply::done,
                     &eventloop,
                     &QEventLoop::quit);
    eventloop.exec();

    PushRawTransaction::BlockCypherResponse r = reply->response();

    if(r == PushRawTransaction::BlockCypherResponse::Broadcasted) {
        return true;
    } else {

        // Create full error mesasge
        QString s = "Client::pushRawTransaction - " + reply->errorReport();

        // ADD PROPER EXCEPTION TYPE LATER
        throw std::runtime_error(s.toStdString());
    }
}

FundWalletFromFaucet::Reply * Client::fundWalletFromFaucetAsync(QString address, quint64 amount) {

    QJsonObject request {
        {"address", address},
        {"amount", QString::number(amount).toDouble()}
    };

    // Make POST request
    QNetworkReply * reply = post("faucet?token=" + _token, request);

    // Return reply manager
    return new FundWalletFromFaucet::Reply(reply);
}

bool Client::fundWalletFromFaucet(QString address, quint64 amount) {

    FundWalletFromFaucet::Reply * reply = fundWalletFromFaucetAsync(address, amount);

    // Block until we have reply finished
    QEventLoop eventloop;
    QObject::connect(reply,
                     &FundWalletFromFaucet::Reply::done,
                     &eventloop,
                     &QEventLoop::quit);
    eventloop.exec();

    FundWalletFromFaucet::BlockCypherResponse r = reply->response();

    if(r == FundWalletFromFaucet::BlockCypherResponse::Funded) {
        qDebug() << "Wallet Funded";
        return true;
    } else {

        // Create full error mesasge
        QString s = "Client::fundWalletFromFaucet - " + reply->errorReport();
        qDebug() << s;
    }
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

QNetworkReply * Client::deleteResource(const QString & url) {

    // Create url and corresponding request
    QUrl qurl(_endPoint + url);
    QNetworkRequest request(qurl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    return _manager->deleteResource(request);
}

}
