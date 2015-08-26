/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef BLOCKCYPHER_HPP
#define BLOCKCYPHER_HPP

#include <common/P2PKHAddress.hpp>
#include <common/TransactionId.hpp>

#include <QJsonObject>
#include <QString>

class QNetworkRequest;

namespace Coin {
    enum class Network;
}

#define BLOCKCYPHER_MAINNET_ENDPOINT "https://api.blockcypher.com/v1/btc/main/"
#define BLOCKCYPHER_TESTNET3_ENDPOINT "https://api.blockcypher.com/v1/btc/main/"
#define BLOCKCYPHER_TOKEN

namespace Wallet {

const char * endPoint(Coin::Network network);

namespace BlockCypher {

struct Wallet {

    Wallet(const QString & token, const QString & name, const QList<Coin::P2PKHAddress> & addresses);

    Wallet(const QJsonObject & json);

    QJsonObject toJson() const;

    // POST query for creating wallet
    void createWallet(QNetworkRequest * request, QByteArray & payload);

    // User token associated with this wallet
    QString _token;

    // Name of the wallet
    QString _name;

    // List of addresses associated with this wallet
    QList<Coin::P2PKHAddress> _addresses;

};

// GET query for getting wallet
void getWallet(QNetworkRequest * request, const QString & name);

// GET query for adding address to wallet
void addAddress(QNetworkRequest * request, const QString & name, const Coin::P2PKHAddress & address);

/**
 * A TXRef object represents summarized data about
 * a transaction input or output. Typically found in an array
 * within an Address object, which
 * is usually returned from the
 * standard Address Endpoint.
 */
struct TXRef {

    enum class Preference {
        high,
        medium,
        low
    };

    // Height of the block that contains this transaction.
    // If this is an unconfirmed transaction, it will equal -1.
    int _block_height;

    // The hash of the transaction containing this input or output.
    // While reasonably unique, using hashes as identifiers may be unsafe.
    Coin::TransactionId _tx_hash;

    // Index of the input in the transaction. It’s a negative number for an output.
    int _tx_input_n;

    // Index of the output in the transaction. It’s a negative number for an input.
    int _tx_output_n;

    // The value transfered by this input/output in satoshis exchanged in this transaction.
    quint64 _value;

    // The likelihood that this transaction will make it to the next block;
    // reflects the preference level miners have to include this transaction.
    // Can be high, medium or low.
    Preference preference;

    // True if this output was spent; false otherwise.
    bool spent;

    // True if this is an attempted double spend; false otherwise.
    bool doble_spend;

    // Number of subsequent blocks,
    // including the block the transaction is in.
    // Unconfirmed transactions have 0 confirmations.
    quint32 _confirmations;
};





}

}

#endif // BLOCKCYPHER_HPP

