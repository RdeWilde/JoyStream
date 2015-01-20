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
};

#endif // BUYER_PEER_PLUGIN_HPP
