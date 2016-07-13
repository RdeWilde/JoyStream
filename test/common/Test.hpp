/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 30 2015
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

class Test : public QObject
{
    Q_OBJECT

private slots:

    /**
     * Test routines
     */

    void transactionId();

    void basic();

    void makeAndCheckSig();

    void sighash();

    void signForP2PKHSpend();

    void addresses();

    /**
      * Utilities
      */

    void popData();

    /**
     * Input and output scripts
     */

    void P2PKHScriptPubKey();
    void P2PKHScriptSig();

    void MultisigScriptPubKey();
    //void MultisigScriptSig();

    void P2SHScriptPubKey();
    void P2SHScriptSig();

    void P2SHMultisigScriptPubKey();
    void P2SHMultisigScriptSig();

    /**
      * BIP 39 Seed
      */

    void BIP39();

private:

    static bool sighash(const QJsonArray & fixture);
};

#endif // TEST_HPP
