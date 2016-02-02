/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#ifndef BLOCKCYPHER_TXINPUT_HPP
#define BLOCKCYPHER_TXINPUT_HPP

#include <blockcypher/ScriptType.hpp>

#include <QString>
#include <QStringList>
#include <QJsonValue>

class QJsonObject;

namespace Coin {
    class TxIn;
}

namespace BlockCypher {

    /**
     * TXInput (http://dev.blockcypher.com/?shell#txinput)
     * =========================================
     * A TXInput represents an input consumed within
     * a transaction. Typically found within an array
     * in a TX. In most cases, TXInputs are from
     * previous UTXOs, with the most prominent exceptions
     * being attempted double-spend and coinbase inputs.
    */
    struct TXInput {

        TXInput() {}

        TXInput(const QJsonObject & o);

        bool operator==(const TXInput & o);

        Coin::TxIn toInput() const;

        // The previous transaction hash where this input was an output. 
        // Not present for coinbase transactions
        // reversed byte order (as used by block explorer and CoinCore)
        QString _prev_hash;

        // The index of the output being spent within the previous transaction. 
        // Not present for coinbase transactions.
        uint32_t _output_index;

        // The value of the output being spent within the previous transaction.
        // ***Not present for coinbase transactions***
        uint64_t _output_value;

        // Raw hexadecimal encoding of the script
        QString _script;

        // The type of script that encumbers the output corresponding to this input.
        ScriptType _script_type;

        // An array of public addresses associated with the output of the previous transaction.
        QStringList _addresses;

        // Legacy 4-byte sequence number, not usually relevant unless dealing with locktime encumbrances
        uint32_t  _sequence;
    };
}


#endif // BLOCKCYPHER_TXINPUT_HPP

