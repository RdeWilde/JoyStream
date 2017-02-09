#ifndef JOYSTREAM_PROTOCOL_WIRE_OUTPUT_WIRE_STREAM_HPP
#define JOYSTREAM_PROTOCOL_WIRE_OUTPUT_WIRE_STREAM_HPP


#include <protocol_wire/NetworkInt.hpp>

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

class OutputWireStream {
public:

    // Construct the stream from streambuf
    OutputWireStream(std::streambuf* buf);

    std::streamsize writeObserve(const Observe&);
    std::streamsize writeBuy(const Buy&);
    std::streamsize writeSell(const Sell&);
    std::streamsize writeJoinContract(const JoinContract&);
    std::streamsize writeJoiningContract(const JoiningContract&);
    std::streamsize writeReady(const Ready&);
    std::streamsize writeRequestFullPiece(const RequestFullPiece&);
    std::streamsize writeFullPiece(const FullPiece&);
    std::streamsize writePayment(const Payment&);

    virtual OutputWireStream& operator<<(const Observe &obj);
    virtual OutputWireStream& operator<<(const Buy &obj);
    virtual OutputWireStream& operator<<(const Sell &obj);
    virtual OutputWireStream& operator<<(const JoinContract &obj);
    virtual OutputWireStream& operator<<(const JoiningContract &obj);
    virtual OutputWireStream& operator<<(const Ready &obj);
    virtual OutputWireStream& operator<<(const RequestFullPiece &obj);
    virtual OutputWireStream& operator<<(const FullPiece &obj);
    virtual OutputWireStream& operator<<(const Payment &obj);

    static std::streamsize sizeOfObserve();
    static std::streamsize sizeOfBuy();
    static std::streamsize sizeOfSell();
    static std::streamsize sizeOfJoinContract();
    static std::streamsize sizeOfJoiningContract();
    static std::streamsize sizeOfReady();
    static std::streamsize sizeOfRequestFullPiece();
    static std::streamsize sizeOfFullPiece(const FullPiece&);
    static std::streamsize sizeOfPayment(const Payment&);
protected:

    // write integers
    template<class IntType>
    std::streamsize writeInt(IntType value) {
        NetworkInt<IntType> encodedInt(value);
        return writeBytes(encodedInt.data(), encodedInt.rawLength());
    }

    // write raw data
    std::streamsize writeBytes(const unsigned char *, std::streamsize);
    std::streamsize writeBytes(const char *, std::streamsize);
    std::streamsize writeBytes(const std::vector<unsigned char>&);

    // write bitcoin common types
    std::streamsize writePubKeyHash(const Coin::PubKeyHash&);
    std::streamsize writePublicKey(const Coin::PublicKey&);
    std::streamsize writeTypeSafeOutpoint(const Coin::typesafeOutPoint&);
    std::streamsize writeTransactionId(const Coin::TransactionId&);
    std::streamsize writeSignature(const Coin::Signature&);

    // write piece data
    std::streamsize writePieceData(const PieceData&);

private:
    // prevent copy and assignment ?

    // stream buffer we will write to
    std::streambuf* _buffer;
};

}
}

#include "../../src/OutputWireStream.cpp"

#endif // JOYSTREAM_PROTOCOL_WIRE_OUTPUT_WIRE_STREAM_HPP
