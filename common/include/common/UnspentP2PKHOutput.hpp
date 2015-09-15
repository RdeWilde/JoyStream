/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_UNSPENT_P2PKH_OUTPUT_HPP
#define COIN_UNSPENT_P2PKH_OUTPUT_HPP

#include <common/KeyPair.hpp>
#include <common/typesafeOutPoint.hpp>
//#include <CoinCore/CoinNodeData.h> // Coin::OutPoint

namespace Coin {

/**
 * Refactor this later into something which can hold a
 * set of utxo, not just one, and also what the coinage
 * of inputs are and so on, so that fee calculation can be
 * improved. (does this even matter in fee market?)
 */
class UnspentP2PKHOutput
{
public:

    // Default constructor
    UnspentP2PKHOutput();

    // Constructor from members
    UnspentP2PKHOutput(const KeyPair & keyPair, const typesafeOutPoint & outPoint, quint64 setValue);

    // Constructor from json
    UnspentP2PKHOutput(const QJsonObject & json);

    // Comparator
    bool operator==(const UnspentP2PKHOutput & o) const;
    bool operator!=(const UnspentP2PKHOutput & o) const;

    // Getters and setters
    KeyPair keyPair() const;
    void setKeyPair(const KeyPair & keyPair);

    typesafeOutPoint outPoint() const;
    void setOutPoint(const typesafeOutPoint & outPoint);

    quint64 value() const;
    void setValue(quint64 value);

private:

    // Controls utxo
    KeyPair _keyPair;

    // OutPoint locating utxo
    typesafeOutPoint _outPoint;

    // Value of output
    quint64 _value;

    /**
      we may need scriptPubKey so that we can create spendingsignatures,
      but as long as its p2pkh we can always regenerate from keypair.
      Coin::Output _ouput
    */

};

}

#include <QMetaType>
Q_DECLARE_METATYPE(Coin::UnspentP2PKHOutput)

#endif // COIN_UNSPENT_P2PKH_OUTPUT_HPP