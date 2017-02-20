#ifndef JOYSTREAM_PROTOCOL_WIRE_OUTPUT_WIRE_STREAM_HPP
#define JOYSTREAM_PROTOCOL_WIRE_OUTPUT_WIRE_STREAM_HPP

#include <protocol_wire/BinaryStreamWriter.hpp>

#include <common/PubKeyHash.hpp>

namespace Coin {
    class PublicKey;
    class typesafeOutPoint;
    class TransactionId;
    class Signature;
}

namespace joystream {
namespace protocol_wire {

class Observe;
class Buy;
class Sell;
class JoinContract;
class JoiningContract;
class Ready;
class RequestFullPiece;
class FullPiece;
class Payment;

class PieceData;
class BuyerTerms;
class SellerTerms;

class OutputWireStream : public BinaryStreamWriter {
public:

    // Construct the stream from streambuf
    OutputWireStream(std::streambuf* buf);

    std::streamsize write(const Observe&);
    std::streamsize write(const Buy&);
    std::streamsize write(const Sell&);
    std::streamsize write(const JoinContract&);
    std::streamsize write(const JoiningContract&);
    std::streamsize write(const Ready&);
    std::streamsize write(const RequestFullPiece&);
    std::streamsize write(const FullPiece&);
    std::streamsize write(const Payment&);

    template<class T>
    static std::streamsize sizeOf(const T& msg) {
        // create stream but do not do any actual writing only count total bytes written
        OutputWireStream stream(nullptr);
        return stream.write(msg);
    }

private:

    // write bitcoin common types
    std::streamsize writePubKeyHash(const Coin::PubKeyHash&);
    std::streamsize writePublicKey(const Coin::PublicKey&);
    std::streamsize writeTypeSafeOutpoint(const Coin::typesafeOutPoint&);
    std::streamsize writeTransactionId(const Coin::TransactionId&);
    std::streamsize writeSignature(const Coin::Signature&);

    // write piece data
    std::streamsize writePieceData(const PieceData&);
};

}
}


#endif // JOYSTREAM_PROTOCOL_WIRE_OUTPUT_WIRE_STREAM_HPP
