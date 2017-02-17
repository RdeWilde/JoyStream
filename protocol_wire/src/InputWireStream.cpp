#include <protocol_wire/InputWireStream.hpp>

#include <protocol_wire/Observe.hpp>
#include <protocol_wire/Buy.hpp>
#include <protocol_wire/Sell.hpp>
#include <protocol_wire/JoinContract.hpp>
#include <protocol_wire/JoiningContract.hpp>
#include <protocol_wire/Ready.hpp>
#include <protocol_wire/RequestFullPiece.hpp>
#include <protocol_wire/FullPiece.hpp>
#include <protocol_wire/Payment.hpp>

namespace joystream {
namespace protocol_wire {

InputWireStream::InputWireStream(std::streambuf* buf)
    : BinaryStreamReader(buf), _buffer(buf) {
}

Observe InputWireStream::readObserve() {
    // empty payload for Observe message
    return Observe();
}

Buy InputWireStream::readBuy() {
    Buy buy;

    auto feeRate = readInt<decltype(buy.terms().maxContractFeePerKb())>();
    auto locktime = readInt<decltype(buy.terms().maxLock())>();
    auto price = readInt<decltype(buy.terms().maxPrice())>();
    auto sellers = readInt<decltype(buy.terms().minNumberOfSellers())>();

    buy.setTerms(BuyerTerms(price, locktime, sellers, feeRate));

    return buy;
}

Sell InputWireStream::readSell() {
    Sell sell;

    auto feeRate = readInt<decltype(sell.terms().minContractFeePerKb())>();
    auto locktime = readInt<decltype(sell.terms().minLock())>();
    auto price = readInt<decltype(sell.terms().minPrice())>();
    auto sellers = readInt<decltype(sell.terms().maxSellers())>();
    auto settlementFee = readInt<decltype(sell.terms().settlementFee())>();

    sell.setTerms(SellerTerms(price, locktime, sellers, feeRate, settlementFee));

    auto termsIndex = readInt<decltype(sell.index())>();

    sell.setIndex(termsIndex);

    return sell;
}

JoinContract InputWireStream::readJoinContract() {
    JoinContract join;

    join.setIndex(readInt<decltype(join.index())>());

    return join;
}

JoiningContract InputWireStream::readJoiningContract() {
    JoiningContract joining;

    auto publicKey = readPublicKey();
    auto pubKeyHash = readPubKeyHash();

    joining.setContractPk(publicKey);
    joining.setFinalPkHash(pubKeyHash);

    return joining;
}

Ready InputWireStream::readReady() {
    Ready ready;

    auto value = readInt<decltype(ready.value())>();
    auto anchor = readTypeSafeOutpoint();
    auto publicKey = readPublicKey();
    auto pubKeyHash = readPubKeyHash();

    ready.setValue(value);
    ready.setAnchor(anchor);
    ready.setContractPk(publicKey);
    ready.setFinalPkHash(pubKeyHash);

    return ready;
}

RequestFullPiece InputWireStream::readRequestFullPiece() {
    RequestFullPiece request;

    request.setPieceIndex(readInt<decltype(request.pieceIndex())>());

    return request;
}

FullPiece InputWireStream::readFullPiece() {
    FullPiece piece;

    auto pieceData = readPieceData();

    piece.setPieceData(pieceData);

    return piece;
}

Payment InputWireStream::readPayment() {
    Payment payment;

    auto sig = readSignature();

    payment.setSig(sig);

    return payment;
}

Coin::PubKeyHash InputWireStream::readPubKeyHash() {
    Coin::PubKeyHash hash;
    readBytes(hash.data(), hash.rawLength());
    return hash;
}

Coin::PublicKey InputWireStream::readPublicKey(){
    std::vector<unsigned char> compressedRaw;

    readBytes(compressedRaw, Coin::PublicKey::compressedLength());

    return Coin::PublicKey::fromCompressedRaw(compressedRaw);
}

Coin::typesafeOutPoint InputWireStream::readTypeSafeOutpoint() {
    Coin::typesafeOutPoint outpoint;

    auto txid = readTransactionId();

    outpoint.setIndex(readInt<decltype(outpoint.index())>());
    outpoint.setTransactionId(txid);

    return outpoint;
}

Coin::TransactionId InputWireStream::readTransactionId() {
    std::vector<unsigned char> rawTxId;
    readBytes(rawTxId, Coin::TransactionId::length());
    return Coin::TransactionId::fromRPCByteOrder(rawTxId);
}

Coin::Signature InputWireStream::readSignature() {
    std::vector<unsigned char> rawSig;

    auto sigSize = readInt<uint8_t>();

    if(sigSize > Coin::Signature::maxLength)
        throw std::runtime_error("signature size larger than maximum");

    readBytes(rawSig, sigSize);

    return Coin::Signature::fromRawDER(rawSig);
}


PieceData InputWireStream::readPieceData() {

    auto size = readInt<uint32_t>();

    // check max size limit?
    if(size > _buffer->in_avail()) {
        throw std::runtime_error("unable to read piece data, not enough data in stream buffer");
    }

    // http://www.boost.org/doc/libs/1_63_0/libs/smart_ptr/shared_array.htm
    boost::shared_array<char> piece(new char[size]);

    readBytes(&piece[0], size);

    return PieceData(piece, size);
}

}
}
