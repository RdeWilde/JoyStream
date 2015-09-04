/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#ifndef TRANSACTION_SIGNATURE_HPP
#define TRANSACTION_SIGNATURE_HPP

#include <common/Signature.hpp>
#include <common/SigHashType.hpp>

namespace Coin {

// Represents a signature for a transaction
class TransactionSignature {

public:

    TransactionSignature(const Signature & sig, SigHashType _type);

    // Serialized as scriptSig ready: <op load signature><DER signature><1 byte sighash type flag>
    uchar_vector serializeForScriptSig() const;

    // Getters and setters
    Signature sig() const;
    void setSig(const Signature & sig);

    SigHashType type() const;
    void setType(SigHashType type);

private:

    Signature _sig;

    SigHashType _type;

    //unint inputSigned;
};

}

#endif // TRANSACTION_SIGNATURE_HPP

