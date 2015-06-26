/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <QCoreApplication>

#include "TestLoggerManager.hpp"
#include "TestTxId.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    TestLoggerManager tcTestLoggerManager;
    QTest::qExec(&tcTestLoggerManager, argc, argv);

    TestTxId tcTestTxId;
    QTest::qExec(&tcTestTxId, argc, argv);
}
