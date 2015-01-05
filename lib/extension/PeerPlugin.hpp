#ifndef PEER_PLUGIN_HPP
#define PEER_PLUGIN_HPP

#include "PeerPluginConfiguration.hpp"
#include "Request/PeerPluginRequest.hpp"
#include "PluginMode.hpp"

#include <libtorrent/extensions.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/bitfield.hpp>
#include <libtorrent/peer_request.hpp>
#include <libtorrent/disk_buffer_holder.hpp>
#include <libtorrent/buffer.hpp>
#include <libtorrent/peer_id.hpp> // sha1_hash

#include <QObject>

#include <queue>

class TorrentPlugin;
class PeerPluginStatus;
class PeerPluginRequest;
class ExtendedMessagePayload;
class ObserveMessage;
class BuyMessage;
class SellMessage;

/*
 * We inherit from QObject so we can send signals, and QObject must be first:
 * http://doc.trolltech.com/4.5/moc.html
 */

class PeerPlugin : public QObject, public libtorrent::peer_plugin {

    Q_OBJECT

public:

    // Constructor
    PeerPlugin(TorrentPlugin * torrentPlugin,
               libtorrent::bt_peer_connection * bittorrentPeerConnection,
               QLoggingCategory & category);
               //PeerPluginConfiguration * peerPluginConfiguration);

    // Destructor
    ~PeerPlugin();

    /**
     * All virtual functions below should ONLY be called by libtorrent network thread,
     * never by other threads, as this causes synchronization failures.
     */
    virtual char const* type() const;
    virtual void add_handshake(libtorrent::entry & handshake);
    virtual void on_disconnect(libtorrent::error_code const & ec);
    virtual void on_connected();
    virtual bool on_handshake(char const* reserved_bits);
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
    virtual bool on_extended(int length, int msg, libtorrent::buffer::const_interval body);
    virtual bool on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body);
    virtual void on_piece_pass(int index);
    virtual void on_piece_failed(int index);
    virtual void tick();
    virtual bool write_request(libtorrent::peer_request const & peerRequest);

    //const libtorrent::tcp::endpoint & getEndPoint() const;
    //const PeerPluginId & getPeerPluginId() const;

    /**
     * Subroutines for libtorrent thread.
     */

    // Processig routine for peer plugin requests, request pointer is owned by plugin dispatcher
    void processPeerPluginRequest(const PeerPluginRequest * peerPluginRequest);

    // Torrent plugin calls to start
    //void startPlugin(PluginMode pluginMode);
    void startPlugin();
    void startPlugin(const SellMessage & m);
    void startPlugin(const BuyMessage & m);

    // Sends extended message to peer
    // does not take ownership of pointer
    void sendExtendedMessage(const ExtendedMessagePayload & extendedMessage);

    // Determines the message type, calls correct handler, then frees message
    void processExtendedMessage(ExtendedMessagePayload * extendedMessage);

    // Processess a message
    void processPassiveMessage(const ObserveMessage * passiveMessage);
    void processBuyMessage(const BuyMessage * buyMessage);
    void processSellMessage(const SellMessage * sellMessage);

    /*
    void processSetupBeginMessage();
    void processSetupBeginRejectMessage();
    void processSetupContractMessage();
    void processSetupContractSignedMessage();
    void processSetupRefundMessage();
    */

    //void setConfiguration(PeerPluginConfiguration * peerPluginConfiguration);

    // Getters
    BEPSupportStatus peerBEP10SupportedStatus() const;
    BEPSupportStatus peerBEP43SupportedStatus() const;
    PeerPluginState peerPluginState() const;
    libtorrent::tcp::endpoint endPoint() const;

protected:

    // Torrent plugin for torrent
    TorrentPlugin * _torrentPlugin;

    // Connection to peer for this plugin
    libtorrent::bt_peer_connection * _bittorrentPeerConnection;

    // Logging category
    QLoggingCategory & _category;

    // Endpoint
    libtorrent::tcp::endpoint _endPoint;

    // Queue of received valid messages which have not yet been processed
    // messages enter queue in on_extended(), and are dispatched in tick()
    std::queue<ExtendedMessagePayload *> _unprocessedMessageQueue;

    // Indicates if plugin has been started
    // Before this becomes true, plugin will
    // not do anythng which compromises eventually
    // going into seller or buyer mode
    bool _pluginStarted;

    /**
     * Persistent state below
     */

    // Mode of plugin when started
    PluginMode _clientPluginMode;

    // Mapping from messages to BEP10 ID of peer
    ExtendedMessageIdMapping _clientMapping, _peerMapping;

    // Indicates whether peer supports
    BEPSupportStatus _peerBEP10SupportedStatus, // BEP10
                        _peerBEP43SupportedStatus; // BEP43

    // State of peer plugin
    PeerPluginState _peerPluginState;

    // Id of this peer plugin
    PeerPluginId _peerPluginId; // assess later, is the redundancy worth it

    // Has mode of peer been observed
    bool _peerPluginModeObserved;

    // Mode of peer when observed,
    // not valid when _peerPluginModeObserved == false
    PluginMode _peerPluginMode;

    // Last message received of the given type from peer
    const SellMessage * _peerSellMessage;
    const BuyMessage * _peerBuyMessage;

    /**
     * ==============================================
     * SHARDED STATE BELOW, FACTOR OUT INTO BUYER AND
     * SELLER PEER PLUGIN WHEN THERE IS MORE CLARITY.
     * ==============================================
     */

    /**
     * Peer
     */


    /**
     * Client
     */

    /*
    // Sell client
    quint32 _clientSellerPrice; // price put in sell message

    // Buy client
    quint32 _clientBuyerPrice;
    quint32 _clientFee;
    qint32 _clientPeerBtcVersion;
    */

signals:

    //void peerPluginStatusUpdated(const PeerPluginStatus & status);

};

#endif
