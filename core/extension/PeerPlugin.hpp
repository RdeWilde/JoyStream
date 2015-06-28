/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

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
class RequestFullPiece;
class FullPiece;
class Payment;



enum class PluginMode;

/**
 * We inherit from QObject so we can send signals, and QObject must be first:
 * http://doc.trolltech.com/4.5/moc.html
 */

/**
 * @brief Abstract parent type for peer plugins, in seller, buyer and observer mode.
 */
class PeerPlugin : public QObject, public libtorrent::peer_plugin {

    Q_OBJECT

public:

    // Mode which has been announced by a peer
    enum class PeerModeAnnounced {
        none,
        observer,
        seller,
        buyer
    };


    class Status {

    public:

        // Defualt constructor
        Status();

        // Constructor from members
        Status(PeerModeAnnounced peerModeAnnounced, BEPSupportStatus peerBEP10SupportStatus, BEPSupportStatus peerJoyStreamBEPSupportStatus);

        // Getters and setters
        virtual PluginMode pluginMode() const = 0;

        PeerModeAnnounced peerModeAnnounced() const;
        void setPeerModeAnnounced(PeerModeAnnounced peerModeAnnounced);

        BEPSupportStatus peerBEP10SupportStatus() const;
        void setPeerBEP10SupportStatus(BEPSupportStatus peerBEP10SupportStatus);

        BEPSupportStatus peerJoyStreamBEPSupportStatus() const;
        void setPeerBitSwaprBEPSupportStatus(BEPSupportStatus peerJoyStreamBEPSupportStatus);

    private:

        // Announced peer mode
        PeerModeAnnounced _peerModeAnnounced;

        // Indicates whether peer supports BEP10
        BEPSupportStatus _peerBEP10SupportStatus;

        // Indicates whether peer supports BEP43 .. BitSwapr
        BEPSupportStatus _peerBitSwaprBEPSupportStatus;

        // Net amount of funds (satoshies) sent
        quint64 _balance;
    };

    // Constructor
    PeerPlugin(TorrentPlugin * plugin,
               libtorrent::bt_peer_connection * connection,
               bool scheduledForDeletingInNextTorrentPluginTick,
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
    libtorrent::bt_peer_connection * connection();

    bool peerTimedOut(int maxDelay) const;
    BEPSupportStatus peerBEP10SupportStatus() const;
    BEPSupportStatus peerBitSwaprBEPSupportStatus() const;
    libtorrent::tcp::endpoint endPoint() const;

    bool connectionAlive() const;
    bool lastReceivedMessageWasMalformed() const;
    virtual PluginMode mode() const = 0;
    virtual quint64 balance() const = 0;

    PeerModeAnnounced peerModeAnnounced() const;
    void setPeerModeAnnounced(PeerModeAnnounced peerModeAnnounced);

    bool scheduledForDeletingInNextTorrentPluginTick() const;
    void setScheduledForDeletingInNextTorrentPluginTick(bool scheduledForDeletingInNextTorrentPluginTick);

    libtorrent::error_code deletionErrorCode() const;
    void setDeletionErrorCode(const libtorrent::error_code &deletionErrorCode);

private:

    // Torrent plugin for torrent
    TorrentPlugin * _plugin;

protected:

    // Connection to peer for this plugin
    libtorrent::bt_peer_connection * _connection;

    // Endpoint: can be deduced from connection, but is worth keeping if connection pointer becomes invalid
    libtorrent::tcp::endpoint _endPoint;

    // Announced peer mode
    PeerModeAnnounced _peerModeAnnounced;

    // Time since last message was sent to peer, is used to judge if peer has timed out
    QTime _timeSinceLastMessageSent;

    // Last message arriving in on_extended() which was malformed accoridng
    // to ExtendedMessagePayload::fromRaw().
    bool _lastReceivedMessageWasMalformed;

    // Last message was not compatible with state of plugin
    bool _lastMessageWasStateIncompatible;

    // Set when peer plugin should be disconnected and deleted by corresponding torrent plugin
    bool _scheduledForDeletingInNextTorrentPluginTick;
    libtorrent::error_code _deletionErrorCode;

    // Mapping from messages to BEP10 ID of client
    ExtendedMessageIdMapping _clientMapping;

    // Mapping from messages to BEP10 ID of peer
    ExtendedMessageIdMapping _peerMapping;

    // Indicates whether peer supports BEP10
    BEPSupportStatus _peerBEP10SupportStatus;

    // Indicates whether peer supports BEP43 .. BitSwapr
    BEPSupportStatus _peerBitSwaprBEPSupportStatus;

    // Logging category
    QLoggingCategory & _category;

    // Processess message in subclass,
    // are only called if extended handshake has been completed successfully.
    virtual void processObserve(const Observe * m) = 0;
    virtual void processBuy(const Buy * m) = 0;
    virtual void processSell(const Sell * m) = 0;
    virtual void processJoinContract(const JoinContract * m) = 0;
    virtual void processJoiningContract(const JoiningContract * m) = 0;
    virtual void processSignRefund(const SignRefund * m) = 0;
    virtual void processRefundSigned(const RefundSigned * m) = 0;
    virtual void processReady(const Ready * m) = 0;
    virtual void processRequestFullPiece(const RequestFullPiece * m) = 0;
    virtual void processFullPiece(const FullPiece * m) = 0;
    virtual void processPayment(const Payment * m) = 0;

};

#endif
