/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015
 */

#include <Test.hpp>
#include <core/core.hpp>

using namespace joystream::core;

template <>
std::string IdToString(const libtorrent::tcp::endpoint & endPoint) {
    return libtorrent::print_endpoint(endPoint);
}

void Test::init() {
}

void Test::basic() {

    BroadcastTransaction callback([](const Coin::Transaction & ) -> void {});

    Node node(callback);
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
