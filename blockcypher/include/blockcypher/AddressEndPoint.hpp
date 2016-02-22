/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#ifndef BLOCKCYPHER_ADDRESSENDPOINT_HPP
#define BLOCKCYPHER_ADDRESSENDPOINT_HPP

#include <blockcypher/BlockCypher.hpp>
#include <blockcypher/Wallet.hpp>
#include <blockcypher/Address.hpp>

class QNetworkRequest;
class QNetworkReply;

namespace BlockCypher {
namespace AddressEndPoint {

    /**
     * curl http://api.blockcypher.com/v1/btc/main/addrs/1DEP8i3QJCsomS4BSMY2RpU1upv62aGvhD
     *
     *   {
     *   "address": "1DEP8i3QJCsomS4BSMY2RpU1upv62aGvhD",
     *   "total_received": 4433416,
     *   "total_sent": 0,
     *   "balance": 4433416,
     *   "unconfirmed_balance": 0,
     *   "final_balance": 4433416,
     *   "n_tx": 7,
     *   "unconfirmed_n_tx": 0,
     *   "final_n_tx": 7,
     *   "txrefs": [
     *   {
     *     "tx_hash": "14b1052855bbf6561bc4db8aa501762e7cc1e86994dda9e782a6b73b1ce0dc1e",
     *     "block_height": 302013,
     *     "tx_input_n": -1,
     *    "tx_output_n": 0,
     *    "value": 20213,
     *    "ref_balance": 4433416,
     *    "spent": false,
     *    "confirmations": 63066,
     *    "confirmed": "2014-05-22T03:46:25Z",
     *    "double_spend": false
     *  },..],
     *   "tx_url": "https://api.blockcypher.com/v1/btc/main/txs/"
     *  }
     *
     */

    enum class BlockCypherResponse {
        Pending,
        Fetched,
        WalletDoesNotExist,

        catch_all // temporary
    };

    // QNetworkReply signal handler corresponding to a spesific call
    class Reply : public BlockCypher::Reply {

        Q_OBJECT

    public:

        Reply(QNetworkReply * reply, const QString & walletName, bool unspentOnly, uint limit, uint confirmations);

        virtual const char * errorMessage() const;

        QString walletName() const;

        bool unspentOnly() const;

        uint limit() const;

        uint confirmations() const;

        BlockCypherResponse response() const;

        std::vector<Address> addresses() const;

    public slots:

        virtual void processReply();

    signals:

        // Finished processing network reply
        void done(BlockCypherResponse response);

    private:

        // Original request to which handler corresponds
        const QString _walletName;

        bool _unspentOnly;

        uint _limit;

        uint _confirmations;

        // Result of call
        BlockCypherResponse _response;

        // Address object for wallet
        std::vector<Address> _addresses;

        //QJsonObject _error;
    };

}
}

#endif // BLOCKCYPHER_ADDRESSENDPOINT_HPP

