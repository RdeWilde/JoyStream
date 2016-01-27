/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#ifndef BLOCKCYPHER_WALLET_HPP
#define BLOCKCYPHER_WALLET_HPP

#include <common/P2PKHAddress.hpp>
#include <QString>

class QJsonObject;
class QNetworkRequest;

namespace blockcypher {

    /**
     * Wallet (http://dev.blockcypher.com/#wallet)
     * ====================================
     * A Wallet contains a list of addresses associated by its name and the user’s token.
     * It can be used interchangeably with all the Address API endpoints,
     * and in many places that require addresses, like when Creating Transactions.
     */
    struct Wallet {

        Wallet();

        Wallet(const QString & token, const QString & name, const QList<Coin::P2PKHAddress> & addresses);

        bool operator==(const Wallet & o);

        Wallet(const QJsonObject & json);

        QJsonObject toJson() const;

        // User token associated with this wallet
        QString _token;

        // Name of the wallet
        QString _name;

        // List of addresses associated with this wallet
        QList<Coin::P2PKHAddress> _addresses;

    };

}

#endif // BLOCKCYPHER_WALLET_HPP

