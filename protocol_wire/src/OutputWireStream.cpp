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

#include <protocol_wire/MessageType.hpp>

namespace joystream {
namespace protocol_wire {

OutputWireStream::OutputWireStream(std::streambuf* buf)
    : BinaryStreamWriter(buf) {
}

std::streamsize OutputWireStream::write(const Observe &) {
    // empty payload
    return 0;
}

std::streamsize OutputWireStream::write(const Buy & buy) {
    std::streamsize written = 0;

    written += writeInt<>(buy.terms().maxContractFeePerKb());
    written += writeInt<>(buy.terms().maxLock());
    written += writeInt<>(buy.terms().maxPrice());
    written += writeInt<>(buy.terms().minNumberOfSellers());

    return written;
}

std::streamsize OutputWireStream::write(const Sell & sell) {
    std::streamsize written = 0;

    written += writeInt<>(sell.terms().minContractFeePerKb());
    written += writeInt<>(sell.terms().minLock());
    written += writeInt<>(sell.terms().minPrice());
    written += writeInt<>(sell.terms().maxSellers());
    written += writeInt<>(sell.terms().settlementFee());

    written += writeInt<>(sell.index());

    return written;
}

std::streamsize OutputWireStream::write(const JoiningContract &joining) {
    std::streamsize written = 0;

    written += writePublicKey(joining.contractPk());
    written += writePubKeyHash(joining.finalPkHash());

    return written;
}

std::streamsize OutputWireStream::write(const JoinContract & join) {
    return writeInt<>(join.index());
}

std::streamsize OutputWireStream::write(const Ready &ready) {
    std::streamsize written = 0;

    written += writeInt<>(ready.value());
    written += writeTypeSafeOutpoint(ready.anchor());
    written += writePublicKey(ready.contractPk());
    written += writePubKeyHash(ready.finalPkHash());

    return written;
}

std::streamsize OutputWireStream::write(const RequestFullPiece &request) {
    return writeInt<uint32_t>(request.pieceIndex());
}

std::streamsize OutputWireStream::write(const FullPiece &piece) {
    return writePieceData(piece.pieceData());
}

std::streamsize OutputWireStream::write(const Payment &payment) {
    return writeSignature(payment.sig());
}

std::streamsize OutputWireStream::writePubKeyHash(const Coin::PubKeyHash& hash) {
    return writeBytes(hash.data(), hash.rawLength());
}

std::streamsize OutputWireStream::writePublicKey(const Coin::PublicKey& pk){
    return writeBytes(pk.toCompressedRawVector());
}

std::streamsize OutputWireStream::writeTypeSafeOutpoint(const Coin::typesafeOutPoint& outPoint) {
    std::streamsize written = 0;
    written += writeTransactionId(outPoint.transactionId());
    written += writeInt<>(outPoint.index());
    return written;
}

std::streamsize OutputWireStream::writeTransactionId(const Coin::TransactionId& txid) {
    return writeBytes(txid.toRPCByteOrderVector());
}

std::streamsize OutputWireStream::writeSignature(const Coin::Signature& sig) {
    std::streamsize written = 0;
    auto data = sig.rawDER();
    written += writeInt<uint8_t>(data.size());
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
