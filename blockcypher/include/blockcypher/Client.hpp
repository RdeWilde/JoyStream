/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef BLOCKCYPHER_CLIENT_HPP
#define BLOCKCYPHER_CLIENT_HPP

//#include <blockcypher/AddAddressToWallet.hpp> // can't forward declare typedef

#include <common/P2PKHAddress.hpp>
#include <common/TransactionId.hpp>
#include <common/Network.hpp>

#include <QJsonObject>
#include <QString>
#include <QVector>
#include <QNetworkReply> // cannot forward declare

namespace Coin {
    class Transaction;
}

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
//#define BLOCKCYPHER_TOKEN "aa10fe97a83259a4628d09b125bebf5a"

// comment out

namespace BlockCypher {

    class Reply;
    struct Wallet;
    struct Address;

    namespace CreateWallet {
        enum class BlockCypherResponse;
        class Reply;
    }

    namespace DeleteWallet {
        enum class BlockCypherResponse;
        class Reply;
    }

    namespace GetWallet {
        class Reply;
    }

    namespace AddAddressToWallet {
        typedef CreateWallet::BlockCypherResponse BlockCypherResponse;
        typedef CreateWallet::Reply Reply;
    }

    namespace AddressEndPoint {
        class Reply;
    }

    namespace PushRawTransaction {
        class Reply;
    }

    namespace FundWalletFromFaucet {
        class Reply;
    }

    class Client {

    public:

        Client(QNetworkAccessManager * manager, Coin::Network network, const QString & token);

        /**
         * CREATE WALLET
         */

        // Non-blocking routine which creates wallet and returns corresponding reply object
        CreateWallet::Reply * createWalletAsync(const Wallet & requested);

        // Blocking routine which creates wallet, or throws exception if failed
        Wallet createWallet(const Wallet & requested);

        /**
         * DELETE WALLET
         */

        // Non-blocking routine which deletes wallet and returns corresponding reply object
        DeleteWallet::Reply * deleteWalletAsync(const QString & name);

        // Blocking routine which deletes wallet, or throws exception if failed
        void deleteWallet(const QString & name);

        /**
         * GET WALLET
         */

        // Non-blocking routine which gets a wallet with given name and returns corresponding reply object
        GetWallet::Reply * getWalletAsync(const QString & name);

        // Blocking routine which creates wallet, or throws exception if failed
        Wallet getWallet(const QString & name);

        // Blocking routine tests if a wallet does NOT exist
        bool walletDoesNotExist(const QString & name);

        /**
         * ADD ADDRESS
         */

        // Non-blocking routine which adds fresh addresses in given wallet, and returns corresponding replyobject
        AddAddressToWallet::Reply * addAddressToWalletAsync(const Wallet & requested);

        // Add new addresses in given wallet to wallet, returns new total wallet
        Wallet addAddressToWallet(const Wallet & requested);

        /**
         * ADDRESS ENDPOINT
         * http://dev.blockcypher.com/?shell#address-endpoint
         */

        // Non-blocking routine which adds fresh addresses in given wallet, and returns corresponding replyobject
        AddressEndPoint::Reply * addressEndPointAsync(const QString & walletName, bool unspentOnly = true, uint limit = 200, uint confirmations = -1);

        // Add new addresses in given wallet to wallet, returns new total wallet
        Address addressEndPoint(const QString & walletName, bool unspentOnly = true, uint limit = 200, uint confirmations = -1);

        /**
         * PUSH RAW TRANSACTION
         */

        PushRawTransaction::Reply * pushRawTransactionAsync(const Coin::Transaction & toBeBroadcasted);

        // Push Raw Transaction Endpoint
        bool pushRawTransaction(const Coin::Transaction & toBeBroadcasted);

        /**
          * Fund Wallet From Faucet
          */

        FundWalletFromFaucet::Reply * fundWalletFromFaucetAsync(QString address, quint64 amount);
        bool fundWalletFromFaucet(QString address, quint64 amount);

        /**
         * UTILITIES
         */

        // Endpoint for given network
        static QString endPoint(Coin::Network network);

        QNetworkReply * post(const QString & url, const QJsonObject & data);
        QNetworkReply * get(const QString & url);
        QNetworkReply * deleteResource(const QString & url);

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

