/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2015
 */

#ifndef BLOCKCYPHER_DELETEWALLET_HPP
#define BLOCKCYPHER_DELETEWALLET_HPP

#include <blockcypher/BlockCypher.hpp>

class QNetworkRequest;
class QNetworkReply;

namespace BlockCypher {
namespace DeleteWallet {

    /**
     * # normal wallet
     * curl -X DELETE https://api.blockcypher.com/v1/btc/main/wallets/alice?token=YOURTOKEN
     */

    enum class BlockCypherResponse {
        Pending,
        Deleted,
        DidntExist,
        InvalidName, // too long or whatever
        UnknownError // QNetworkReply::bytesAvailable() == 0
    };

    class Reply : public BlockCypher::Reply {

        Q_OBJECT

    public:

        Reply(QNetworkReply * reply, const QString & name);

        QString name() const;
        BlockCypherResponse response() const;

    public slots:

        virtual void QNetworkReplyFinished();

    signals:

        // Finished processing network reply
        void done(BlockCypherResponse response);

    private:

        // Original request to which handler corresponds
        QString _name;

        // Result of call
        BlockCypherResponse _response;
    };
}
}

#endif // BLOCKCYPHER_DELETEWALLET_HPP
