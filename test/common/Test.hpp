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

    void scripts();

private:

    static bool sighash(const QJsonArray & fixture);
};

#endif // TEST_HPP
