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

    enum class BlockCypherResponse {
        Pending,
        Created,
        AlreadyExists,
        InvalidName, // too long or whatever

        catch_all // temporary
    };

    // QNetworkReply signal handler corresponding to a spesific call
    class Reply : public BlockCypher::Reply {

        Q_OBJECT

    public:

        Reply(QNetworkReply * reply, const Wallet & requested);

        Wallet requested() const;
        BlockCypherResponse response() const;
        Wallet created() const;

    public slots:

        virtual void QNetworkReplyFinished();

    signals:

        // Finished processing network reply
        void done(BlockCypherResponse response);

    private:

        // Original request to which handler corresponds
        Wallet _requested;

        // Result of call
        BlockCypherResponse _response;

        // Created wallet
        Wallet _created;
        //QJsonObject _error;
    };
}
}

#endif // BLOCKCYPHER_CREATEWALLET_HPP

