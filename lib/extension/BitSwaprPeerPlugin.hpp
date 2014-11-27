
#ifndef BITSWAPR_PEER_PLUGIN_HPP
#define BITSWAPR_PEER_PLUGIN_HPP

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
#include <QLoggingCategory>

// MOC declarations
#include <QMetaType>
//Q_DECLARE_METATYPE(BitSwaprPeerPlugin *)

#include <boost/asio/ip/tcp.hpp> // ip::tcp::endpoint

// Used directing logging to category object.
#define CATEGORY (*category_)

// Forward declaration
class BitSwaprTorrentPlugin;

// Names of all messages
extern const char * message_names[];

// Number of messages
#define NUMBER_OF_MESSAGES 15

// We inherit from QObject so we can send signals, and QObject must be first:
// http://doc.trolltech.com/4.5/moc.html
class BitSwaprPeerPlugin : public QObject, public libtorrent::peer_plugin {

    Q_OBJECT

    // BEP support state indicator
    enum PEER_BEP_SUPPORTED_STATUS {
        unknown,
        supported,
        not_supported
    };

private:

    // Torrent plugin for torrent
    BitSwaprTorrentPlugin * torrentPlugin_;

    // Connection to peer for this plugin
    libtorrent::peer_connection * peerConnection_;

    // Indicates whether peer supports BEP10
    PEER_BEP_SUPPORTED_STATUS peerBEP10SupportedStatus;

    // Indicates whether peer supports BEP43
    PEER_BEP_SUPPORTED_STATUS peerBEP43SupportedStatus;

    // Mapping from messages to BEP10 ID of peer
    unsigned int peerMessageMapping[NUMBER_OF_MESSAGES],
                    clientMessageMapping[NUMBER_OF_MESSAGES];

    // Logging category
    QLoggingCategory * category_;

public:

    // Constructor
    BitSwaprPeerPlugin(BitSwaprTorrentPlugin * torrentPlugin, libtorrent::peer_connection * peerConnection, QLoggingCategory * category);

    // Destructor
    ~BitSwaprPeerPlugin();

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

signals:

    /*
     * Notifying controller
     */
/*
    class PeerHandle {

        public:

        libtorrent::sha1_hash info_hash;

        boost::ip::tcp::endpoint

    };
*/

    void peerAdded(BitSwaprPeerPlugin * peerPlugin);

};

#endif
