/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_UNSPENT_OUTPUT_HPP
#define COIN_UNSPENT_OUTPUT_HPP

#include <common/KeyPair.hpp>
#include <common/typesafeOutPoint.hpp>
//#include <CoinCore/CoinNodeData.h> // Coin::OutPoint

namespace Coin {

    class UnspentOutput
    {
    public:

        // Default constructor
        UnspentOutput();

        // Constructor from members
        UnspentOutput(const KeyPair & keyPair, const typesafeOutPoint & outPoint, quint64 setValue, uchar_vector scriptPubKey, uchar_vector redeemScript);

        // Comparator
        bool operator==(const UnspentOutput & o) const;
        bool operator!=(const UnspentOutput & o) const;

        // Getters and setters
        KeyPair keyPair() const;
        void setKeyPair(const KeyPair & keyPair);

        typesafeOutPoint outPoint() const;
        void setOutPoint(const typesafeOutPoint & outPoint);

        quint64 value() const;
        void setValue(quint64 value);

        uchar_vector scriptPubKey() const;
        void setScriptPubKey(uchar_vector);

        uchar_vector redeemScript() const;
        void setRedeemScript(uchar_vector);

        virtual bool spendable() const = 0;
        virtual uchar_vector getScriptSig(const TransactionSignature &sig) const = 0;

    private:

        // Controls utxo
        KeyPair _keyPair;

        // OutPoint locating utxo
        typesafeOutPoint _outPoint;

        // Value of output
        quint64 _value;

        // Output script so we can create a signature
        uchar_vector _scriptPubKey;

        // The redeem script to determine how to spend the output and appending to the input script
        uchar_vector _redeemScript;

    };

}

#endif // COIN_UNSPENT_OUTPUT_HPP
