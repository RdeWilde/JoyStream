/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef BLOCKCYPHER_CLIENT_HPP
#define BLOCKCYPHER_CLIENT_HPP

#include <common/P2PKHAddress.hpp>
#include <common/TransactionId.hpp>
#include <common/Network.hpp>

#include <QJsonObject>
#include <QString>
#include <QVector>
#include <QNetworkReply> // cannot forward declare

class QNetworkRequest;
class QNetworkReply;
class QNetworkAccessManager;

/**
 * http://dev.blockcypher.com/#restful-resources
 * =============================================
 * Bitcoin      Main        api.blockcypher.com/v1/btc/main
 * Bitcoin      Testnet3    api.blockcypher.com/v1/btc/test3
 * Dogecoin     Main        api.blockcypher.com/v1/doge/main
 * Litecoin     Main        api.blockcypher.com/v1/ltc/main
 * BlockCypher	Test        api.blockcypher.com/v1/bcy/test
 */

#define BLOCKCYPHER_MAINNET_ENDPOINT "https://api.blockcypher.com/v1/btc/main/"
#define BLOCKCYPHER_TESTNET3_ENDPOINT "https://api.blockcypher.com/v1/btc/test3/"
#define BLOCKCYPHER_TOKEN "aa10fe97a83259a4628d09b125bebf5a"

namespace BlockCypher {

    class Reply;

    namespace CreateWallet {
        class Reply;
    }

    struct Wallet;

    class Client {

    public:

        /**
         * CREATE WALLET
         */

        // Non blocking routine which creates wallet and returns corresponding reply object
        CreateWallet::Reply * createWalletAsync(const Wallet & requested) const;

        // Blocking routine which creates wallet, or throws exception if failed
        Wallet createWallet(const Wallet & requested) const;

        /**
         * GET WALLET
         */

        // GET query for getting wallet
        // http://dev.blockcypher.com/#get-wallet-endpoint
        void getWallet(QNetworkRequest * request, const QString & name) const;
        Wallet getWallet(const QString & name) const;

        /**
         * ADD ADDRESS
         */

        // GET query for adding address to wallet
        // http://dev.blockcypher.com/#add-addresses-to-wallet-endpoint
        void addAddress(QNetworkRequest * request, const QString & name, const QList<Coin::P2PKHAddress> & addresses);

        // Push Raw Transaction Endpoint (http://dev.blockcypher.com/#push-raw-transaction-endpoint)
        void pushRawTransaction(const QString & rawTransaction);

        // Endpoint for given network
        static QString endPoint(Coin::Network network);

        Client(QNetworkAccessManager * manager, Coin::Network network, const QString & token);

    private:

        // Blocks until error() or finished() signal arrives
        void block(const BlockCypher::Reply * reply) const;

        // Network access manager
        QNetworkAccessManager * _manager;

        // Network for which we are communicating with
        Coin::Network _network;

        // API endpoint corresponding to network
        QString _endPoint;

        // API token
        QString _token;
    };

}

#endif // BLOCKCYPHER_CLIENT_HPP

