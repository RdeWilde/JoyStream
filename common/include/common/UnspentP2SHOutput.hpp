/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_UNSPENT_P2SH_OUTPUT_HPP
#define COIN_UNSPENT_P2SH_OUTPUT_HPP

#include <common/P2SHScriptPubKey.hpp>
#include <common/UnspentOutput.hpp>
#include <common/KeyPair.hpp>
#include <common/TransactionSignature.hpp>

#include <CoinCore/typedefs.h>
#include <CoinCore/CoinNodeData.h> // Coin::OutPoint

namespace Coin {

class UnspentP2SHOutput : public UnspentOutput
{
public:

    // Default constructor
    UnspentP2SHOutput();

    // Constructor from members
    UnspentP2SHOutput(const KeyPair & keyPair, const uchar_vector & redeemScript, const uchar_vector & optionalData, const typesafeOutPoint & outPoint, uint64_t setValue);

    // Comparator
    bool operator==(const UnspentP2SHOutput & o) const;
    bool operator!=(const UnspentP2SHOutput & o) const;

    // Getters and setters
    KeyPair keyPair() const;
    void setKeyPair(const KeyPair & keyPair);

    uchar_vector redeemScript() const;
    void setRedeemScript(const uchar_vector &);

    uchar_vector optionalData() const;
    void setOptionalData(const uchar_vector &);

    virtual uchar_vector scriptPubKey() const;
    virtual uchar_vector scriptSig(const Transaction &, const SigHashType &) const;

private:

    // Controls utxo
    KeyPair _keyPair;

    // The redeem script to determine how to spend the output and appending to the input script
    uchar_vector _redeemScript;

    // Script chunk which can load optional data
    uchar_vector _optionalDataLoadingScriptChunk;

    TransactionSignature signTransaction(const Transaction & tx, bool anyoneCanPay = false) const;
};

}

#endif // COIN_UNSPENT_P2SH_OUTPUT_HPP
