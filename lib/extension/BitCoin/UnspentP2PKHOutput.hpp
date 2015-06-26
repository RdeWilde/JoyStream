/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef UNSPENT_P2PKH_OUTPUT_HPP
#define UNSPENT_P2PKH_OUTPUT_HPP

#include "KeyPair.hpp"
#include "OutPoint.hpp"

class QJsonObject;

class UnspentP2PKHOutput
{
public:

    // Default constructor
    UnspentP2PKHOutput();

    // Constructor from members
    UnspentP2PKHOutput(const KeyPair & keyPair, const OutPoint & outPoint, quint64 setValue);

    // Constructor from json
    UnspentP2PKHOutput(const QJsonObject & json);

    // Comparator
    bool operator==(const UnspentP2PKHOutput & o) const;
    bool operator!=(const UnspentP2PKHOutput & o) const;

    // Encode as json
    QJsonObject json() const;

    // Getters and setters
    KeyPair keyPair() const;
    void setKeyPair(const KeyPair & keyPair);

    OutPoint outPoint() const;
    void setOutPoint(const OutPoint & outPoint);

    quint64 value() const;
    void setValue(quint64 value);

private:

    // Controls utxo
    KeyPair _keyPair;

    // (TxId,index)
    OutPoint _outPoint;

    // Value of output
    quint64 _value;

};

#include <QMetaType>
Q_DECLARE_METATYPE(UnspentP2PKHOutput)

#endif // UNSPENT_P2PKH_OUTPUT_HPP
