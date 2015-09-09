/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#ifndef BLOCKCYPHER_CREATEWALLET_HPP
#define BLOCKCYPHER_CREATEWALLET_HPP

#include <blockcypher/BlockCypher.hpp>
#include <blockcypher/Wallet.hpp>
#include <common/P2PKHAddress.hpp>

class QNetworkRequest;
class QNetworkReply;

namespace BlockCypher {
namespace CreateWallet {

    /**
     * http://dev.blockcypher.com/#create-wallet-endpoint
     *
     * # normal wallet
     * curl -d '{"name": "alice","addresses": ["1JcX75oraJEmzXXHpDjRctw3BX6qDmFM8e"]}' https://api.blockcypher.com/v1/btc/main/wallets?token=YOURTOKEN
     *
     * {"token": "YOURTOKEN",
     * "name": "alice",
     * "addresses": [
     *  "1JcX75oraJEmzXXHpDjRctw3BX6qDmFM8e"
     * ]}
    */

    // QNetworkReply signal handler corresponding to a spesific call
    class Reply : public BlockCypher::Reply {

        Q_OBJECT

    public:

        Reply(QNetworkReply * reply, const Wallet & requested);

        // Getters
        Wallet requested() const;

        Wallet response() const;

    public slots:

        virtual void QNetworkReplyError(QNetworkReply::NetworkError code);

        virtual void QNetworkReplyFinished();

    signals:

        // There was a network error
        void error(QNetworkReply::NetworkError code);

        // The given wallet was returned from the original call
        void walletCreated(const Wallet & wallet);

    private:

        // Original request to which handler corresponds
        Wallet _requested;

        // Response
        Wallet _response;
    };
}
}

#endif // BLOCKCYPHER_CREATEWALLET_HPP

