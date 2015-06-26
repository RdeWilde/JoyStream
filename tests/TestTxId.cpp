/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "TestTxId.hpp"

#include "extension/BitCoin/TxId.hpp"

/**
void TestTxId::initTestCase() {
    //qDebug("initTestCase()");
}

void TestTxId::cleanupTestCase() {
    //qDebug("cleanupTestCase()");
}

void TestTxId::init() {
    //qDebug("init()");
}

void TestTxId::cleanup() {
    //qDebug("cleanup()");
}
*/

void TestTxId::constructor() {

    QString hexTxId = "9450ab1842304f5c91c06c59d42000b20f6cdc501da42eadb5c29af26be3c6d8";

    TxId tx(hexTxId);

    QVERIFY(tx.toString() == hexTxId);
}

// QTEST_MAIN:
// Generates a main routine which runs all private slot
// test routines on TestTxId

//QTEST_MAIN(TestTxId)
