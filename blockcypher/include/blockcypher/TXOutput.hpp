/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 25 2015
 */

#ifndef BLOCKCYPHER_TX_OUTPUT_HPP
#define BLOCKCYPHER_TX_OUTPUT_HPP

#include <blockcypher/ScriptType.hpp>

#include <QString>
#include <QStringList>

class QJsonObject;

namespace Coin {
    class TxOut;
}

namespace BlockCypher {


    /**
     * TXOutput (http://dev.blockcypher.com/#txoutput)
     * =========================================
     * A TXOutput represents an output created by a transaction.
     * Typically found within an array in a TX.
     */
    struct TXOutput {

        // Value in this transaction output, in satoshis.
        uint64_t _value;

        // Raw hexadecimal encoding of the encumbrance script for this output.
        QString _script;

        // Addresses that correspond to this output; typically this will only have a
        // single address, and you can think of this output as having “sent” value to
        // the address contained herein.
        QStringList _addresses;

        // The type of encumbrance script used for this output.
        ScriptType _script_type;

        TXOutput(const QJsonObject & o);

        bool operator==(const TXOutput & o);

        Coin::TxOut toOutput() const;

    };
}

#endif // BLOCKCYPHER_TX_OUTPUT_HPP
