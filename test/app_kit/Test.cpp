/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <Test.hpp>
#include <app_kit/kit.hpp>

#include <common/Network.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/asio/impl/src.hpp>

#include <protocol_session/Exceptions.hpp>

void Test::initTestCase()
{
    _tempDataPath = QDir::tempPath() + QDir::separator() + "joystream" + QDir::separator();
}

void Test::init()
{
    boost::filesystem::remove_all(_tempDataPath.toStdString());
    boost::filesystem::create_directory(_tempDataPath.toStdString());
}

void Test::cleanup()
{

}

void Test::cleanupTestCase()
{
    boost::filesystem::remove_all(_tempDataPath.toStdString());
}

void Test::walletCreation()
{

    joystream::appkit::DataDirectory dataDir(_tempDataPath.toStdString());


    auto kit = joystream::appkit::AppKit::create(dataDir.walletFilePath().toStdString(),
                                                 dataDir.blockTreeFilePath().toStdString(),
                                                 TEST_BITCOIN_NETWORK);

    // Make sure the unique_ptr is still managing the interal SPVWallet and Node
    QVERIFY((bool)kit->node());
    QVERIFY((bool)kit->wallet());
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
