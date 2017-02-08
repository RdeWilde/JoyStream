#include <protocol_wire/OutputWireStream.hpp>

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

OutputWireStream::OutputWireStream(std::streambuf* buf)
    : _buffer(buf) {
}

OutputWireStream& OutputWireStream::operator<<(const Observe &) {
    // empty payload for Observe message
    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const Buy &buy) {
    std::streamsize written = 0;

    written += writeInt<uint64_t>(buy.terms().maxContractFeePerKb());
    written += writeInt<uint16_t>(buy.terms().maxLock());
    written += writeInt<uint64_t>(buy.terms().maxPrice());
    written += writeInt<uint32_t>(buy.terms().minNumberOfSellers());

    assert(written == sizeOfBuy());

    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const Sell &sell) {
    std::streamsize written = 0;

    written += writeInt<uint64_t>(sell.terms().minContractFeePerKb());
    written += writeInt<uint16_t>(sell.terms().minLock());
    written += writeInt<uint64_t>(sell.terms().minPrice());
    written += writeInt<uint32_t>(sell.terms().maxSellers());

    assert(written == sizeOfSell());

    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const JoinContract &join) {
    auto written = writeInt<uint32_t>(join.index());

    assert(written == sizeOfJoinContract());

    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const JoiningContract &joining) {
    std::streamsize written = 0;

    written += writePublicKey(joining.contractPk());
    written += writePubKeyHash(joining.finalPkHash());

    assert(written == sizeOfJoiningContract());

    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const Ready &ready) {
    std::streamsize written = 0;

    written += writeInt<uint64_t>(ready.value());
    written += writeTypeSafeOutpoint(ready.anchor());
    written += writePublicKey(ready.contractPk());
    written += writePubKeyHash(ready.finalPkHash());

    assert(written == sizeOfReady());

    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const RequestFullPiece &request) {
    std::streamsize written = writeInt<uint32_t>(request.pieceIndex());

    assert(written == sizeOfRequestFullPiece());

    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const FullPiece &piece) {
    std::streamsize written = writePieceData(piece.pieceData());

    assert(written == sizeOfFullPiece(piece));

    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const Payment &payment) {
    std::streamsize written = writeSignature(payment.sig());

    assert(written == sizeOfPayment(payment));

    return *this;
}

std::streamsize OutputWireStream::sizeOfObserve() {
    return 0;
}

std::streamsize OutputWireStream::sizeOfBuy() {

    const static std::streamsize size =   8  // feerate  - uint64_t
                                        + 2  // locktime - uint16_t
                                        + 8  // price    - uint64_t
                                        + 4; // sellers  - uint32_t

    return size;
}

std::streamsize OutputWireStream::sizeOfSell() {
    const static std::streamsize size =   8  // feerate  - uint64_t
                                        + 2  // locktime - uint16_t
                                        + 8  // price    - uint64_t
                                        + 4; // sellers  - uint32_t
    return size;
}

std::streamsize OutputWireStream::sizeOfJoinContract() {
    const static std::streamsize size =   4;  // index  - uint32_t
    return size;
}

std::streamsize OutputWireStream::sizeOfJoiningContract() {
    const static std::streamsize size =   Coin::PublicKey::compressedLength() // public key
                                        + Coin::PubKeyHash::rawLength(); // pubkeyhash
    return size;
}

std::streamsize OutputWireStream::sizeOfReady() {
    const static std::streamsize size =    8 // value - uint64_t
                                         + Coin::TransactionId::rawLength() // outpoint - txid
                                         + 4 // outpoint index - uint32_t
                                         + Coin::PublicKey::compressedLength()
                                         + Coin::PubKeyHash::rawLength();

    return size;
}

std::streamsize OutputWireStream::sizeOfRequestFullPiece() {
    const static std::streamsize size =   4;  // piece index  - uint32_t
    return size;
}

std::streamsize OutputWireStream::sizeOfFullPiece(const FullPiece& piece) {
    const static std::streamsize size =   4  // piece length uint32_t
                                         + piece.pieceData().length(); // piece data
    return size;
}

std::streamsize OutputWireStream::sizeOfPayment(const Payment& payment) {
    const static std::streamsize size =   4  // signature length uint32_t
                                         + payment.sig().length(); // signature
    return size;
}

std::streamsize OutputWireStream::writeBytes(const unsigned char* data, std::streamsize size) {
    auto ptr = reinterpret_cast<const char*>(data);

    auto written = _buffer->sputn(ptr, size);

    if(written != size) {
        throw std::runtime_error("stream buffer full, writing failed");
    }

    return written;
}

std::streamsize OutputWireStream::writeBytes(const char* data, std::streamsize size) {
    auto written = _buffer->sputn(data, size);

    if(written != size) {
        throw std::runtime_error("stream buffer full, writing failed");
    }

    return written;
}


std::streamsize OutputWireStream::writeBytes(const std::vector<unsigned char> & data) {
    return writeBytes(&data[0], data.size());
}

std::streamsize OutputWireStream::writePubKeyHash(const Coin::PubKeyHash& hash) {
    return writeBytes(&hash[0], hash.rawLength());
}

std::streamsize OutputWireStream::writePublicKey(const Coin::PublicKey& pk){
    return writeBytes(pk.toCompressedRawVector());
}

std::streamsize OutputWireStream::writeTypeSafeOutpoint(const Coin::typesafeOutPoint& outPoint) {
    std::streamsize written = 0;
    written += writeTransactionId(outPoint.transactionId());
    written += writeInt<uint32_t>(outPoint.index());
    return written;
}

std::streamsize OutputWireStream::writeTransactionId(const Coin::TransactionId& txid) {
    return writeBytes(txid.toRPCByteOrderVector());
}

std::streamsize OutputWireStream::writeSignature(const Coin::Signature& sig) {
    std::streamsize written = 0;
    auto data = sig.raw();
    written += writeInt<uint32_t>(data.size());
    written += writeBytes(data);
    return written;
}


std::streamsize OutputWireStream::writePieceData(const PieceData& pieceData) {
    std::streamsize written = 0;
    written += writeInt<uint32_t>(pieceData.length());
    written += writeBytes(&pieceData.piece()[0], pieceData.length());
    return written;
}

}
}
