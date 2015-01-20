#ifndef PEER_PLUGIN_HPP
#define PEER_PLUGIN_HPP

#include "PeerPluginConfiguration.hpp"
#include "Request/PeerPluginRequest.hpp"
#include "PeerAction.hpp"
#include "BitCoin/PublicKey.hpp"
#include "BitCoin/Hash.hpp"
#include "BitCoin/Signature.hpp"

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
    PeerPlugin(TorrentPlugin * torrentPlugin,
               libtorrent::bt_peer_connection * btConnection,
               QLoggingCategory & category);

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
    virtual bool on_extended(int length, int msg, libtorrent::buffer::const_interval body) = 0;
    virtual bool on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) = 0;
    virtual void on_piece_pass(int index) = 0;
    virtual void on_piece_failed(int index) = 0;
    virtual void tick() = 0;
    virtual bool write_request(libtorrent::peer_request const & peerRequest) = 0;

    /**
     * Subroutines for libtorrent thread.
     */

    // Process messages in unprocessed message queue
    void processUnprocessedMessages();

    // Processig routine for peer plugin requests, request pointer is owned by plugin dispatcher
    void processPeerPluginRequest(const PeerPluginRequest * peerPluginRequest);

    // Sends extended message to peer
    // does not take ownership of pointer
    void sendExtendedMessage(const ExtendedMessagePayload & extendedMessage);

    // Determines the message type, calls correct handler, then frees message
    void processExtendedMessage(ExtendedMessagePayload * extendedMessage);

    // Processess message, return whether or not message was valid, that is
    // 1) was compatible with last action of peer
    // 2) was premature, i.e. came before we had even sent a preconditional message
    bool processObserve(const Observe * m);
    bool processBuy(const Buy * m);
    bool processSell(const Sell * m);
    bool processJoinContract(const JoinContract * m);
    bool processJoiningContract(const JoiningContract * m);
    bool processSignRefund(const SignRefund * m);
    bool processRefundSigned(const RefundSigned * m);
    bool processReady(const Ready * m);
    bool processPayment(const Payment * m);
    bool processEnd(const End * m);

    void sendStatusToController();

    // Utilitliy
    bool peerTimedOut(int maxDelay) const;

    // Getters
    BEPSupportStatus peerBEP10SupportStatus() const;
    BEPSupportStatus peerBEP43SupportStatus() const;
    PeerAction peerState() const;
    libtorrent::tcp::endpoint endPoint() const;
    bool isConnected() const;
    bool peerSentInvalidMessage() const;

protected:

    // Torrent plugin for torrent
    TorrentPlugin * _torrentPlugin;

    // Connection to peer for this plugin
    libtorrent::bt_peer_connection * _bittorrentPeerConnection;

    // Logging category
    QLoggingCategory & _category;

    // Endpoint
    libtorrent::tcp::endpoint _endPoint;

    // Queue of received valid messages befor plugin started.
    // Enter through on_extended(), are processed when plugin starts
    // REMOVE LATER WHEN PLUGIN IS ACTIVE FROM GET GO.
    QQueue<ExtendedMessagePayload *> _beforePluginStartsMessageQueue;

    // Time since last message was sent to peer, is used to judge if peer has timed out
    QTime _timeSinceLastMessageSent;

    // Connection status with peer
    bool _isConnected;

    // Peer sent an invalid message
    // This could either be because
    // a) peer sent a message which was not well formed
    // b) peer sent a message which was incompatible with mode
    // c) peer sent a message which was incompatible with state of interaction
    bool _lastPeerMessageWasValid;

    // Type of last message client sent to peer
    MessageType _lastMessageSent;

private:

    // Mapping from messages to BEP10 ID of peer
    ExtendedMessageIdMapping _clientMapping, _peerMapping;

    // Indicates whether peer supports
    BEPSupportStatus _peerBEP10SupportedStatus, // BEP10
                        _peerBitSwaprBEPSupportedStatus ; // BitSwapr BEP
};

#endif
