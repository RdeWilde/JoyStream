/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 11 2015
 */

#ifndef BLOCKCYPHER_FUND_WALLET_FROM_FAUCET_HPP
#define BLOCKCYPHER_FUND_WALLET_FROM_FAUCET_HPP

#include <blockcypher/BlockCypher.hpp>
#include <blockcypher/TX.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::Transaction

namespace BlockCypher {
namespace FundWalletFromFaucet {

    enum class BlockCypherResponse {
        Pending,
        Funded,
        Error,

        catch_all // temporary
    };

    // QNetworkReply signal handler corresponding to a spesific call
    class Reply : public BlockCypher::Reply {

        Q_OBJECT

    public:

        Reply(QNetworkReply * reply);

        virtual const char * errorMessage() const;

        BlockCypherResponse response() const;

    public slots:

        virtual void processReply();

    signals:

        // Finished processing network reply
        void done(BlockCypherResponse response);

    private:

        // Original request to which handler corresponds

        // Result of call
        BlockCypherResponse _response;

    };
}
}

#endif // BLOCKCYPHER_FUND_WALLET_FROM_FAUCET_HPP

