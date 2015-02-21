#ifndef SELLER_PEER_PLUGIN_HPP
#define SELLER_PEER_PLUGIN_HPP

#include "PeerPlugin.hpp"

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/Signature.hpp"

class SellerPeerPlugin : public PeerPlugin
{
public:

    /**
     * @brief Configuration of seller peer plugin.
     */
    class Configuration : public PeerPlugin::Configuration {

    public:

        // Default constructor
        Configuration();



    private:


    };

    // Constructor
    SellerPeerPlugin(TorrentPlugin * torrentPlugin,
                     libtorrent::bt_peer_connection * bittorrentPeerConnection,
                     const Configuration & configuration,
                     QLoggingCategory & category);

    // Destructor
    virtual ~SellerPeerPlugin();

    // Libtorrent callbacks
    char const * type() const;
    virtual void on_disconnect(libtorrent::error_code const & ec);
    virtual void on_connected();
    virtual bool on_extension_handshake(libtorrent::lazy_entry const & handshake);
    virtual bool on_have(int index);
    virtual bool on_bitfield(libtorrent::bitfield const & bitfield);
    virtual bool on_have_all();
    virtual bool on_reject(libtorrent::peer_request const & peerRequest);
    virtual bool on_request(libtorrent::peer_request const & peerRequest);
    virtual bool on_unchoke();
    virtual bool on_interested();
    virtual bool on_allowed_fast(int index);
    virtual bool on_have_none();
    virtual bool on_choke();
    virtual bool on_not_interested();
    virtual bool on_piece(libtorrent::peer_request const & piece, libtorrent::disk_buffer_holder & data);
    virtual bool on_suggest(int index);
    virtual bool on_cancel(libtorrent::peer_request const & peerRequest);
    virtual bool on_dont_have(int index);
    virtual void sent_unchoke();
    virtual bool can_disconnect(libtorrent::error_code const & ec);
    virtual bool on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body);
    virtual void on_piece_pass(int index);
    virtual void on_piece_failed(int index);
    virtual void tick();
    virtual bool write_request(libtorrent::peer_request const & peerRequest);

    // Getters and setters
    virtual PluginMode mode() const;

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


    // Processess message
    virtual void processObserve(const Observe * m);
    virtual void processBuy(const Buy * m);
    virtual void processSell(const Sell * m);
    virtual void processJoinContract(const JoinContract * m);
    virtual void processJoiningContract(const JoiningContract * m);
    virtual void processSignRefund(const SignRefund * m);
    virtual void processRefundSigned(const RefundSigned * m);
    virtual void processReady(const Ready * m);
    virtual void processPayment(const Payment * m);
};

#endif // SELLER_PEER_PLUGIN_HPP
