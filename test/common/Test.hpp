/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 30 2015
 */

#ifndef COMMON_TEST_HPP
#define COMMON_TEST_HPP

#include <QtTest/QtTest>

//class uchar_vector;
class QJsonObject;

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

#endif // COMMON_TEST_HPP
