/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <Test.hpp>
#include <app_kit/AppKit.hpp>

#include <common/Network.hpp>
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

}

void Test::init()
{

}

void Test::cleanup()
{

}

void Test::cleanupTestCase()
{

}

void Test::walletCreation()
{
    joystream::AppKit::createWallet("/tmp", Coin::Network::testnet3);
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
