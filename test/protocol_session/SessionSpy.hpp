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

    SendMessageOnConnectionCallbackSlot() : called(false) { reset(); }

    void reset() {
        called = false;
        message = std::unique_ptr<const protocol_wire::ExtendedMessagePayload>();
    }

    SendMessageOnConnection hook() {

        return [this](const protocol_wire::ExtendedMessagePayload * message) {
            this->called = true;
            this->message = std::unique_ptr<const protocol_wire::ExtendedMessagePayload>(message);
        };
    }

    bool called;
    std::unique_ptr<const protocol_wire::ExtendedMessagePayload> message;
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
