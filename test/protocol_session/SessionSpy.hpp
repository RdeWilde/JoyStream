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
struct FrameQueue : public std::deque<Frame<Args...>> {

    template <uint N>
    typename std::tuple_element<N, Frame<Args...>>::type frontFrame() const {

        if(std::deque<Frame<Args...>>::empty())
            throw std::runtime_error("Frame queue is empty.");
        else {

            Frame<Args...> f = this->front();//std::deque<Frame<Args...>>::front();

            return std::get<N>(f);
        }
    }
};

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

template <class ConnectionIdType>
using RemovedConnectionCallbackSlot = SubroutineCallbackSlot<ConnectionIdType, DisconnectCause>;

template <class ConnectionIdType>
using GenerateKeyPairsCallbackSlot = FunctionCallbackSlot<std::vector<Coin::KeyPair>,int>;

template <class ConnectionIdType>
using GenerateP2PKHAddressesCallbackSlot = FunctionCallbackSlot<std::vector<Coin::P2PKHAddress>,int>;

//// ***** CHANGE FROM UNIQUE_PTR TO SHARED_PTR ***** ////

/// NB***: This will leak when cleared, as no one is managing queue.
/// A fix would be to have state machine send out smart pointer rather than raw pointer
typedef SubroutineCallbackSlot<const protocol_wire::ExtendedMessagePayload *> SendMessageOnConnectionCallbackSlot;

typedef FunctionCallbackSlot<bool, Coin::Transaction> BroadcastTransactionCallbackSlot;

template <class ConnectionIdType>
using FullPieceArrivedCallbackSlot = SubroutineCallbackSlot<const ConnectionIdType &, const protocol_wire::PieceData &, int>;

template <class ConnectionIdType>
using LoadPieceForBuyerCallbackSlot = SubroutineCallbackSlot<const ConnectionIdType &, unsigned int>;

template <class ConnectionIdType>
using ClaimLastPaymentCallbackSlot = SubroutineCallbackSlot<const ConnectionIdType &, const joystream::paymentchannel::Payee &>;

template <class ConnectionIdType>
using AnchorAnnouncedCallbackSlot = SubroutineCallbackSlot<const ConnectionIdType &, const Coin::typesafeOutPoint &>;

template <class ConnectionIdType>
struct ConnectionSpy {

    ConnectionSpy(const ConnectionIdType & x) :id(x) { reset(); }

    void reset() { sendMessageOnConnectionCallbackSlot.clear(); }

    bool blank() const { return sendMessageOnConnectionCallbackSlot.empty(); }

    ConnectionIdType id;

    SendMessageOnConnectionCallbackSlot sendMessageOnConnectionCallbackSlot;

};

template <class ConnectionIdType>
class SessionSpy {

public:

    // Handlers for all calls with return types is required, as slots
    // use them to return respons
    SessionSpy(const GenerateKeyPairsCallbackHandler &,
               const GenerateP2PKHAddressesCallbackHandler &,
               const BroadcastTransaction &,
               Session<ConnectionIdType> *);

    ~SessionSpy();

    void toMonitoredObserveMode();

    void toMonitoredSellMode(const SellingPolicy &,
                             const protocol_wire::SellerTerms &);

    void toMonitoredBuyMode(const Coin::UnspentP2PKHOutput &,
                            const BuyingPolicy &,
                            const protocol_wire::BuyerTerms &,
                            const TorrentPieceInformation &);

    // Returns spy for connection.
    // Connection spy owned by session spy.
    ConnectionSpy<ConnectionIdType> * addConnection(const ConnectionIdType & id);

    bool noSessionEvents() const;

    bool noConnectionEventsExcept(const ConnectionIdType & id) const;

    void reset();

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

    // Connection spies
    std::map<ConnectionIdType, ConnectionSpy<ConnectionIdType> *> connectionSpies;

private:

    Session<ConnectionIdType> * _session;
};

// Needed due to c++ needing implementation for all uses of templated types
#include "SessionSpy.cpp"
#endif // SESSIONSPY_HPP
