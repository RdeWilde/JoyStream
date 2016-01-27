#ifndef TXOUTPUT_HPP
#define TXOUTPUT_HPP




//Ugly paths for now.
//#include "../../../shared_folder/JoyStream/blockcypher/include/blockcypher/ScriptType.hpp"
//#include "../../../shared_folder/JoyStream/deps/linux/src/mSIGNA/sysroot/include/stdutils/uchar_vector.h"
#include <QString>
#include <QStringList>


class QJsonObject;

namespace Coin {
    class TxOut;
}

namespace blockcypher {


    struct TXOutput {


        // Value in this transaction output, in satoshis.
        // => BC value
        uint64_t _value;

        // from API: Raw hexadecimal encoding of the encumbrance script for this output.
        // => BC script_type string
        uchar_vector _script;

        // Addresses that correspond to this output; typically this will only have a
        // single address, and you can think of this output as having “sent” value to
        // the address contained herein.
        //TODO: This is ok for now, but in the future we likely should use a proper
        //TODO: polymorhpic address type, as described in issue 65 in the main repo.
        //TODO: It will encapsulate the different types of address which exist.
        QStringList _addresses;

        // The type of encumbrance script used for this output.
        // =>BC script_type string
        ScriptType _script_type;


        // == Optional : do we need these?

        // The transaction hash that spent this output.
        // Only returned for outputs that have been spent.
        // The spending transaction may be unconfirmed.
        // QString spent_by;

        // A hex-encoded representation of an OP_RETURN
        // data output, without any other script instructions.
        // Only returned for outputs whose script_type is null-data.
        // QString data_hex;

        // An ASCII representation of an OP_RETURN data output,
        // without any other script instructions. Only returned for
        // outputs whose script_type is null-data and if its data
        // falls into the visible ASCII range.
        // QString data_string;


        // ==


        //Methods takes a QJsonObject as param.
        TXOutput(const QJsonObject & o);
        //Compare operator for this struct.
        bool operator==(const TXOutput & o);
        //      Coin::TxIn toInput() const;

    };



} // end namespace BlockCypher
#endif // TXOUTPUT_HPP
