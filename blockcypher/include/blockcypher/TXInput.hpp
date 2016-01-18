/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#ifndef BLOCKCYPHER_TXINPUT_HPP
#define BLOCKCYPHER_TXINPUT_HPP

#include <blockcypher/ScriptType.hpp>
#include <stdutils/uchar_vector.h>
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

        // The previous transaction hash where this input was an output. 
        // Not present for coinbase transactions
        QString _prev_hash;

        // The index of the output being spent within the previous transaction. 
        // Not present for coinbase transactions.
        int _output_index;

        // The value of the output being spent within the previous transaction.
        // **Not present for coinbase transactions**
        // => BC output_value  integer
        //uint64_t _output_value;
        quint64 _output_value;

        // from API: Raw hexadecimal encoding of the script
        // =>BC script string
        uchar_vector _script;

        // The type of script that encumbers the output corresponding to this input.
        // =>BC script_type string
        ScriptType _script_type;

        // An array of public addresses associated with the output of the previous transaction.
        //TODO: This is ok for now, but in the future we likely should use a proper
        //TODO: polymorhpic address type, as described in issue 65 in the main repo.
        //TODO: It will encapsulate the different types of address which exist.
        QStringList _addresses;

        // Legacy 4-byte sequence number, not usually relevant unless dealing with locktime encumbrances
        uint32_t  _sequence;

        // == Optional fields, are any of these needed?
        //
        // Number of confirmations of the previous transaction for which this 
        // input was an output. Currently, only returned in unconfirmed transactions.
        // Do we need this?
        // int age;
        //
        // Name of Wallet or HDWallet from which to derive inputs. 
        // Only used when constructing transactions via the Creating Transactions process.
        // Do we need this?
        // QString wallet_name 
        //
        // Token associated with Wallet or HDWallet used to derive inputs. 
        // Only used when constructing transactions via the Creating Transactions process.
        // QString wallet_token

        // ==

        //Methods takes a QJsonObject as param.
        TXInput(const QJsonObject & o);
        //TXInput(const QJsonValue & o);
        //Compare operator for this struct.
        bool operator==(const TXInput & o);
        Coin::TxIn toInput() const;
    };
}


#endif // BLOCKCYPHER_TXINPUT_HPP

