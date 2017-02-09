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
    : _buffer(buf) {
}

InputWireStream& InputWireStream::operator>>(Observe &) {
    // empty payload for Observe message
    return *this;
}

InputWireStream& InputWireStream::operator>>(Buy &buy) {

    auto feeRate = readInt<decltype(buy.terms().maxContractFeePerKb())>();
    auto locktime = readInt<decltype(buy.terms().maxLock())>();
    auto price = readInt<decltype(buy.terms().maxPrice())>();
    auto sellers = readInt<decltype(buy.terms().minNumberOfSellers())>();

    buy.setTerms(BuyerTerms(price, locktime, sellers, feeRate));

    return *this;
}

InputWireStream& InputWireStream::operator>>(Sell &sell) {

    auto feeRate = readInt<decltype(sell.terms().minContractFeePerKb())>();
    auto locktime = readInt<decltype(sell.terms().minLock())>();
    auto price = readInt<decltype(sell.terms().minPrice())>();
    auto sellers = readInt<decltype(sell.terms().maxSellers())>();
    auto settlementFee = readInt<decltype(sell.terms().settlementFee())>();

    sell.setTerms(SellerTerms(price, locktime, sellers, feeRate, settlementFee));

    auto termsIndex = readInt<decltype(sell.index())>();

    sell.setIndex(termsIndex);

    return *this;
}

InputWireStream& InputWireStream::operator>>(JoinContract &join) {

    join.setIndex(readInt<decltype(join.index())>());

    return *this;
}

InputWireStream& InputWireStream::operator>>(JoiningContract &joining) {

    auto publicKey = readPublicKey();
    auto pubKeyHash = readPubKeyHash();

    joining.setContractPk(publicKey);
    joining.setFinalPkHash(pubKeyHash);

    return *this;
}

InputWireStream& InputWireStream::operator>>(Ready &ready) {

    auto value = readInt<decltype(ready.value())>();
    auto anchor = readTypeSafeOutpoint();
    auto publicKey = readPublicKey();
    auto pubKeyHash = readPubKeyHash();

    ready.setValue(value);
    ready.setAnchor(anchor);
    ready.setContractPk(publicKey);
    ready.setFinalPkHash(pubKeyHash);

    return *this;
}

InputWireStream& InputWireStream::operator>>(RequestFullPiece &request) {

    request.setPieceIndex(readInt<decltype(request.pieceIndex())>());
    return *this;
}

InputWireStream& InputWireStream::operator>>(FullPiece &piece) {

    auto pieceData = readPieceData();

    piece.setPieceData(pieceData);

    return *this;
}

InputWireStream& InputWireStream::operator>>(Payment &payment) {

    auto sig = readSignature();

    payment.setSig(sig);

    return *this;
}


std::streamsize InputWireStream::readBytes(unsigned char* data, std::streamsize size) {
    auto ptr = reinterpret_cast<char*>(data);

    auto read = _buffer->sgetn(ptr, size);

    if(read != size) {
        throw std::runtime_error("error reading from stream buffer");
    }

    return read;
}

std::streamsize InputWireStream::readBytes(char *data, std::streamsize size) {
    auto read = _buffer->sgetn(data, size);;

    if(read != size) {
        throw std::runtime_error("error reading from stream buffer");
    }

    return read;
}

std::streamsize InputWireStream::readBytes(std::vector<unsigned char> & data, std::streamsize size) {
    data.resize(size);
    return readBytes(data.data(), size);
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

    return Coin::Signature(rawSig);
}


PieceData InputWireStream::readPieceData() {

    auto size = readInt<uint32_t>();

    // check size limit - to avoid

    // allocate memory for piece
    char* data = static_cast<char*>(malloc(size));

    if(data == nullptr) {
        throw std::runtime_error("unable to allocate memory for piece");
    }

    boost::shared_array<char> piece(data);

    readBytes(data, size);

    return PieceData(piece, size);
}

}
}
