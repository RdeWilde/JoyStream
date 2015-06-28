/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef TEST_TXID_HPP
#define TEST_TXID_HPP

#include <QtTest/QtTest>

class TestTxId : public QObject
{
    Q_OBJECT

private slots:

    /**
    // Will be called before the first testfunction is executed.
    void initTestCase();

    // Will be called after the last testfunction was executed.
    void cleanupTestCase();

    // Will be called before each testfunction is executed.
    void init();

    // Will be called after every testfunction.
    void cleanup();

    */

    // Tests constructor
    void constructor();
};

#endif // TEST_TXID_HPP
