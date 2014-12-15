#ifndef PEER_PLUGIN_HPP
#define PEER_PLUGIN_HPP

#include "PeerPluginConfiguration.hpp"

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
               QLoggingCategory & category,
               PeerPluginConfiguration & peerPluginConfiguration);

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

protected:

    // Torrent plugin for torrent
    TorrentPlugin * _torrentPlugin;

    // Connection to peer for this plugin
    libtorrent::bt_peer_connection * _bittorrentPeerConnection;

    // Logging category
    QLoggingCategory & _category;

    // Configuration for peer: Reference to object in torrent configuration
    PeerPluginConfiguration & _peerPluginConfiguration;

signals:

    //void peerPluginStatusUpdated(const PeerPluginStatus & status);

};

#endif
