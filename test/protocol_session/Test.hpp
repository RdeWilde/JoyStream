/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 18 2016
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

#include <SessionSpy.hpp>
#include <common/P2PKScriptPubKey.hpp>
#include <common/UnspentP2SHOutput.hpp>

using namespace joystream;
using namespace joystream::protocol_session;

// Id type used to identify connections
typedef uint ID;

class Test : public QObject {

    Q_OBJECT

public:

    Test();

private slots:

    // Runs before & after each unit, creates/deletes up session and spy
    void init();
    void cleanup();

    //// Cases

    // Test observe mode transitions
    void observing();

    // Test sell mode transitions
    void selling_basic();
    void selling();
    //void selling_buyer_invited_with_bad_terms();
    void selling_buyer_requested_invalid_piece();
    void selling_buyer_interrupted_payment();
    void selling_buyer_sent_invalid_payment();
    void selling_buyer_disappears();

    // Test buy mode transitions
    void buying_basic();
    void buying();
    void buying_seller_has_interrupted_contract();
    //void buying_seller_servicing_piece_has_timed_out();
    void buying_seller_sent_invalid_piece();

private:

    //// NB: None of these routines can return values, as they use QTest macroes which dont return this value.

    // Variable shared across all units tests
    Coin::Network network;
    Session<ID> * session;
    SessionSpy<ID> * spy;

    // Integer value used to generate a key in nextPrivateKey()
    uint nextKey;
    // Generates private key which is 32 byte unsigned integer encoded i
    Coin::PrivateKey nextPrivateKey();


    static paymentchannel::Payor getPayor(const protocol_wire::SellerTerms &,
                                          const protocol_wire::Ready &,
                                          const Coin::PrivateKey &,
                                          const Coin::PublicKey &,
                                          const Coin::PubKeyHash &payeeFinalPkHash);

    //// Routines for doing spesific set of tests which can be used across number of cases
    //// Spy is always reset, if affected, by each call

    // Takes started session and spy for session:
    // (1) four peers join without announcing their mode
    // (2) one drops out
    // (3) the rest change modes
    // (4) pause
    // (5) new peer joins
    // (6) old peer updates terms
    // (7) stop
    // (8) start
    void basic();

    // Adds peer
    void addConnection(ID);

    // Peer disconnects
    void removeConnection(ID);

    // Start session for first time, so terms are sent
    void firstStart();

    // Stop session
    void stop();

    // Pause session
    void pause();

    // Session to observe mode
    void toObserveMode();

    // Session to sell mode
    void toSellMode(const protocol_wire::SellerTerms &,
                    int);

    // Session to buy mode
    void toBuyMode(const protocol_wire::BuyerTerms &,
                   const TorrentPieceInformation &);

    //// Pure assert subroutines: do not clear spy

    // Asserts for removal of given peer for given reason
    void assertConnectionRemoved(ID expectedId, DisconnectCause expectedCause) const;

    // Asserts that mode and terms in session match most recent send message callback
    void assertTermsSentToPeer(const SendMessageOnConnectionCallbackSlot &) const;

    // Assert that mode nd terms in sesson were sent to all peers
    void assertTermsSentToAllPeers() const;

    //
    void assertFullPieceSent(ID, const protocol_wire::PieceData &) const;

    //// Selling


    class BuyerPeer {
        // implement later
    };

    // Adds a buyer peer with given id and terms, and navigate to 'ready for piece request' state
    // Write payee contractPk/finalscripthash into last two args
    // (1) peer joins with given id
    // (2) peer announces given buyer terms
    // (3) peer (buyer) sends contract invitation
    // (4) peer (buyer) announces contract
    void addBuyerAndGoToReadyForPieceRequest(ID, const protocol_wire::BuyerTerms &, const protocol_wire::Ready &, Coin::PublicKey &, Coin::PubKeyHash &payeeFinalPkHash);

    //
    void receiveValidFullPieceRequest(ID, int);

    //
    void sendFullPiece(ID, const protocol_wire::PieceData &, int);

    //
    void exchangeDataForPayment(ID, uint, paymentchannel::Payor &);

    //// Buying

    // perhaps this should subclass, or include, a connection spy?
    struct SellerPeer {

        ID id;
        protocol_wire::SellerTerms terms;
        uint32_t sellerTermsIndex;

        ConnectionSpy<ID> * spy;

        // Join message
        Coin::KeyPair contractKeys, finalKeys;
        protocol_wire::JoiningContract joiningContract;

        // Announced ready message
        protocol_wire::Ready ready;

        paymentchannel::Payee payee;

        SellerPeer(ID id, protocol_wire::SellerTerms terms, uint32_t sellerTermsIndex)
            : id(id)
            , terms(terms)
            , sellerTermsIndex(sellerTermsIndex)
            , spy(nullptr) {
        }

        protocol_wire::JoiningContract setJoiningContract() {

            contractKeys = Coin::KeyPair::generate();
            finalKeys = Coin::KeyPair::generate();
            Coin::RedeemScriptHash scriptHash(Coin::P2PKScriptPubKey(finalKeys.pk()));
            joiningContract = protocol_wire::JoiningContract(contractKeys.pk(), scriptHash);

            return joiningContract;
        }

        void contractAnnounced() {

            SendMessageOnConnectionCallbackSlot & slot = spy->sendMessageOnConnectionCallbackSlot;

            QVERIFY((int)slot.size() > 0);
            const protocol_wire::ExtendedMessagePayload * m = std::get<0>(slot.front());

            QCOMPARE(m->messageType(), protocol_wire::MessageType::ready);

            if(m->messageType() == protocol_wire::MessageType::ready) {

                const protocol_wire::Ready * m2 = dynamic_cast<const protocol_wire::Ready *>(m);

                ready = *m2;
                payee = getPayee();

                // Remove message at front
                slot.pop_front();
                delete m2;
            }
        }

        void assertNoContractAnnounced() {
            QCOMPARE((int)spy->sendMessageOnConnectionCallbackSlot.size(), 0);
        }

        void assertContractValidity(const Coin::Transaction & tx) {
            QVERIFY(payee.isContractValid(tx));
        }

        void validatePayment(const Coin::Signature & sig) {
            QVERIFY(payee.registerPayment(sig));
        }

        bool hasPendingFullPieceRequest() {

            if(spy->sendMessageOnConnectionCallbackSlot.size() != 1)
                return false;

            const protocol_wire::ExtendedMessagePayload * m = std::get<0>(spy->sendMessageOnConnectionCallbackSlot.front());

            return m->messageType() == protocol_wire::MessageType::request_full_piece;
        }

        paymentchannel::Payee getPayee() {

            return paymentchannel::Payee(0,
                                         Coin::RelativeLockTime::fromTimeUnits(terms.minLock()),
                                         terms.minPrice(),
                                         ready.value(),
                                         terms.settlementFee(),
                                         ready.anchor(),
                                         contractKeys,
                                         joiningContract.finalPkHash(),
                                         ready.contractPk(),
                                         ready.finalPkHash(),
                                         Coin::Signature());
        }

    };

    typedef std::pair<StartDownloadConnectionInformation, SellerPeer> BuyerSellerRelationship;

    static Coin::Transaction simpleContract(const std::vector<BuyerSellerRelationship> & v) {

        paymentchannel::ContractTransactionBuilder::Commitments commitments;

        for(auto s : v) {

            StartDownloadConnectionInformation inf = s.first;
            SellerPeer peer = s.second;

            paymentchannel::Commitment c(inf.value, inf.buyerContractKeyPair.pk(), peer.joiningContract.contractPk(), Coin::RelativeLockTime::fromTimeUnits(peer.terms.minLock()));

            commitments.push_back(c);
        }

        paymentchannel::ContractTransactionBuilder builder;
        builder.setCommitments(commitments);

        return builder.transaction();
    }

    static PeerToStartDownloadInformationMap<ID> downloadInformationMap(const std::vector<BuyerSellerRelationship> & v) noexcept {

        PeerToStartDownloadInformationMap<ID> map;

        for(auto s : v) {

            StartDownloadConnectionInformation inf = s.first;
            SellerPeer peer = s.second;

            map.insert(std::make_pair(peer.id, inf));
        }

        return map;
    }

    void add(SellerPeer &);

    //void join(const SellerPeer &);

    void completeExchange(SellerPeer &);

    bool hasPendingFullPieceRequest(const std::vector<SellerPeer> &);

    void takeSingleSellerToExchange(SellerPeer &);

    ////

    void assertSellerInvited(const SellerPeer &);

};

#endif // TEST_HPP
