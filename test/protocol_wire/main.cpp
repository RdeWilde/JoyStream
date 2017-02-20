#include <string>
#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <protocol_wire/protocol_wire.hpp>
#include <protocol_wire/char_array_buffer.hpp>

using namespace joystream::protocol_wire;

template <class MESSAGE_TYPE>
MESSAGE_TYPE writeAndReadFromStream(const MESSAGE_TYPE &msg)
{
    std::vector<char> raw(OutputWireStream::sizeOf(&msg));

    char_array_buffer buff(raw);

    OutputWireStream writeStream(&buff);

    writeStream.writeMessage(&msg);

    InputWireStream readStream(&buff);

    auto m2 = readStream.readMessage(msg.messageType());
    MESSAGE_TYPE* m2_ = dynamic_cast<MESSAGE_TYPE*>(m2.get());
    return *m2_;
}

TEST(protocol_wire_test, char_array_buffer) {
    char data[] = {0, 0, 0, 0, 0};
    char_array_buffer buff(data, data + sizeof(data));

    EXPECT_EQ(buff.in_avail(), sizeof(data));

    buff.sputc(0xbe);
    buff.sputc(0xef);

    EXPECT_EQ((unsigned char)data[0], 0xbe);
    EXPECT_EQ((unsigned char)data[1], 0xef);

    // seek write position
    EXPECT_EQ(buff.pubseekpos(3, std::ios::out), 3);

    char AB[2] = {'A', 'B'};
    buff.sputn(AB, 2);

    EXPECT_EQ(data[3], 'A');
    EXPECT_EQ(data[4], 'B');

    // cannot write beyond the limits of the array - bytes written should equal 0
    EXPECT_EQ(buff.sputn(AB, 2), 0);

    // fail to seek outside bounds of array
    EXPECT_EQ(buff.pubseekpos(sizeof(data), std::ios::in), -1);
    EXPECT_EQ(buff.pubseekpos(-1, std::ios::in), -1);

    // seek to absolute position
    EXPECT_EQ(buff.pubseekpos(1, std::ios::in), 1);
    EXPECT_EQ(buff.in_avail(), 4);
    EXPECT_EQ((unsigned int)buff.sgetc(), 0xef);

    // seek to relative position
    EXPECT_EQ(buff.pubseekoff(0, std::ios_base::end, std::ios::in), sizeof(data) - 1);
    EXPECT_EQ(buff.sgetc(), 'B');
}

TEST(protocol_wire_test, network_int)
{
    const int64_t val = 0x0102030405060708;
    NetworkInt<int64_t> i(val);

    EXPECT_EQ(i.rawLength(), sizeof(int64_t));
    EXPECT_EQ(i.value(), val);

    EXPECT_EQ(i.at(0), 1);
    EXPECT_EQ(i.at(1), 2);
    EXPECT_EQ(i.at(2), 3);
    EXPECT_EQ(i.at(3), 4);
    EXPECT_EQ(i.at(4), 5);
    EXPECT_EQ(i.at(5), 6);
    EXPECT_EQ(i.at(6), 7);
    EXPECT_EQ(i.at(7), 8);

    const std::string raw(i.rawLength(), 0);

    std::stringbuf msgBuf(raw);
    std::ostream writeStream(&msgBuf);

    writeStream << i;

    std::istream readStream(&msgBuf);

    NetworkInt<int64_t> x;
    readStream >> x;
    EXPECT_EQ(x.value(), val);

    const int8_t val8 = -1;
    NetworkInt<int8_t> i8(val8);

    EXPECT_EQ(i8.rawLength(), sizeof(int8_t));
    EXPECT_EQ(i8.value(), val8);

    EXPECT_EQ(i8.at(0), 0xff);

}

TEST(protocol_wire_test, observe)
{
    Observe m;

    EXPECT_EQ(MessageType::observe, m.messageType());

    Observe m2 = writeAndReadFromStream<Observe>(m);

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

    FullPiece m2 = writeAndReadFromStream<FullPiece>(m);

    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, invalid_fullPiece) {

    const std::string raw(4, 0);
    std::stringbuf msgBuf(raw);
    OutputWireStream writeStream(&msgBuf);
    writeStream << (uint32_t)20;

    InputWireStream readStream(&msgBuf);
    EXPECT_THROW(readStream.readMessage(MessageType::full_piece), std::exception);
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
    Coin::Signature sig = Coin::Signature::fromRawDERHex("8185781409579048901234890234");
    Payment m(sig);

    EXPECT_EQ(m.sig(), sig);
    EXPECT_EQ(m.messageType(), MessageType::payment);

    Payment m2 = writeAndReadFromStream<Payment>(m);

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
