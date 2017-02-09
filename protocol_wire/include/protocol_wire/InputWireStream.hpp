#ifndef JOYSTREAM_PROTOCOL_WIRE_INPUT_WIRE_STREAM_HPP
#define JOYSTREAM_PROTOCOL_WIRE_INPUT_WIRE_STREAM_HPP


#include <protocol_wire/NetworkInt.hpp>
#include <protocol_wire/MessageType.hpp>

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

class ExtendedMessagePayload;

class InputWireStream {
public:

    // Construct the stream from streambuf
    InputWireStream(std::streambuf* buf);

    std::shared_ptr<ExtendedMessagePayload> readMessage(MessageType type);

    virtual InputWireStream& operator>>(Observe &obj);
    virtual InputWireStream& operator>>(Buy &obj);
    virtual InputWireStream& operator>>(Sell &obj);
    virtual InputWireStream& operator>>(JoinContract &obj);
    virtual InputWireStream& operator>>(JoiningContract &obj);
    virtual InputWireStream& operator>>(Ready &obj);
    virtual InputWireStream& operator>>(RequestFullPiece &obj);
    virtual InputWireStream& operator>>(FullPiece &obj);
    virtual InputWireStream& operator>>(Payment &obj);

protected:

    // read integers
    template<class IntType>
    IntType readInt();

    std::streamsize readBytes(unsigned char *destination, std::streamsize size);
    std::streamsize readBytes(char* destination, std::streamsize size);

    // read size bytes into the vector starting at beginning,
    // vector size will be resized to size
    std::streamsize readBytes(std::vector<unsigned char> & destination, std::streamsize size);

    Coin::PubKeyHash readPubKeyHash();
    Coin::PublicKey readPublicKey();
    Coin::typesafeOutPoint readTypeSafeOutpoint();
    Coin::TransactionId readTransactionId();
    Coin::Signature readSignature();
    PieceData readPieceData();

private:
    // prevent copy and assignment ?

    // stream buffer we will write to
    std::streambuf* _buffer;
};

template<class IntType>
IntType InputWireStream::readInt() {
    const auto size = NetworkInt<IntType>::size();

    char data[size];

    auto read = readBytes(data, size);

    if(read != size)
        throw std::runtime_error("reading from stream buffer failed");

    return NetworkInt<IntType>(data).value();
}

}
}


#endif // JOYSTREAM_PROTOCOL_WIRE_INPUT_WIRE_STREAM_HPP
