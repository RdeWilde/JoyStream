#ifndef SELLER_PEER_PLUGIN_HPP
#define SELLER_PEER_PLUGIN_HPP

#include "PeerPlugin.hpp"

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/Signature.hpp"

class SellerPeerPlugin : public PeerPlugin
{
public:

    // Constructor
    SellerPeerPlugin(TorrentPlugin * torrentPlugin,
                     libtorrent::bt_peer_connection * bittorrentPeerConnection,
                     QLoggingCategory & category);

    virtual bool on_extended(int length, int msg, libtorrent::buffer::const_interval body);
    virtual void tick();

private:

    // Seller
    quint64 _sBuyerMaxPrice;
    quint32 _sBuyerMaxLock;

        /**
         * channel
         */

        //PayeePaymentChannel _channel;

        PublicKey _sPK;
        //PrivateKey _sSK;

        PublicKey _sBuyerContractPK;
        Hash _sContractHash;
        quint32 _sContractOutputIndex;
        quint64 _sContractOutputValue;

        // Set when refund signature sent, as this is lower bound
        QDateTime _sContractRefundEarliestSpendable;

        // Signature for last valid payment from buyer
        Signature _sLastValidPaymentSignatureReceived;

    // Requests received, but not serviced
    QList<quint32> _sPendingRequests;
};

#endif // SELLER_PEER_PLUGIN_HPP
