/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#include <blockcypher/ScriptType.hpp>

#include <QString>

namespace BlockCypher {

    ScriptType toScriptType(const QString & s) {

        if(s == "pay-to-script-hash")
            return ScriptType::pay_to_script_hash;
        else if(s == "pay-to-pubkey-hash")
            return ScriptType::pay_to_pubkey_hash;
        else
            Q_ASSERT(false);
    }

    const char * fromScriptType(ScriptType type) {

        switch(type) {
            case ScriptType::pay_to_script_hash: return "pay-to-script-hash";
            case ScriptType::pay_to_pubkey_hash: return "pay-to-pubkey-hash";
            default:
                Q_ASSERT(false);
        }
    }

}
