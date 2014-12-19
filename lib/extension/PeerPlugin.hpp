#ifndef PEER_PLUGIN_HPP
#define PEER_PLUGIN_HPP

#include "PeerPluginConfiguration.hpp"
#include "PeerPluginRequest/PeerPluginRequest.hpp"

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

// Forward declaration
class TorrentPlugin;
class PeerPluginStatus;
class PeerPluginRequest;

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
    void processPeerPluginRequest(const PeerPluginRequest * peerPluginRequest);

    // Torrent plugin calls to start
    void startPlugin(StartedPluginMode pluginMode);

    //void setConfiguration(PeerPluginConfiguration * peerPluginConfiguration);


    // Getters
    ExtendedMessageIdMapping & getPeerMapping();
    void setPeerMapping(const ExtendedMessageIdMapping & peerMapping);

    ExtendedMessageIdMapping & getClientMapping();
    void setClientMapping(const ExtendedMessageIdMapping & clientMapping);

    BEPSupportStatus & getPeerBEP10SupportedStatus();
    void setPeerBEP10SupportedStatus(const BEPSupportStatus & peerBEP10SupportedStatus);

    BEPSupportStatus & getPeerBEP43SupportedStatus();
    void setPeerBEP43SupportedStatus(const BEPSupportStatus & peerBEP43SupportedStatus);

    PeerPluginState & getPeerPluginState();
    void setPeerPluginState(const PeerPluginState & peerPluginState);

    PeerPluginId & getPeerPluginId();
    void setPeerPluginId(const PeerPluginId & peerPluginId);

protected:

    // Torrent plugin for torrent
    TorrentPlugin * _torrentPlugin;

    // Connection to peer for this plugin
    libtorrent::bt_peer_connection * _bittorrentPeerConnection;

    // Logging category
    QLoggingCategory & _category;

    // Endpoint
    libtorrent::tcp::endpoint _endPoint;

    // Indicates if plugin has been started
    // Before this becomes true, plugin will
    // not do anythng which compromises eventually
    // going into seller or buyer mode
    bool _pluginStarted;

    // Mode of plugin when started
    StartedPluginMode _pluginStartedMode;

    // Mapping from messages to BEP10 ID of peer
    ExtendedMessageIdMapping _clientMapping, _peerMapping;

    // Indicates whether peer supports
    BEPSupportStatus _peerBEP10SupportedStatus, // BEP10
                        _peerBEP43SupportedStatus; // BEP43

    // State of peer plugin
    PeerPluginState _peerPluginState;

    // Id of this peer plugin
    PeerPluginId _peerPluginId; // assess later, is the redundancy worth it

signals:

    //void peerPluginStatusUpdated(const PeerPluginStatus & status);

};

#endif
