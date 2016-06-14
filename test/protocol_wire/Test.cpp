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

    PieceData data = PieceData::fromHex("91789132479abcdfabcdfabd123789");
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
    Coin::RedeemScriptHash finalScriptHash = Coin::RedeemScriptHash::fromRawHash(uchar_vector("31149292f8ba11da4aeb833f6cd8ae0650a82340"));
    JoiningContract m(contractPk, finalScriptHash);

    QCOMPARE(m.contractPk(), contractPk);
    QCOMPARE(m.finalScriptHash(), finalScriptHash);
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

void Test::ready() {

    quint64 value = 1123;
    Coin::typesafeOutPoint anchor(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")),
                                  55);
    Coin::PublicKey contractPk(uchar_vector("03ffe71c26651de3056af555d92cee57a42c36976ac1259f0b5cae6b9e94ca38d8"));
    Coin::RedeemScriptHash finalScriptHash = Coin::RedeemScriptHash::fromRawHash(uchar_vector("03a3fac91cac4a5c9ec870b444c4890ec7d68671"));
    Ready m(value, anchor, contractPk, finalScriptHash);

    QCOMPARE(m.value(), value);
    QCOMPARE(m.anchor(), anchor);
    QCOMPARE(m.contractPk(), contractPk);
    QCOMPARE(m.finalScriptHash(), finalScriptHash);

    QCOMPARE(m.messageType(), MessageType::ready);
    TEST_READ_AND_WRITE_FROM_STREAM(m, Ready)
}

void Test::refundSigned() {

    Coin::Signature sig("8185781409579048901234890234");
    RefundSigned m(sig);

    QCOMPARE(m.sig(), sig);
    QCOMPARE(m.messageType(), MessageType::refund_signed);

    // Can't use macro
    //TEST_READ_AND_WRITE_FROM_STREAM(m, RefundSigned)

    QByteArray raw(m.length(), 0);
    QDataStream writeStream(&raw, QIODevice::WriteOnly);

    m.write(writeStream);

    QDataStream readStream(raw);
    RefundSigned m2(readStream, m.length());

    QCOMPARE(m, m2);
}

void Test::requestFullPiece() {

    int pieceIndex = 89;
    RequestFullPiece m(pieceIndex);

    QCOMPARE(m.pieceIndex(), pieceIndex);
    QCOMPARE(m.messageType(), MessageType::request_full_piece);
    TEST_READ_AND_WRITE_FROM_STREAM(m, RequestFullPiece)
}

void Test::signRefund() {

    Coin::TransactionId contractTxId(Coin::TransactionId::fromRPCByteOrder(std::string("eee2b334dd735dac60ae57893c2528087fd3d386b57cac42f4e6ace6403f16b3")));
    quint32 index = 3;
    quint64 value = 1231232;
    Coin::PublicKey contractPk(uchar_vector("03ffe71c26651de3056af555d92cee57a42c36976ac1259f0b5cae6b9e94ca38d8"));
    Coin::PublicKey finalPk(uchar_vector("03ab327d4297619c8d40af4db1b768923f3b3cc5b554dc560cb2959764d33a71d7"));

    SignRefund m(contractTxId, index, value, contractPk, finalPk);

    QCOMPARE(m.contractTxId(), contractTxId);
    QCOMPARE(m.index(), index);
    QCOMPARE(m.value(), value);
    QCOMPARE(m.contractPk(), contractPk);
    QCOMPARE(m.finalPk(), finalPk);
    QCOMPARE(m.messageType(), MessageType::sign_refund);
    TEST_READ_AND_WRITE_FROM_STREAM(m, SignRefund)
}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
