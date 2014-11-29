
#ifndef PEER_PLUGIN_HPP
#define PEER_PLUGIN_HPP

#include <libtorrent/extensions.hpp>
#include <libtorrent/peer_connection.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/bitfield.hpp>
#include <libtorrent/peer_request.hpp>
#include <libtorrent/disk_buffer_holder.hpp>
#include <libtorrent/buffer.hpp>

#include <QObject>

#include <boost/asio/ip/tcp.hpp> // ip::tcp::endpoint

#include "PeerPluginStatus.hpp"

// Forward declaration
class TorrentPlugin;
class PeerPluginStatus;

// Names of all messages
//extern const char * message_names[];



/**
  * ALMOST CERTAINLY, THIS CLASS WILL
  * HAVE TO BE FACTORED INTO TWO VERSIONS OF
  * THE PEER PLUGIN, ONE BEHAVING AS A SEELER,
  * AND ONE BEHAVING AS A BUYER, BUT LETS POSTPONE
  * THAT UNTIL IT BECOMES CLEAR THAT THIS IS IN
  * FACT WORTH IT
  */

// We inherit from QObject so we can send signals, and QObject must be first:
// http://doc.trolltech.com/4.5/moc.html
class PeerPlugin : public QObject, public libtorrent::peer_plugin {

    Q_OBJECT

public:

    // BEP support state indicator, is used for both regular BitTorrent handshake
    // and BEP10 handshake.
    enum PEER_BEP_SUPPORTED_STATUS {

        // Before handshake
        unknown,

        // After handshake
        supported,
        not_supported
    };

    // Role of a peer
    enum PEER_ROLE {

        // Sent buy message as first message after extended handshake
        buyer,

        // Sent sell message as first message after extended handshake
        seller
    };

    enum BUYER_CHANNEL_STATE {

    };

    enum SELLER_CHANNEL_STATE {

    };

    // Constructor
    PeerPlugin(TorrentPlugin * torrentPlugin,
               libtorrent::peer_connection * peerConnection,
               QLoggingCategory & category,
               PEER_ROLE role);

    // Destructor
    ~PeerPlugin();

    /**
     * All virtual functions below should ONLY
     * be called by libtorrent network thread,
     * never by other threads, as this causes synchronization
     * failures.
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

    /**
     * Public routines used by non-libtorrent thread
     */
    libtorrent::sha1_hash getInfoHash();

    PEER_BEP_SUPPORTED_STATUS getPeerBEP10SupportedStatus();
    PEER_BEP_SUPPORTED_STATUS getPeerBEP43SupportedStatus();

private:

    // Torrent plugin for torrent
    TorrentPlugin * torrentPlugin_;

    // Connection to peer for this plugin
    libtorrent::peer_connection * peerConnection_;

    // Indicates whether peer supports
    PEER_BEP_SUPPORTED_STATUS peerBEP10SupportedStatus, // BEP10
                        peerBEP43SupportedStatus; // BEP43

    // Indicates role of peer
    PEER_ROLE peerRole_, clientRole_;

    // Mapping from messages to BEP10 ID of peer
    unsigned int peerMessageMapping[NUMBER_OF_MESSAGES],
                    clientMessageMapping[NUMBER_OF_MESSAGES];

    // Logging category
    QLoggingCategory & category_;

signals:

    void peerAdded(PeerPlugin * peerPlugin);

    void updatePeerPluginStatus(PeerPluginStatus status);

};

#endif
