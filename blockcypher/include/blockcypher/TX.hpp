/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 26 2016
 */

#ifndef BLOCKCYPHER_TX_HPP
#define BLOCKCYPHER_TX_HPP

#include <blockcypher/TXInput.hpp>
#include <blockcypher/TXOutput.hpp>
#include <common/BlockId.hpp>

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QJsonValue>
#include <QJsonObject>
#include <QVector>
#include <QJsonArray>

#include <boost/optional.hpp>

namespace Coin {
    class Transaction;
}

namespace BlockCypher {

    class TX {

        public:

            TX() {}

            TX(const QJsonObject & o);

            Coin::Transaction toTransaction() const;

            QString hash() const { return _hash; }
            int32_t block_height() const { return _block_height; }
            QStringList addresses() const { return _addresses; }
            uint64_t total() const { return _total; }
            uint64_t fees() const { return _fees; }
            uint32_t size() const { return _size; }
            QDateTime received() const { return _received; }
            uint32_t version() const { return _ver; }
            bool double_spend() const { return _double_spend; }
            uint32_t inputs_size() const { return _vin_sz; }
            uint32_t outputs_size() const { return _vout_sz; }
            uint32_t confirmations() const { return _confirmations; }
            QVector<TXInput> inputs() const { return _inputs; }
            QVector<TXOutput> outputs() const { return _outputs; }

            boost::optional<double> confidence() const { return _confidence; }
            boost::optional<QDateTime> confirmed() const { return _confirmed; }
            boost::optional<Coin::BlockId> block_id() const { return _block_hash; }
            boost::optional<QString> double_of() const { return _double_of; }
            boost::optional<QString> double_spend_tx() const { return _double_spend_tx; }
            boost::optional<QString> next_inputs_url() const { return _next_inputs; }
            boost::optional<QString> next_outputs_url() const { return _next_outputs; }

        private:

            // ====================================================
            // M A N D A T O R Y  M E M B E R S
            // ====================================================


            // ** hash : txid aka hash
            //Example value: c63073d683e82d87542d51be5f996f48648327e1bc0b935af295ff79ef25c5b8
            //Todo: Use Coin::TransactionId
            //tested: ok
            //Datatype used in bitcoin source code: char[32]
            // reversed byte order (as used by block explorer and CoinCore)
            QString _hash;

            // ** block_height : Height of the block that contains this transaction.
            //If this is an unconfirmed transaction, it will equal -1 in blockcypher TX payload.
            //tested insert: ok
            //Datatype used in bitcoin source code: int32_t?
            int32_t _block_height;

            // ** addresses : Array of bitcoin public addresses involved in the transaction.
            //tested insert: ok
            QStringList _addresses;

            // ** total : The total number of satoshis exchanged in this transaction.
            //tested insert: ok
            uint64_t _total;

            // ** fees : The total number of fees—in satoshis—collected by miners in this transaction.
            //tested insert: ok
            uint64_t _fees;

            // ** size : The size of the transaction in bytes.
            uint32_t _size;

            // ** preference : The likelihood that this transaction will make it to the next block;
            //reflects the preference level miners have to include this transaction.
            //Can be high, medium or low.
            //TODO: Make enum? But do we need this field?
            //QString preference;

            // ** relayed_by : Address of the peer that sent BlockCypher’s servers this transaction.
            //Ex. value: "54.173.119.29:18333"
            //QString relayed_by;

            // ** received : Time this transaction was received by BlockCypher’s servers.
            //tested insert: ok
            QDateTime _received;

            // ** ver : Version number, typically 1 for Bitcoin transactions.
            //tested insert: ok
            uint32_t _ver;

            // ** lock_time : Time when transaction can be valid. Can be interpreted in
            // two ways: if less than 500 million, refers to block height.
            // If more, refers to Unix epoch time.
            //tested insert: ok
            uint32_t _lock_time;

            // ** double_spend : True if this is an attempted double spend; false otherwise.
            //tested insert: ok
            bool _double_spend;

            // ** vin_sz : Total number of inputs in the transaction.
            //tested insert: ok
            int _vin_sz;

            // ** vout_sz : Total number of outputs in the transaction.
            //tested insert: ok
            int _vout_sz;

            // ** confirmations : Number of subsequent blocks, including the block the
            // transaction is in. Unconfirmed transactions have 0 confirmations.
            //tested insert: ok
            uint32_t _confirmations;

            // ** inputs : TXInput Array, blockcypher limited to 20 by default.
            QVector<TXInput> _inputs;
            // ** outputs : TXOutput Array, blockcypher limited to 20 by default.
            QVector<TXOutput> _outputs;


            //** block_index : Not sure what this is. Some sort of internal number for block cypher ?
            //Their documentation says nothing about it
            //Any point of using this?
            //int block_index;


            // ====================================================
            // O P T I O N A L  V A L U E S
            // ====================================================

            // ** confidence : The percentage chance this transaction will be included in the next block, if unconfirmed.
            // See http://dev.blockcypher.com/#confidence-factor
            boost::optional<double> _confidence;

            // ** confirmed : Time at which transaction was included in a block;
            // only present for confirmed transactions.
            // Ex. val:
            boost::optional<QDateTime> _confirmed;

            // ** receive_count : Number of peers that have sent this transaction to BlockCypher;
            // only present for unconfirmed transactions.
            // Note Jan 7th 2016. This is not received in TX payload for tx-confirmation with conf=0 on websocket.
            //int receive_count

            // ** change_address : Address BlockCypher will use to send back your change,
            //if you constructed this transaction. If not set,
            //defaults to the address from which the coins were originally sent.
            //Note Jan 7th 2016- do we need this?
            //QString change_address;

            // ** block_hash : Hash of the block that contains this transaction;
            // only present for confirmed transactions.
            //Example value: 0000000000000867c1190402b0e484bcbee7375d8e73e46b5292028394093d46
            boost::optional<Coin::BlockId> _block_hash;

            // ** double_of : If this transaction is a double-spend (i.e. double_spend == true)
            // then this is the hash of the transaction it’s double-spending.
            boost::optional<QString> _double_of;

            boost::optional<QString> _double_spend_tx;

            // ** data_protocol : Returned if this transaction contains an OP_RETURN associated
            // with a known data protocol. Data protocols currently detected: blockchainid ;
            // openassets ; factom ; colu ; coinspark
            //QString data_protocol;

            // ** hex : Hex-encoded bytes of the transaction, as sent over the network.
            //QString hex;

            // See http://dev.blockcypher.com/#tx
            // Note: next_inputs and next_outputs will be present if tx inputs are too many, I think
            // default limit is 20 at blockcypher, so would we ever need this?

            // ** next_inputs : If there are more transaction inptus that couldn’t fit into the
            // TXInput array, this is the BlockCypher URL to query the next set of TXInputs (within a TX object).
            boost::optional<QString> _next_inputs;

            // ** next_outputs : If there are more transaction outputs that couldn’t fit into the TXOutput array,
            // this is the BlockCypher URL to query the next set of TXOutputs(within a TX object).
            boost::optional<QString> _next_outputs;
    };
}

#endif // BLOCKCYPHER_TX_HPP
