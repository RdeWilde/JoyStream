/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

namespace Coin {
    class Transaction;
}

#define NETWORK_TYPE Coin::Network::testnet3

class Test : public QObject {

    Q_OBJECT

private slots:


};

#endif // TEST_HPP
