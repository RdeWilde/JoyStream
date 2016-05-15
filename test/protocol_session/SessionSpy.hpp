/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, May 9 2016
 */

#ifndef SESSIONSPY_HPP
#define SESSIONSPY_HPP

#include <protocol_session/protocol_session.hpp>
#include <protocol_wire/protocol_wire.hpp>

using namespace joystream;
using namespace joystream::protocol_session;

template <class ConnectionIdType>
struct RemovedConnectionCallbackSlot {

    RemovedConnectionCallbackSlot() { reset(); }

    void reset() {
        called = false;
        cause = DisconnectCause::buyer_interrupted_payment; // arbitary, but fixed, default value
    }

    RemovedConnectionCallbackHandler<ConnectionIdType> hook() const {
        return [this](const ConnectionIdType & id, DisconnectCause cause) {
            this->called = true;
            this->id = id;
            this->cause = cause;
        };
    }

    bool called;
    ConnectionIdType id;
    DisconnectCause cause;
};

template <class ConnectionIdType>
struct GenerateKeyPairsCallbackSlot {

    GenerateKeyPairsCallbackSlot(GenerateKeyPairsCallbackHandler & handler)
        : _handler(handler){ reset(); }

    void reset() {
        called = false;
        n = 0;
    }

    GenerateP2PKHAddressesCallbackHandler hook() const {
        return [this](int n) {
            this->called = true;
            this->n = n;
            return _handler(n);
        };
    }

    bool called;
    int n;

private:

    GenerateKeyPairsCallbackHandler _handler;
};

template <class ConnectionIdType>
struct GenerateP2PKHAddressesCallbackSlot {

    GenerateP2PKHAddressesCallbackSlot(GenerateP2PKHAddressesCallbackHandler & handler)
        : _handler(handler){ reset(); }

    void reset() {
        called = false;
        n = 0;
    }

    GenerateP2PKHAddressesCallbackHandler hook() const {
        return [this](int n) {
            this->called = true;
            this->n = n;
            return _handler(n);
        };
    }

    bool called;
    int n;

private:

    GenerateP2PKHAddressesCallbackHandler _handler;
};

struct SendMessageOnConnectionCallbackSlot {

    SendMessageOnConnectionCallbackSlot() { reset(); }

    void reset() {
        called = false;
        observe = protocol_wire::Observe();
        buy = protocol_wire::Buy();
        sell = protocol_wire::Sell();
        join_contract = protocol_wire::JoinContract();
        joining_contract = protocol_wire::JoiningContract();
        //protocol_wire::sign_refund sign_refund;
        //protocol_wire::refund_signed refund_signed;
        ready = protocol_wire::Ready();
        request_full_piece = protocol_wire::RequestFullPiece();
        full_piece = protocol_wire::FullPiece();
        payment = protocol_wire::Payment();
    }

    SendMessageOnConnection hook() {

        return [this](const protocol_wire::ExtendedMessagePayload & message) {

            this->called = true;

            switch(message.messageType()) {
                case protocol_wire::MessageType::observe: observe = *(dynamic_cast<const protocol_wire::Observe *>(&message)); break;
                case protocol_wire::MessageType::buy: buy = *(dynamic_cast<const protocol_wire::Buy *>(&message)); break;
                case protocol_wire::MessageType::sell: sell = *(dynamic_cast<const protocol_wire::Sell *>(&message)); break;
                case protocol_wire::MessageType::join_contract: join_contract = *(dynamic_cast<const protocol_wire::JoinContract *>(&message)); break;
                case protocol_wire::MessageType::joining_contract: joining_contract = *(dynamic_cast<const protocol_wire::JoiningContract *>(&message)); break;
                case protocol_wire::MessageType::ready: ready = *(dynamic_cast<const protocol_wire::Ready *>(&message)); break;
                case protocol_wire::MessageType::request_full_piece: request_full_piece = *(dynamic_cast<const protocol_wire::RequestFullPiece *>(&message)); break;
                case protocol_wire::MessageType::full_piece: full_piece = *(dynamic_cast<const protocol_wire::FullPiece *>(&message)); break;
                case protocol_wire::MessageType::payment: payment = *(dynamic_cast<const protocol_wire::Payment *>(&message)); break;
                default:
                    assert(false);
            }
        };
    }

    bool called;
    protocol_wire::Observe observe;
    protocol_wire::Buy buy;
    protocol_wire::Sell sell;
    protocol_wire::JoinContract join_contract;
    protocol_wire::JoiningContract joining_contract;
    //protocol_wire::sign_refund sign_refund;
    //protocol_wire::refund_signed refund_signed;
    protocol_wire::Ready ready;
    protocol_wire::RequestFullPiece request_full_piece;
    protocol_wire::FullPiece full_piece;
    protocol_wire::Payment payment;

};

struct BroadcastTransactionCallbackSlot {

    BroadcastTransactionCallbackSlot(const BroadcastTransaction & handler)
        : _handler(handler) { reset(); }

    void reset() {
        called = false;
        tx = Coin::Transaction();
    }

    BroadcastTransaction hook() {
        return [this](const Coin::Transaction & tx) {
            this->called = true;
            this->tx = tx;
            return _handler(tx);
        };
    }

    bool called;
    Coin::Transaction tx;

private:

    BroadcastTransaction _handler;
};

template <class ConnectionIdType>
struct FullPieceArrivedCallbackSlot {

    FullPieceArrivedCallbackSlot() { reset(); }

    void reset() {
        called = false;
        id = ConnectionIdType();
        pieceData = protocol_wire::PieceData();
        index = 0;
    }

    FullPieceArrived<ConnectionIdType> hook() const {
        return [this](const ConnectionIdType & id, const protocol_wire::PieceData & pieceData, int index) {
            this->called = true;
            this->id = id;
            this->pieceData = pieceData;
            this->index = index;
        };
    }

    bool called;
    ConnectionIdType id;
    protocol_wire::PieceData pieceData;
    int index;
};

template <class ConnectionIdType>
struct LoadPieceForBuyerCallbackSlot {

    LoadPieceForBuyerCallbackSlot() { reset(); }

    void reset() {
        called = false;
        id = ConnectionIdType();
        index = 0;
    }

    LoadPieceForBuyer<ConnectionIdType> hook() const {
        return [this](const ConnectionIdType & id, unsigned int index) {
            this->called = true;
            this->id = id;
            this->index = index;
        };
    }

    bool called;
    ConnectionIdType id;
    unsigned int index;
};

template <class ConnectionIdType>
struct ClaimLastPaymentCallbackSlot {

    ClaimLastPaymentCallbackSlot() { reset(); }

    void reset() {
        called = false;
        id = ConnectionIdType();
        payee = joystream::paymentchannel::Payee();
    }

    ClaimLastPayment<ConnectionIdType> hook() {
        return [this](const ConnectionIdType & id, const joystream::paymentchannel::Payee & payee) {
            this->called = true;
            this->id = id;
            this->payee = payee;
        };
    }

    bool called;
    ConnectionIdType id;
    joystream::paymentchannel::Payee payee;
};

template <class ConnectionIdType>
struct AnchorAnnouncedCallbackSlot {

    AnchorAnnouncedCallbackSlot() { reset(); }

    void reset() {
        called = false;
        id = ConnectionIdType();
        anchor = Coin::typesafeOutPoint();
    }

    AnchorAnnounced<ConnectionIdType> hook() {
        return [this](const ConnectionIdType & id, const Coin::typesafeOutPoint & anchor) {
            this->called = true;
            this->id = id;
            this->anchor = anchor;
        };
    }

    bool called;
    ConnectionIdType id;
    Coin::typesafeOutPoint anchor;
};

template <class ConnectionIdType>
struct ConnectionSpy {

    ConnectionSpy(const ConnectionIdType &);

    void reset() { sendMessageOnConnectionCallbackSlot.reset(); }

    ConnectionIdType id;

    SendMessageOnConnectionCallbackSlot sendMessageOnConnectionCallbackSlot;

};

template <class ConnectionIdType>
class SessionSpy {

public:

    SessionSpy();

    void toMonitoredSellMode(Session<ConnectionIdType> *,
                             const SellingPolicy &,
                             const protocol_wire::SellerTerms &);

    void toMonitoredBuyMode(Session<ConnectionIdType> *,
                            const Coin::UnspentP2PKHOutput &,
                            const BuyingPolicy &,
                            const protocol_wire::BuyerTerms &,
                            const TorrentPieceInformation &);

    void reset();

    bool blank();

    //// General
    RemovedConnectionCallbackSlot<ConnectionIdType> removedConnectionCallbackSlot;
    GenerateKeyPairsCallbackSlot<ConnectionIdType> generateKeyPairsCallbackSlot;
    GenerateP2PKHAddressesCallbackSlot<ConnectionIdType> generateP2PKHAddressesCallbackSlot;

    //// Buying
    BroadcastTransactionCallbackSlot broadcastTransactionCallbackSlot;
    FullPieceArrivedCallbackSlot<ConnectionIdType> fullPieceArrivedCallbackSlot;

    //// Selling
    LoadPieceForBuyerCallbackSlot<ConnectionIdType> loadPieceForBuyerCallbackSlot;
    ClaimLastPaymentCallbackSlot<ConnectionIdType> claimLastPaymentCallbackSlot;
    AnchorAnnouncedCallbackSlot<ConnectionIdType> anchorAnnouncedCallbackSlot;

private:

    Session<ConnectionIdType> * _session;

    std::map<ConnectionIdType, ConnectionSpy<ConnectionIdType>> _connectionSpies;

};

#endif // SESSIONSPY_HPP
