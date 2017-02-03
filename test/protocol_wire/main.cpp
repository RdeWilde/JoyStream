#include <string>
#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <protocol_wire/protocol_wire.hpp>

using namespace joystream::protocol_wire;

template <class MESSAGE_TYPE>
MESSAGE_TYPE writeAndReadFromStream(MESSAGE_TYPE msg)
{
    const std::string raw(msg.length(), 0);
    std::stringbuf msgBuf(raw);
    std::ostream writeStream(&msgBuf);

    msg.write(writeStream);

    std::istream readStream(&msgBuf);
    MESSAGE_TYPE m2(readStream);

    return m2;
}

TEST(protocol_wire_test, buy)
{
    BuyerTerms terms(2,4,5,6);
    Buy m(terms);
    Buy m2 = writeAndReadFromStream<Buy>(m);

    EXPECT_EQ(terms, m.terms());
    EXPECT_EQ(MessageType::buy, m.messageType());
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, sell)
{
    // Sell(const SellerTerms & terms): construct message
    SellerTerms terms(2,4,5,6,7);
    uint32_t index = 44;
    Sell m(terms, index);
    Sell m2 = writeAndReadFromStream<Sell>(m);

    EXPECT_EQ(terms, m.terms());
    EXPECT_EQ(index, m.index());
    EXPECT_EQ(MessageType::sell, m.messageType());
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, fullPiece)
{
    PieceData data = PieceData::fromHex("91789132479abcdfabcdfabd123789");
    FullPiece m(data);

    EXPECT_EQ(data, m.pieceData());
    EXPECT_EQ(MessageType::full_piece, m.messageType());

    // Can't use macro
    //writeAndReadFromStream(m, FullPiece)

    const std::string raw(m.length(), 0);
    std::stringbuf msgBuf(raw);
    std::ostream writeStream(&msgBuf);

    m.write(writeStream);

    std::istream readStream(&msgBuf);
    FullPiece m2(readStream, m.length());

    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, joinContract)
{
    uint32_t index = 32;
    JoinContract m(index);
    JoinContract m2 = writeAndReadFromStream<JoinContract>(m);

    EXPECT_EQ(index, m.index());
    EXPECT_EQ(MessageType::join_contract, m.messageType());
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, joiningContract)
{
    Coin::PublicKey contractPk = Coin::PublicKey::fromCompressedRawHex("03ffe71c26651de3056af555d92cee57a42c36976ac1259f0b5cae6b9e94ca38d8");
    Coin::PubKeyHash finalPkHash(uchar_vector("31149292f8ba11da4aeb833f6cd8ae0650a82340"));

    JoiningContract m(contractPk, finalPkHash);
    JoiningContract m2 = writeAndReadFromStream<JoiningContract>(m);

    EXPECT_EQ(m.contractPk(), contractPk);
    EXPECT_EQ(m.finalPkHash(), finalPkHash);
    EXPECT_EQ(m.messageType(), MessageType::joining_contract);
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, payment)
{
    Coin::Signature sig("8185781409579048901234890234");
    Payment m(sig);

    EXPECT_EQ(m.sig(), sig);
    EXPECT_EQ(m.messageType(), MessageType::payment);

    // Can't use macro
    //writeAndReadFromStream<Payment>(m);

    const std::string raw(m.length(), 0);
    std::stringbuf msgBuf(raw);
    std::ostream writeStream(&msgBuf);

    m.write(writeStream);

    std::istream readStream(&msgBuf);
    Payment m2(readStream, m.length());

    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, ready)
{
    uint64_t value = 1123;
    Coin::typesafeOutPoint anchor(Coin::TransactionId::fromRPCByteOrder(std::string("97a27e013e66bec6cb6704cfcaa5b62d4fc6894658f570ed7d15353835cf3547")),
                                  55);
    Coin::PublicKey contractPk = Coin::PublicKey::fromCompressedRawHex("03ffe71c26651de3056af555d92cee57a42c36976ac1259f0b5cae6b9e94ca38d8");
    Coin::PubKeyHash finalPkHash(uchar_vector("03a3fac91cac4a5c9ec870b444c4890ec7d68671"));

    Ready m(value, anchor, contractPk, finalPkHash);
    Ready m2 = writeAndReadFromStream<Ready>(m);

    EXPECT_EQ(m.value(), value);
    EXPECT_EQ(m.anchor(), anchor);
    EXPECT_EQ(m.contractPk(), contractPk);
    EXPECT_EQ(m.finalPkHash(), finalPkHash);

    EXPECT_EQ(m.messageType(), MessageType::ready);
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, requestFullPiece)
{
    int pieceIndex = 89;
    RequestFullPiece m(pieceIndex);
    RequestFullPiece m2 = writeAndReadFromStream<RequestFullPiece>(m);

    EXPECT_EQ(m.pieceIndex(), pieceIndex);
    EXPECT_EQ(m.messageType(), MessageType::request_full_piece);
    EXPECT_EQ(m, m2);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
