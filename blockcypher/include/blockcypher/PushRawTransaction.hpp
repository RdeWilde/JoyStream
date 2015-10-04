/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 11 2015
 */

#ifndef BLOCKCYPHER_PUSH_RAW_TRANSACTION_HPP
#define BLOCKCYPHER_PUSH_RAW_TRANSACTION_HPP

#include <blockcypher/BlockCypher.hpp>
#include <blockcypher/TX.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

/**
 * (http://dev.blockcypher.com/#push-raw-transaction-endpoint)
 * If you’d prefer to use your own transaction library
 * instead of the recommended path of our two-endpoint
 * transaction generation we’re still happy to help you
 * propagate your raw transactions. Simply send your raw
 * hex-encoded transaction to this endpoint and we’ll
 * leverage our huge network of nodes to propagate your
 * transaction faster than anywhere else.
 *
 * curl -d '{"tx":"01000000011935b41d12936df99d322ac8972b74ec
 * ff7b79408bbccaf1b2eb8015228beac8000000006b483045022100921f
 * c36b911094280f07d8504a80fbab9b823a25f102e2bc69b14bcd369dfc
 * 7902200d07067d47f040e724b556e5bc3061af132d5a47bd96e901429d
 * 53c41e0f8cca012102152e2bb5b273561ece7bbe8b1df51a4c44f5ab0b
 * c940c105045e2cc77e618044ffffffff0240420f00000000001976a914
 * 5fb1af31edd2aa5a2bbaa24f6043d6ec31f7e63288ac20da3c00000000
 * 001976a914efec6de6c253e657a9d5506a78ee48d89762fb3188ac0000
 * 0000"}' https://api.blockcypher.com/v1/bcy/test/txs/push
 *
 */

namespace BlockCypher {
namespace PushRawTransaction {

    enum class BlockCypherResponse {
        Pending,
        Broadcasted,
        InvalidTx,

        catch_all // temporary
    };

    // QNetworkReply signal handler corresponding to a spesific call
    class Reply : public BlockCypher::Reply {

        Q_OBJECT

    public:

        Reply(QNetworkReply * reply, const Coin::Transaction & toBeBroadcasted);

        virtual const char * errorMessage() const;

        Coin::Transaction toBeBroadcasted() const;
        BlockCypherResponse response() const;
        TX returned() const;

    public slots:

        virtual void processReply();

    signals:

        // Finished processing network reply
        void done(BlockCypherResponse response);

    private:

        // Original request to which handler corresponds
        Coin::Transaction _toBeBroadcasted;

        // Result of call
        BlockCypherResponse _response;

        // Created wallet
        TX _returned;
        //Wallet _created;
        //QJsonObject _error;
    };
}
}

#endif // BLOCKCYPHER_PUSH_RAW_TRANSACTION_HPP

