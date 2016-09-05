/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

#define TEST_BITCOIN_NETWORK Coin::Network::testnet3

class Test : public QObject
{
    Q_OBJECT

private:
    QString _tempDataPath;

private slots:

    // Will be called once at the start
    void initTestCase();

    // Will be called before each unit test is executed
    void init();

    // Will be called after every unit test
    void cleanup();

    // Unit Tests
    void walletCreation();

    // Will be called at the end of all tests
    void cleanupTestCase();
};

#endif // TEST_HPP
