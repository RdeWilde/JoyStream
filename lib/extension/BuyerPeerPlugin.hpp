#ifndef BUYER_PEER_PLUGIN_HPP
#define BUYER_PEER_PLUGIN_HPP

#include "PeerPlugin.hpp"

class BuyerPeerPlugin : public PeerPlugin
{
public:

    // Constructor
    BuyerPeerPlugin(TorrentPlugin * torrentPlugin,
                    libtorrent::bt_peer_connection * bittorrentPeerConnection,
                    QLoggingCategory & category);

    // Libtorrent callbacks
    virtual void on_disconnect(libtorrent::error_code const & ec);
    virtual void on_connected();

    virtual bool on_extended(int length, int msg, libtorrent::buffer::const_interval body);
    void tick();

private:


    // Last observed peer action
    PeerAction _lastPeerAction;

    // Buyer
    quint64 _bsellerMinPrice;
    quint32 _bsellerMinLock;
    PublicKey _bLastSellerPK;

        /**
         * Contract bulding state
         */

        // join_contract message has been sent to peer
        bool _invitedToJoinContract;

        // sign_refund message has been sent to peer
        bool _invitedToSignRefund;

        // signature in refund_signed was not valid, or refund_signed
        // was not returned within time limit
        bool _failedToSignRefund;

        /**
         * This should not be here, is in payment chnnale in buyer torrent plugin
        quint32 _bContractOutputIndex;
        quint64 _bContractOutputValue;

        Signature _bContractOutputRefund;

        QList<quint32> _sUnservicedSentRequests;
        */

        // Processess message, return whether or not message was valid, that is
        // 1) was compatible with last action of peer
        // 2) was premature, i.e. came before we had even sent a preconditional message
        virtual bool processObserve(const Observe * m);
        virtual bool processBuy(const Buy * m);
        virtual bool processSell(const Sell * m);
        virtual bool processJoinContract(const JoinContract * m);
        virtual bool processJoiningContract(const JoiningContract * m);
        virtual bool processSignRefund(const SignRefund * m);
        virtual bool processRefundSigned(const RefundSigned * m);
        virtual bool processReady(const Ready * m);
        virtual bool processPayment(const Payment * m);
        virtual bool processEnd(const End * m);
};

#endif // BUYER_PEER_PLUGIN_HPP
