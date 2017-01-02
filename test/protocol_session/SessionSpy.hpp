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

template <typename... Args>
using Frame = std::tuple<Args...>;

template <typename... Args>
using FrameQueue = std::deque<Frame<Args...>>;

// Callback slot where the underlying callback has no return value, i.e. is a 'subroutine'
template <typename... Args>
class SubroutineCallbackSlot : public FrameQueue<Args...> {

public:

    typedef std::function<void(Args...)> Callback;

    Callback hook() {
        return [this](Args... args) -> void {
            this->push_back(Frame<Args...>(args...));
        };
    }

};

// Callback slot where the underlying callback has return value, i.e. is a 'function'
template <class R, typename... Args>
class FunctionCallbackSlot  : public FrameQueue<Args...>{

public:

    typedef std::function<R(Args...)> Callback;

    FunctionCallbackSlot(const Callback & callback) : _callback(callback) {}

    Callback hook() {
        return [this](Args... args) {
            this->push_back(Frame<Args...>(args...));
            return _callback(args...);
        };
    }

private:

    Callback _callback;
};

/** NB: Callback slot should never accept byref parameter, as that
 * prevents deep copying into Frame, but rather just keeping refernces which go stale.
 * */

template <class ConnectionIdType>
using RemovedConnectionCallbackSlot = SubroutineCallbackSlot<ConnectionIdType, DisconnectCause>;

//template <class ConnectionIdType>
//using GenerateKeyPairCallbackSlot = FunctionCallbackSlot<Coin::KeyPair, const P2SHScriptGeneratorFromPubKey&, const uchar_vector&>;

template <class ConnectionIdType>
using GenerateReceiveAddressesCallbackSlot = FunctionCallbackSlot<std::vector<Coin::P2PKHAddress>,int>;

template <class ConnectionIdType>
using GenerateChangeAddressesCallbackSlot = FunctionCallbackSlot<std::vector<Coin::P2PKHAddress>,int>;

typedef SubroutineCallbackSlot<const protocol_wire::ExtendedMessagePayload *> SendMessageOnConnectionCallbackSlot;

template <class ConnectionIdType>
using FullPieceArrivedCallbackSlot = SubroutineCallbackSlot<ConnectionIdType, protocol_wire::PieceData, int>;

template <class ConnectionIdType>
using LoadPieceForBuyerCallbackSlot = SubroutineCallbackSlot<ConnectionIdType, unsigned int>;

template <class ConnectionIdType>
using ClaimLastPaymentCallbackSlot = SubroutineCallbackSlot<ConnectionIdType, joystream::paymentchannel::Payee>;

template <class ConnectionIdType>
using AnchorAnnouncedCallbackSlot = SubroutineCallbackSlot<ConnectionIdType, quint64, Coin::typesafeOutPoint, Coin::PublicKey, Coin::PubKeyHash>;

template <class ConnectionIdType>
struct ConnectionSpy {

    ConnectionSpy(const ConnectionIdType & x) :id(x) { }

    ~ConnectionSpy() {
        deleteMessagesInSendMessageOnConnectionCallbackSlot();
    }

    void reset() {

        // Clear memory for each message
        deleteMessagesInSendMessageOnConnectionCallbackSlot();

        // Clear queue
        sendMessageOnConnectionCallbackSlot.clear();
    }

    bool blank() const { return sendMessageOnConnectionCallbackSlot.empty(); }

    ConnectionIdType id;

    SendMessageOnConnectionCallbackSlot sendMessageOnConnectionCallbackSlot;

private:

    void deleteMessagesInSendMessageOnConnectionCallbackSlot() {

        for(auto f : sendMessageOnConnectionCallbackSlot) {

            const protocol_wire::ExtendedMessagePayload * c = std::get<0>(f);

            delete c;
        }

    }

};

template <class ConnectionIdType>
class SessionSpy {

public:

    // Handlers for all calls with return types is required, as slots
    // use them to return respons
    SessionSpy(Session<ConnectionIdType> *);

    ~SessionSpy();

    void toMonitoredObserveMode();

    void toMonitoredSellMode(const protocol_wire::SellerTerms &,
                             int);

    void toMonitoredBuyMode(const protocol_wire::BuyerTerms &,
                            const TorrentPieceInformation &);

    // Returns spy for connection.
    // Connection spy owned by session spy.
    ConnectionSpy<ConnectionIdType> * addConnection(const ConnectionIdType & id);

    void removeConnectionSpies();

    bool blank() const;

    bool blankSession() const;

    bool blankConnectionExcept(const ConnectionIdType & id) const;

    void reset();

    bool onlyCalledRemovedConnection() const;
    bool onlyCalledBroadcastTransaction() const;
    bool onlyCalledFullPieceArrived() const;
    bool onlyCalledAnchorAnnounced() const;
    bool onlyCalledLoadPieceForBuyer() const;
    bool onlyCalledClaimLastPayment() const;

    //// General
    RemovedConnectionCallbackSlot<ConnectionIdType> removedConnectionCallbackSlot;

    //// Buying
    FullPieceArrivedCallbackSlot<ConnectionIdType> fullPieceArrivedCallbackSlot;

    //// Selling
    AnchorAnnouncedCallbackSlot<ConnectionIdType> anchorAnnouncedCallbackSlot;
    LoadPieceForBuyerCallbackSlot<ConnectionIdType> loadPieceForBuyerCallbackSlot;
    ClaimLastPaymentCallbackSlot<ConnectionIdType> claimLastPaymentCallbackSlot;

    // Connection spies
    std::map<ConnectionIdType, ConnectionSpy<ConnectionIdType> *> connectionSpies;

private:

    Session<ConnectionIdType> * _session;
};

// Needed due to c++ needing implementation for all uses of templated types
#include "SessionSpy.cpp"
#endif // SESSIONSPY_HPP
