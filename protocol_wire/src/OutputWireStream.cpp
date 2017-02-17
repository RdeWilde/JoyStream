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
    : _buffer(buf) {
}

std::streamsize OutputWireStream::writeObserve(const Observe &) {
    // empty payload
    return 0;
}

std::streamsize OutputWireStream::writeBuy(const Buy & buy) {
    std::streamsize written = 0;

    written += writeInt<>(buy.terms().maxContractFeePerKb());
    written += writeInt<>(buy.terms().maxLock());
    written += writeInt<>(buy.terms().maxPrice());
    written += writeInt<>(buy.terms().minNumberOfSellers());

    return written;
}

std::streamsize OutputWireStream::writeSell(const Sell & sell) {
    std::streamsize written = 0;

    written += writeInt<>(sell.terms().minContractFeePerKb());
    written += writeInt<>(sell.terms().minLock());
    written += writeInt<>(sell.terms().minPrice());
    written += writeInt<>(sell.terms().maxSellers());
    written += writeInt<>(sell.terms().settlementFee());

    written += writeInt<>(sell.index());

    return written;
}

std::streamsize OutputWireStream::writeJoiningContract(const JoiningContract &joining) {
    std::streamsize written = 0;

    written += writePublicKey(joining.contractPk());
    written += writePubKeyHash(joining.finalPkHash());

    return written;
}

std::streamsize OutputWireStream::writeJoinContract(const JoinContract & join) {
    return writeInt<>(join.index());
}

std::streamsize OutputWireStream::writeReady(const Ready &ready) {
    std::streamsize written = 0;

    written += writeInt<>(ready.value());
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

std::streamsize OutputWireStream::writeMessage(const Message *payload) {
    auto type = payload->messageType();

    switch(type) {

    case MessageType::observe: {
            return writeObserve(*dynamic_cast<const protocol_wire::Observe*>(payload));
        }
    case MessageType::buy: {
            return writeBuy(*dynamic_cast<const protocol_wire::Buy*>(payload));
        }
    case MessageType::sell: {
            return writeSell(*dynamic_cast<const protocol_wire::Sell*>(payload));
        }
    case MessageType::join_contract: {
            return writeJoinContract(*dynamic_cast<const protocol_wire::JoinContract*>(payload));
        }
    case MessageType::joining_contract: {
            return writeJoiningContract(*dynamic_cast<const protocol_wire::JoiningContract*>(payload));
        }
    case MessageType::ready: {
            return writeReady(*dynamic_cast<const protocol_wire::Ready*>(payload));
        }
    case MessageType::request_full_piece: {
            return writeRequestFullPiece(*dynamic_cast<const protocol_wire::RequestFullPiece*>(payload));
        }
    case MessageType::full_piece: {
            return writeFullPiece(*dynamic_cast<const protocol_wire::FullPiece*>(payload));
        }
    case MessageType::payment: {
            return writePayment(*dynamic_cast<const protocol_wire::Payment*>(payload));
        }

    default:
        assert(false); // We are not covering full value range of enum
   }

   return -1;
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

OutputWireStream& OutputWireStream::operator <<(uint8_t value) {
    writeInt<uint8_t>(value);
    return *this;
}

OutputWireStream& OutputWireStream::operator <<(uint32_t value) {
    writeInt<uint32_t>(value);
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
    std::streamsize size =   sizeof(uint32_t)  // piece length
                           + piece.pieceData().length(); // piece data
    return size;
}

std::streamsize OutputWireStream::sizeOfPayment(const Payment& payment) {
    std::streamsize size =   sizeof(uint8_t)  // signature length
                           + payment.sig().length(); // signature
    return size;
}

std::streamsize OutputWireStream::sizeOf(const Message * payload) {

    auto type = payload->messageType();

    switch(type) {

    case MessageType::observe: {
            return sizeOfObserve();
        }
    case MessageType::buy: {
            return sizeOfBuy();
        }
    case MessageType::sell: {
            return sizeOfSell();
        }
    case MessageType::join_contract: {
            return sizeOfJoinContract();
        }
    case MessageType::joining_contract: {
            return sizeOfJoiningContract();
        }
    case MessageType::ready: {
            return sizeOfReady();
        }
    case MessageType::request_full_piece: {
            return sizeOfRequestFullPiece();
        }
    case MessageType::full_piece: {
            const FullPiece* fullPiece = dynamic_cast<const FullPiece*>(payload);
            return sizeOfFullPiece(*fullPiece);
        }
    case MessageType::payment: {
            const Payment* payment = dynamic_cast<const Payment*>(payload);
            return sizeOfPayment(*payment);
        }

    default:
        assert(false); // We are not covering full value range of enum
   }

   return -1;
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
