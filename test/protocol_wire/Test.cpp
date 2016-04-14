/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 14 2016
 */

#include <Test.hpp>
#include <protocol_wire/protocol_wire.hpp>

using namespace joystream::protocol_wire;

// MESSAGE: ExtendedMessagePayLoad object, MESSAGE_TYPE: Name of MESSAGE type
#define TEST_READ_AND_WRITE_FROM_STREAM(MESSAGE, MESSAGE_TYPE) \
{ \
    QByteArray raw(MESSAGE.length(), 0); \
    QDataStream writeStream(&raw, QIODevice::WriteOnly); \
    \
    MESSAGE.write(writeStream); \
    \
    QDataStream readStream(raw); \
    MESSAGE_TYPE m2(readStream); \
    \
    QCOMPARE(m, m2); \
} \

void Test::buy() {

    // Buy(const BuyerTerms & terms): construct message
    BuyerTerms terms(2,4,5,6,7);
    Buy m(terms);

    QCOMPARE(m.terms(), terms);
    QCOMPARE(m.messageType(), MessageType::buy);
    TEST_READ_AND_WRITE_FROM_STREAM(m, Buy)
}

void Test::sell() {

    // Sell(const SellerTerms & terms): construct message
    SellerTerms terms(2,4,5,6,7);
    uint32_t index = 44;
    Sell m(terms, index);

    QCOMPARE(m.terms(), terms);
    QCOMPARE(m.index(), index);
    QCOMPARE(m.messageType(), MessageType::sell);
    TEST_READ_AND_WRITE_FROM_STREAM(m, Sell)
}

void Test::fullPiece() {

    int rawDataLength = 54;
    PieceData data(boost::shared_array<char>{new char [rawDataLength]}, rawDataLength);
    FullPiece m(data);

    QCOMPARE(m.pieceData(), data);
    QCOMPARE(m.messageType(), MessageType::full_piece);

    // Can't use macro
    //TEST_READ_AND_WRITE_FROM_STREAM(m, FullPiece)

    QByteArray raw(m.length(), 0);
    QDataStream writeStream(&raw, QIODevice::WriteOnly);

    m.write(writeStream);

    QDataStream readStream(raw);
    FullPiece m2(readStream, m.length());

    QCOMPARE(m, m2);
}

void Test::joinContract() {

    uint32_t index = 32;
    JoinContract m(index);

    QCOMPARE(m.index(), index);
    QCOMPARE(m.messageType(), MessageType::join_contract);
    TEST_READ_AND_WRITE_FROM_STREAM(m, JoinContract)
}

void Test::joiningContract() {

    Coin::PublicKey contractPk(uchar_vector("03ffe71c26651de3056af555d92cee57a42c36976ac1259f0b5cae6b9e94ca38d8"));
    Coin::PubKeyHash finalPkHash("31149292f8ba11da4aeb833f6cd8ae0650a82340");
    JoiningContract m(contractPk, finalPkHash);

    QCOMPARE(m.contractPk(), contractPk);
    QCOMPARE(m.finalPkHash(), finalPkHash);
    QCOMPARE(m.messageType(), MessageType::joining_contract);
    TEST_READ_AND_WRITE_FROM_STREAM(m, JoiningContract)
}

void Test::payment() {

    Coin::Signature sig("8185781409579048901234890234");
    Payment m(sig);

    QCOMPARE(m.sig(), sig);
    QCOMPARE(m.messageType(), MessageType::payment);

    // Can't use macro
    //TEST_READ_AND_WRITE_FROM_STREAM(m, Payment)

    QByteArray raw(m.length(), 0);
    QDataStream writeStream(&raw, QIODevice::WriteOnly);

    m.write(writeStream);

    QDataStream readStream(raw);
    Payment m2(readStream, m.length());

    QCOMPARE(m, m2);
}


QTEST_MAIN(Test)
#include "moc_Test.cpp"
