/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "TestLoggerManager.hpp"

void TestLoggerManager::initTestCase() {
    //qDebug("initTestCase()");
}

void TestLoggerManager::cleanupTestCase() {
    //qDebug("cleanupTestCase()");
}

void TestLoggerManager::init() {
    //qDebug("init()");
}

void TestLoggerManager::cleanup() {
    //qDebug("cleanup()");
}

void TestLoggerManager::dummyCase() {
    QVERIFY(1 == 1);
}

// QTEST_MAIN:
// Generates a main routine which runs all private slot
// test routines on TestLoggerManager

//QTEST_MAIN(TestLoggerManager)
