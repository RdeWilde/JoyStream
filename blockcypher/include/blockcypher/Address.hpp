#ifndef BLOCKCYPHER_ADDRESS_HPP
#define BLOCKCYPHER_ADDRESS_HPP

#include <common/P2PKHAddress.hpp>
#include <blockcypher/TXRef.hpp>

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
}

#endif // BLOCKCYPHER_ADDRESS_HPP

