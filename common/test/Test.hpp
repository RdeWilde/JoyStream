/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 30 2015
 */

#ifndef COMMON_TEST_HPP
#define COMMON_TEST_HPP

#include <QtTest/QtTest>

class Test : public QObject
{
    Q_OBJECT

private slots:

    /**
     * Test routines
     */

    void makeAndCheckSig();
};

#endif // COMMON_TEST_HPP
