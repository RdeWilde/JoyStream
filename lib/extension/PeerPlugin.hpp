#ifndef PEER_PLUGIN_HPP
#define PEER_PLUGIN_HPP

#include "Request/PeerPluginRequest.hpp"
#include "BEPSupportStatus.hpp"
#include "Message/ExtendedMessageIdMapping.hpp"

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
#include <QDateTime>
#include <QQueue>

class TorrentPlugin;
class PeerPluginStatus;
class PeerPluginRequest;
class ExtendedMessagePayload;
class Observe;
class Buy;
class Sell;
class JoinContract;
class JoiningContract;
class SignRefund;
class RefundSigned;
class Ready;
class Payment;
class End;

/*
 * We inherit from QObject so we can send signals, and QObject must be first:
 * http://doc.trolltech.com/4.5/moc.html
 */

class PeerPlugin : public QObject, public libtorrent::peer_plugin {

    Q_OBJECT

public:

    // Constructor
    PeerPlugin(TorrentPlugin * plugin, libtorrent::bt_peer_connection * connection, QLoggingCategory & category);

    /**
     * All virtual functions below should ONLY be called by libtorrent network thread,
     * never by other threads, as this causes synchronization failures.
     */
    // Destructor
    virtual ~PeerPlugin() = 0;

    // Libtorrent callback
    virtual char const* type() const = 0;
    virtual void add_handshake(libtorrent::entry & handshake);
    virtual void on_disconnect(libtorrent::error_code const & ec) = 0;
    virtual void on_connected() = 0;
    virtual bool on_handshake(char const* reserved_bits);
    virtual bool on_extension_handshake(libtorrent::lazy_entry const & handshake);
    virtual bool on_have(int index) = 0;
    virtual bool on_bitfield(libtorrent::bitfield const & bitfield) = 0;
    virtual bool on_have_all() = 0;
    virtual bool on_reject(libtorrent::peer_request const & peerRequest) = 0;
    virtual bool on_request(libtorrent::peer_request const & peerRequest) = 0;
    virtual bool on_unchoke() = 0;
    virtual bool on_interested() = 0;
    virtual bool on_allowed_fast(int index) = 0;
    virtual bool on_have_none() = 0;
    virtual bool on_choke() = 0;
    virtual bool on_not_interested() = 0;
    virtual bool on_piece(libtorrent::peer_request const & piece, libtorrent::disk_buffer_holder & data) = 0;
    virtual bool on_suggest(int index) = 0;
    virtual bool on_cancel(libtorrent::peer_request const & peerRequest) = 0;
    virtual bool on_dont_have(int index) = 0;
    virtual void sent_unchoke() = 0;
    virtual bool can_disconnect(libtorrent::error_code const & ec) = 0;
    virtual bool on_extended(int length, int msg, libtorrent::buffer::const_interval body);
    virtual bool on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) = 0;
    virtual void on_piece_pass(int index) = 0;
    virtual void on_piece_failed(int index) = 0;
    virtual void tick() = 0;
    virtual bool write_request(libtorrent::peer_request const & peerRequest) = 0;

    /**
     * Subroutines for libtorrent thread.
     */

    // Processig routine for peer plugin requests, request pointer is owned by plugin dispatcher
    //void processPeerPluginRequest(const PeerPluginRequest * peerPluginRequest);

    // Sends extended message to peer, does not take ownership of pointer
    void sendExtendedMessage(const ExtendedMessagePayload & extendedMessage);

    // Determines the message type, calls correct handler, then frees message
    void processExtendedMessage(ExtendedMessagePayload * extendedMessage);

    // Getters
    bool peerTimedOut(int maxDelay) const;
    BEPSupportStatus peerBEP10SupportStatus() const;
    BEPSupportStatus peerBitSwaprBEPSupportStatus() const;
    libtorrent::tcp::endpoint endPoint() const;
    bool connectionAlive() const;
    bool lastPeerMessageWasMalformed() const;
    virtual bool lastPeerMessageWasStateCompatible() const = 0;

protected:

    // Torrent plugin for torrent
    TorrentPlugin * _plugin;

    // Connection to peer for this plugin
    libtorrent::bt_peer_connection * _connection;

    // Logging category
    QLoggingCategory & _category;

    // Endpoint
    libtorrent::tcp::endpoint _endPoint;

    // Time since last message was sent to peer, is used to judge if peer has timed out
    QTime _timeSinceLastMessageSent;

    // Plugin still has contact with peer
    bool _connectionAlive;

    // Last message arriving in on_extended() which was malformed accoridng
    // to ExtendedMessagePayload::fromRaw().
    bool _lastPeerMessageWasMalformed;

    // Processess message
    virtual void processObserve(const Observe * m) = 0;
    virtual void processBuy(const Buy * m) = 0;
    virtual void processSell(const Sell * m) = 0;
    virtual void processJoinContract(const JoinContract * m) = 0;
    virtual void processJoiningContract(const JoiningContract * m) = 0;
    virtual void processSignRefund(const SignRefund * m) = 0;
    virtual void processRefundSigned(const RefundSigned * m) = 0;
    virtual void processReady(const Ready * m) = 0;
    virtual void processPayment(const Payment * m) = 0;
    virtual void processEnd(const End * m) = 0;

    // Callback to subclass routine for handling a successfully extended handshake,
    // typicall by sending a mode message.
    virtual void extendedHandshakeCompleted() = 0;

private:

    // Indicates whether peer supports BEP10 and BitSwapr BEP respectively
    BEPSupportStatus _peerBEP10SupportStatus, _peerBitSwaprBEPSupportStatus;

    // Mapping from messages to BEP10 ID of peer
    ExtendedMessageIdMapping _clientMapping, _peerMapping;

};

#endif
