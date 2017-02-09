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

std::streamsize OutputWireStream::writeObserve(const Observe &) {
    // empty payload
    return 0;
}

std::streamsize OutputWireStream::writeBuy(const Buy & buy) {
    std::streamsize written = 0;

    written += writeInt<decltype(buy.terms().maxContractFeePerKb())>(buy.terms().maxContractFeePerKb());
    written += writeInt<decltype(buy.terms().maxLock())>(buy.terms().maxLock());
    written += writeInt<decltype(buy.terms().maxPrice())>(buy.terms().maxPrice());
    written += writeInt<decltype(buy.terms().minNumberOfSellers())>(buy.terms().minNumberOfSellers());

    return written;
}

std::streamsize OutputWireStream::writeSell(const Sell & sell) {
    std::streamsize written = 0;

    written += writeInt<decltype(sell.terms().minContractFeePerKb())>(sell.terms().minContractFeePerKb());
    written += writeInt<decltype(sell.terms().minLock())>(sell.terms().minLock());
    written += writeInt<decltype(sell.terms().minPrice())>(sell.terms().minPrice());
    written += writeInt<decltype(sell.terms().maxSellers())>(sell.terms().maxSellers());
    written += writeInt<decltype(sell.terms().settlementFee())>(sell.terms().settlementFee());

    written += writeInt<decltype(sell.index())>(sell.index());

    return written;
}

std::streamsize OutputWireStream::writeJoiningContract(const JoiningContract &joining) {
    std::streamsize written = 0;

    written += writePublicKey(joining.contractPk());
    written += writePubKeyHash(joining.finalPkHash());

    return written;
}

std::streamsize OutputWireStream::writeJoinContract(const JoinContract & join) {
    return writeInt<decltype(join.index())>(join.index());
}

std::streamsize OutputWireStream::writeReady(const Ready &ready) {
    std::streamsize written = 0;

    written += writeInt<decltype(ready.value())>(ready.value());
    written += writeTypeSafeOutpoint(ready.anchor());
    written += writePublicKey(ready.contractPk());
    written += writePubKeyHash(ready.finalPkHash());

    return written;
}

std::streamsize OutputWireStream::writeRequestFullPiece(const RequestFullPiece &request) {
    return writeInt<uint32_t>(request.pieceIndex());
}

std::streamsize OutputWireStream::writeFullPiece(const FullPiece &piece) {
    return writePieceData(piece.pieceData());
}

std::streamsize OutputWireStream::writePayment(const Payment &payment) {
    return writeSignature(payment.sig());
}

OutputWireStream& OutputWireStream::operator<<(const Observe &observe) {
    writeObserve(observe);
    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const Buy &buy) {
    writeBuy(buy);
    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const Sell &sell) {
    writeSell(sell);
    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const JoinContract &join) {
    writeJoinContract(join);
    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const JoiningContract &joining) {
    writeJoiningContract(joining);
    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const Ready &ready) {
    writeReady(ready);
    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const RequestFullPiece &request) {
    writeRequestFullPiece(request);
    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const FullPiece &piece) {
    writeFullPiece(piece);
    return *this;
}

OutputWireStream& OutputWireStream::operator<<(const Payment &payment) {
    writePayment(payment);
    return *this;
}

std::streamsize OutputWireStream::sizeOfObserve() {
    const static std::streamsize size = [](){
        Observe msg;
        std::stringbuf buf;
        OutputWireStream stream(&buf);
        return stream.writeObserve(msg);
    }();

    return size;
}

std::streamsize OutputWireStream::sizeOfBuy() {
    const static std::streamsize size = [](){
        Buy msg;
        std::stringbuf buf;
        OutputWireStream stream(&buf);
        return stream.writeBuy(msg);
    }();

    return size;
}

std::streamsize OutputWireStream::sizeOfSell() {
    const static std::streamsize size = [](){
        Sell msg;
        std::stringbuf buf;
        OutputWireStream stream(&buf);
        return stream.writeSell(msg);
    }();

    return size;
}

std::streamsize OutputWireStream::sizeOfJoinContract() {
    const static std::streamsize size = [](){
        JoinContract msg;
        std::stringbuf buf;
        OutputWireStream stream(&buf);
        return stream.writeJoinContract(msg);
    }();

    return size;
}

std::streamsize OutputWireStream::sizeOfJoiningContract() {
    const static std::streamsize size = [](){
        JoiningContract msg;
        std::stringbuf buf;
        OutputWireStream stream(&buf);
        return stream.writeJoiningContract(msg);
    }();

    return size;
}

std::streamsize OutputWireStream::sizeOfReady() {
    const static std::streamsize size = [](){
        Ready msg;
        std::stringbuf buf;
        OutputWireStream stream(&buf);
        return stream.writeReady(msg);
    }();

    return size;
}

std::streamsize OutputWireStream::sizeOfRequestFullPiece() {
    const static std::streamsize size = [](){
        RequestFullPiece msg;
        std::stringbuf buf;
        OutputWireStream stream(&buf);
        return stream.writeRequestFullPiece(msg);
    }();

    return size;
}

std::streamsize OutputWireStream::sizeOfFullPiece(const FullPiece& piece) {
    std::streamsize size =   sizeof(uint32_t)  // piece length uint32_t
                           + piece.pieceData().length(); // piece data
    return size;
}

std::streamsize OutputWireStream::sizeOfPayment(const Payment& payment) {
    std::streamsize size =   sizeof(uint8_t)  // signature length uint32_t
                           + payment.sig().length(); // signature
    return size;
}

std::streamsize OutputWireStream::writeBytes(const unsigned char* from, std::streamsize size) {
    auto ptr = reinterpret_cast<const char*>(from);

    auto written = _buffer->sputn(ptr, size);

    if(written != size) {
        throw std::runtime_error("stream buffer full, writing failed");
    }

    return written;
}

std::streamsize OutputWireStream::writeBytes(const char* from, std::streamsize size) {
    auto written = _buffer->sputn(from, size);

    if(written != size) {
        throw std::runtime_error("stream buffer full, writing failed");
    }

    return written;
}


std::streamsize OutputWireStream::writeBytes(const std::vector<unsigned char> & from) {
    return writeBytes(from.data(), from.size());
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
    written += writeInt<decltype(outPoint.index())>(outPoint.index());
    return written;
}

std::streamsize OutputWireStream::writeTransactionId(const Coin::TransactionId& txid) {
    return writeBytes(txid.toRPCByteOrderVector());
}

std::streamsize OutputWireStream::writeSignature(const Coin::Signature& sig) {
    std::streamsize written = 0;
    auto data = sig.raw();
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
