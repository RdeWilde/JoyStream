/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#ifndef BLOCKCYPHER_SCRIPT_TYPE_HPP
#define BLOCKCYPHER_SCRIPT_TYPE_HPP

class QString;

namespace blockcypher {

    // Transaction script types
    enum class ScriptType {
        pay_to_script_hash,
        pay_to_pubkey_hash,
        pay_to_multi_pubkey_hash,
        pay_to_pubkey,
        null_data
    };

    ScriptType toScriptType(const QString & s);
    const char * fromScriptType(ScriptType t);

}

#endif // BLOCKCYPHER_SCRIPT_TYPE_HPP
