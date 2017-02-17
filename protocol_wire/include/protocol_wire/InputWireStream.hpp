#ifndef JOYSTREAM_PROTOCOL_WIRE_INPUT_WIRE_STREAM_HPP
#define JOYSTREAM_PROTOCOL_WIRE_INPUT_WIRE_STREAM_HPP

#include <protocol_wire/BinaryStreamReader.hpp>

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

class InputWireStream : public BinaryStreamReader {
public:

    // Construct the stream from streambuf
    InputWireStream(std::streambuf* buf);

    Observe readObserve();
    Buy readBuy();
    Sell readSell();
    JoinContract readJoinContract();
    JoiningContract readJoiningContract();
    Ready readReady();
    RequestFullPiece readRequestFullPiece();
    FullPiece readFullPiece();
    Payment readPayment();

private:

    Coin::PubKeyHash readPubKeyHash();
    Coin::PublicKey readPublicKey();
    Coin::typesafeOutPoint readTypeSafeOutpoint();
    Coin::TransactionId readTransactionId();
    Coin::Signature readSignature();
    PieceData readPieceData();

    // prevent copy and assignment ?

    // stream buffer we will write to
    std::streambuf* _buffer;
};

}
}


#endif // JOYSTREAM_PROTOCOL_WIRE_INPUT_WIRE_STREAM_HPP
