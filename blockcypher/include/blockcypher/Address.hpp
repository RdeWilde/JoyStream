/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#ifndef BLOCKCYPHER_ADDRESS_HPP
#define BLOCKCYPHER_ADDRESS_HPP

#include <common/P2PKHAddress.hpp>

#include <blockcypher/TXRef.hpp>
//#include <blockcypher/TX.hpp>
#include <blockcypher/Wallet.hpp>
#include <string>

namespace BlockCypher {

    /**
     * Address Endpoint (http://dev.blockcypher.com/#address-endpoint)
     * ===============================================================
     * The default Address Endpoint strikes a balance between speed of response and data
     * on Addresses. It returns more information about an address’ transactions than the
     * Address Balance Endpoint but doesn’t return full transaction information
     * (like the Address Full Endpoint).
     */
    struct Address {

        Address();

        Address(const QJsonObject & o);

        // The requested address
        // *** Not returned if querying a wallet/HD wallet. ***
        // *** Not only P2PKH addresses, all types!!!! ***
        //Coin::P2PKHAddress _address;

        // The requested wallet object.
        // *** Only returned if querying by wallet name instead of public address. ***
        //Wallet _wallet;

        // Total amount of satoshis received by this address
        uint64_t _total_received;

        // Total amount of satoshis sent by this address
        uint64_t _total_sent;

        // Balance of confirmed satoshis on this address.
        // This is the difference between outputs and inputs on this address,
        // but only for transactions that have been included into a block
        // (i.e., for transactions whose confirmations > 0)
        uint64_t _balance;

        // Balance of unconfirmed satoshis on this address. Can be negative
        // (if unconfirmed transactions are just spending outputs).
        // Only unconfirmed transactions (haven’t made it into a block) are included.
        uint64_t _unconfirmed_balance;

        // Total balance of satoshis, including confirmed
        // and unconfirmed transactions, for this address.
        uint64_t _final_balance;

        // Number of confirmed transactions on this address.
        // Only transactions that have made it into a block (confirmations > 0) are counted.
        uint32_t _n_tx;

        // Number of unconfirmed transactions for this address.
        // Only unconfirmed transactions (confirmations == 0) are counted.
        uint32_t _unconfirmed_n_tx;

        // Final number of transactions, including confirmed
        // and unconfirmed transactions, for this address.
        uint32_t _final_n_tx;

        // Array of transaction inputs and outputs for this address.
        // Usually only returned from the standard.
        std::vector<TXRef> _txrefs;

        // Array of unconfirmed transaction inputs and outputs for this address.
        // Usually only returned from the standard.
        std::vector<TXRef> _unconfirmed_txrefs;

        // To retrieve base URL transactions.
        // To get the full URL, concatenate this URL with a transaction’s hash.
        std::string _tx_url;

    };
}

#endif // BLOCKCYPHER_ADDRESS_HPP

