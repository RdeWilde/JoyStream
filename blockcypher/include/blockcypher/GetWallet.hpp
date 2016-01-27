/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 9 2015
 */

#ifndef BLOCKCYPHER_GETWALLET_HPP
#define BLOCKCYPHER_GETWALLET_HPP

#include <blockcypher/BlockCypher.hpp>
#include <blockcypher/Wallet.hpp>

class QNetworkRequest;
class QNetworkReply;

namespace blockcypher {
namespace GetWallet {

    enum class BlockCypherResponse {
        Pending,
        Returned,
        DoesNotExist,
        InvalidName, // too long or whatever [is this a separate thing from does not exist, as it could nevr have been made?]
        catch_all // Means there was some network issue, i.e. QNetworkReply::bytesAvailable() == 0
    };

    // QNetworkReply signal handler corresponding to a spesific call
    class Reply : public blockcypher::Reply {

        Q_OBJECT

    public:

        Reply(QNetworkReply * reply, const QString & name);

        virtual const char * errorMessage() const;

        // Getters
        QString name() const;
        BlockCypherResponse response() const;
        Wallet wallet() const;

    //public slots:

        virtual void processReply();

    signals:

        // Finished processing network reply
        void done(BlockCypherResponse response);

    private:

        // Original request to which handler corresponds
        QString _name;

        // Result of call
        BlockCypherResponse _response;

        // Response
        Wallet _wallet;
    };

}
}

#endif // BLOCKCYPHER_GETWALLET_HPP

