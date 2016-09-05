/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <Test.hpp>
#include <app_kit/AppKit.hpp>

#include <common/Network.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/asio/impl/src.hpp>

#include <protocol_session/Exceptions.hpp>

// Explicit template instantiation of IdToString()
// used in joystream::protocol_session::exception::ConnectionAlreadyAddedException
template <>
std::string IdToString<boost::asio::ip::basic_endpoint<boost::asio::ip::tcp>>(boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> const&id){
    return id.address().to_string();
}

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
    joystream::AppKit::createWallet(_tempDataPath.toStdString(), Coin::Network::testnet3);

    auto kit = joystream::AppKit::createInstance(_tempDataPath.toStdString(), Coin::Network::testnet3);

    // Make sure the unique_ptr is still managing the interal SPVWallet and Node
    QVERIFY((bool)kit->node());
    QVERIFY((bool)kit->wallet());
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
