#include <string>
#include <cstdint>
#include <iostream>

#include <gtest/gtest.h>

#include <protocol_wire/protocol_wire.hpp>
#include <protocol_wire/char_array_buffer.hpp>

using namespace joystream::protocol_wire;

template <class MESSAGE_TYPE>
struct Stream {
    Stream(const MESSAGE_TYPE &msg):
        message(msg),
        raw(OutputWireStream::sizeOf(msg)),
        buffer(raw) {

        auto written = OutputWireStream(&buffer).write(msg);

        EXPECT_EQ(written, OutputWireStream::sizeOf(msg));
    }

    MESSAGE_TYPE message;
    std::vector<char> raw;
    char_array_buffer buffer;
    InputWireStream reader() { return InputWireStream(&buffer); }
};

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

    Stream<Observe> stream(m);

    stream.reader().readObserve();
}

TEST(protocol_wire_test, buy)
{
    BuyerTerms terms(2,4,5,6);
    Buy m(terms);

    EXPECT_EQ(terms, m.terms());

    Stream<Buy> stream(m);
    auto m2 = stream.reader().readBuy();
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, sell)
{
    // Sell(const SellerTerms & terms): construct message
    SellerTerms terms(2,4,5,6,7);
    uint32_t index = 44;
    Sell m(terms, index);

    EXPECT_EQ(terms, m.terms());
    EXPECT_EQ(index, m.index());

    Stream<Sell> stream(m);
    auto m2 = stream.reader().readSell();
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, fullPiece)
{
    PieceData data = PieceData::fromHex("91789132479abcdfabcdfabd123789");
    FullPiece m(data);

    EXPECT_EQ(data, m.pieceData());

    Stream<FullPiece> stream(m);
    auto m2 = stream.reader().readFullPiece();
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, invalid_fullPiece) {

    const std::string raw(4, 0);
    std::stringbuf msgBuf(raw);
    BinaryStreamWriter writeStream(&msgBuf);
    writeStream.writeInt((uint32_t)20);

    InputWireStream readStream(&msgBuf);
    EXPECT_THROW(readStream.readFullPiece(), std::exception);
}

TEST(protocol_wire_test, joinContract)
{
    uint32_t index = 32;
    JoinContract m(index);

    EXPECT_EQ(index, m.index());

    Stream<JoinContract> stream(m);
    auto m2 = stream.reader().readJoinContract();
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, joiningContract)
{
    Coin::PublicKey contractPk = Coin::PublicKey::fromCompressedRawHex("03ffe71c26651de3056af555d92cee57a42c36976ac1259f0b5cae6b9e94ca38d8");
    Coin::PubKeyHash finalPkHash(uchar_vector("31149292f8ba11da4aeb833f6cd8ae0650a82340"));

    JoiningContract m(contractPk, finalPkHash);

    EXPECT_EQ(m.contractPk(), contractPk);
    EXPECT_EQ(m.finalPkHash(), finalPkHash);

    Stream<JoiningContract> stream(m);
    auto m2 = stream.reader().readJoiningContract();
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, payment)
{
    Coin::Signature sig = Coin::Signature::fromRawDERHex("8185781409579048901234890234");
    Payment m(sig);

    EXPECT_EQ(m.sig(), sig);

    Stream<Payment> stream(m);
    auto m2 = stream.reader().readPayment();
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

    EXPECT_EQ(m.value(), value);
    EXPECT_EQ(m.anchor(), anchor);
    EXPECT_EQ(m.contractPk(), contractPk);
    EXPECT_EQ(m.finalPkHash(), finalPkHash);

    Stream<Ready> stream(m);
    auto m2 = stream.reader().readReady();
    EXPECT_EQ(m, m2);
}

TEST(protocol_wire_test, requestFullPiece)
{
    int pieceIndex = 89;
    RequestFullPiece m(pieceIndex);

    EXPECT_EQ(m.pieceIndex(), pieceIndex);

    Stream<RequestFullPiece> stream(m);
    auto m2 = stream.reader().readRequestFullPiece();
    EXPECT_EQ(m, m2);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
