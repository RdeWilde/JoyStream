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

        // The value of the output being spent within the previous transaction.
        // **Not present for coinbase transactions**
        uint64_t _output_value;

        // from API: Raw hexadecimal encoding of the encumbrance script for this output.
        uchar_vector _script;

        // The type of script that encumbers the output corresponding to this input.
        ScriptType _script_type;

        TXInput(const QJsonObject & o);

        bool operator==(const TXInput & o);

        Coin::TxIn toInput() const;

    };

}

#endif // BLOCKCYPHER_TXINPUT_HPP

