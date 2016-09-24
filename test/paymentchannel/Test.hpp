/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 6 2015
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

namespace Coin {
    class Transaction;
}

#define WALLET_FILE_NAME "test-paymentchannel"
#define WALLET_SEED Coin::Seed::testSeeds[0]
#define NETWORK_TYPE Coin::Network::testnet3

class Test : public QObject
{
    Q_OBJECT

private slots:

    void redeemScript();

    void refund();

    void settlement();

    void channel();

    //void payor();

    //void payee();

    void paychan_one_to_one();
};

#endif // TEST_HPP
