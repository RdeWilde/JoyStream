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
#include <QVector>

class QNetworkRequest;

namespace Coin {
    enum class Network;
}

/**
 * http://dev.blockcypher.com/#restful-resources
 * =============================================
 * Bitcoin      Main        api.blockcypher.com/v1/btc/main
 * Bitcoin      Testnet3	api.blockcypher.com/v1/btc/test3
 * Dogecoin     Main        api.blockcypher.com/v1/doge/main
 * Litecoin     Main        api.blockcypher.com/v1/ltc/main
 * BlockCypher	Test        api.blockcypher.com/v1/bcy/test
 */

#define BLOCKCYPHER_MAINNET_ENDPOINT "https://api.blockcypher.com/v1/btc/main/"
#define BLOCKCYPHER_TESTNET3_ENDPOINT "https://api.blockcypher.com/v1/btc/main/"
#define BLOCKCYPHER_TOKEN "not-defined"

const char * endPoint(Coin::Network network);

namespace BlockCypher {

/**
 * Wallet (http://dev.blockcypher.com/#wallet)
 * ====================================
 * A Wallet contains a list of addresses associated by its name and the user’s token.
 * It can be used interchangeably with all the Address API endpoints,
 * and in many places that require addresses, like when Creating Transactions.
 */
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
 * TXRef (http://dev.blockcypher.com/#txref)
 * =========================================
 * A TXRef object represents summarized data about
 * a transaction input or output. Typically found in an array
 * within an Address object, which
 * is usually returned from the
 * standard Address Endpoint.
 */
struct TXRef {

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

    enum class Preference {
        high,
        medium,
        low
    };

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

/**
 * Address Endpoint (http://dev.blockcypher.com/#address-endpoint)
 * ===============================================================
 * The default Address Endpoint strikes a balance between speed of response and data
 * on Addresses. It returns more information about an address’ transactions than the
 * Address Balance Endpoint but doesn’t return full transaction information
 * (like the Address Full Endpoint).
 */
struct Address {


    // The requested address
    Coin::P2PKHAddress _address;

    // Total amount of satoshis received by this address
    quint64 _total_received;

    // Total amount of satoshis sent by this address
    quint64 _total_sent;

    // Balance of confirmed satoshis on this address.
    // This is the difference between outputs and inputs on this address,
    // but only for transactions that have been included into a block
    // (i.e., for transactions whose confirmations > 0)
    quint64 _balance;

    // Balance of unconfirmed satoshis on this address. Can be negative
    // (if unconfirmed transactions are just spending outputs).
    // Only unconfirmed transactions (haven’t made it into a block) are included.
    quint64 _unconfirmed_balance;

    // Total balance of satoshis, including confirmed
    // and unconfirmed transactions, for this address.
    quint64 _final_balance;

    // Number of confirmed transactions on this address.
    // Only transactions that have made it into a block (confirmations > 0) are counted.
    quint32 _n_tx;

    // Number of unconfirmed transactions for this address.
    // Only unconfirmed transactions (confirmations == 0) are counted.
    quint32 _unconfirmed_n_tx;

    // Final number of transactions, including confirmed
    // and unconfirmed transactions, for this address.
    quint32 _final_n_tx;

    // Array of transaction inputs and outputs for this address.
    // Usually only returned from the standard.
    QVector<TXRef> _txrefs;

    // To retrieve base URL transactions.
    // To get the full URL, concatenate this URL with a transaction’s hash.
    QString _tx_url;
};


/**
 * Push Raw Transaction Endpoint (http://dev.blockcypher.com/#push-raw-transaction-endpoint)
 *
 * Resource     Method	Request Object	Return Object
 * /txs/push	POST	{“tx”:$TXHEX}	TX
 */

void pushRawTransaction(const QString & rawTransaction);


}

#endif // BLOCKCYPHER_HPP

