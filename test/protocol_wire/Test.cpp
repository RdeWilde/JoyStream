/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 14 2016
 */

#include <Test.hpp>
#include <protocol_wire/protocol_wire.hpp>

using namespace joystream::protocol_wire;

void Test::buy() {

    // Buy(const BuyerTerms & terms): construct message
    BuyerTerms terms(2,4,5,6,7);
    Buy m(terms);

    // Getters
    QCOMPARE(m.terms(), terms);
    QCOMPARE(m.messageType(), MessageType::buy);

    // write(QDataStream & stream)
    QByteArray raw(m.length(), 0);
    QDataStream writeStream(&raw, QIODevice::WriteOnly);

    m.write(writeStream);

    // Buy(QDataStream & stream): load new message from stream
    QDataStream readStream(raw);
    Buy m2(readStream);

    // and compare the two and make sure they are identical
    QCOMPARE(m, m2);
}

void Test::sell() {

    // Sell(const SellerTerms & terms): construct message
    SellerTerms terms(2,4,5,6,7);
    uint32_t index = 44;
    Sell m(terms, index);

    // Getters
    QCOMPARE(m.terms(), terms);
    QCOMPARE(m.index(), index);
    QCOMPARE(m.messageType(), MessageType::sell);

    // write(QDataStream & stream)
    QByteArray raw(m.length(), 0);
    QDataStream writeStream(&raw, QIODevice::WriteOnly);

    m.write(writeStream);

    // Buy(QDataStream & stream): load new message from stream
    QDataStream readStream(raw);
    Sell m2(readStream);

    // and compare the two and make sure they are identical
    QCOMPARE(m, m2);
}



QTEST_MAIN(Test)
#include "moc_Test.cpp"
