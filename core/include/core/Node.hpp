/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CORE_NODE_HPP
#define JOYSTREAM_CORE_NODE_HPP

#include <core/detail/detail.hpp>
#include <core/Callbacks.hpp>
//#include <core/controller/Stream.hpp>
#include <extension/extension.hpp>
#include <common/UnspentP2PKHOutput.hpp>

#include <libtorrent/session.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

#include <QObject>
#include <QTimer>
#include <QAbstractSocket> // is nested enum QAbstractSocket::SocketError

#include <functional>

namespace libtorrent {
    class peer_connection;
}

namespace joystream {
namespace bitcoin {
    class SPVWallet;
}
namespace extension {
    class Plugin;
}
namespace core {
namespace configuration {
    class Node;
    struct Torrent;
}

class Torrent;

class Node : public QObject {

    Q_OBJECT

public:

    enum class State {

        // After stopping has been completed, or before starting has been initiated for the first time.
        stopped,

        // After starting has been initaited, but no result, successfull or not.
        starting,

        // After starting has been initiated, and sucessfully completed.
        started,

        // After stopping has been initiated, but it has not yet been completed.
        stopping
    };


    Node(joystream::bitcoin::SPVWallet *);

    /**
     No public routines are *NOT* thread safe, so calls have to be on same thread as owner of controller.
     NB: Not strictly true, some may be, but there are no guarantees as of yet.
     */

    ~Node();

    /**
     @brief Tries to start node, which results in both BitTorrent and DHT ports opening up.
            Node must be in @State::stopped state.

     @param configuration Setup configurations for the node
     @param nodeStarted called if startup succeeds
     @param nodeStartFailed called if startup fails
     @throws exception::StateIncompatibleOperation if node is not already State::stopped.
     @signal nodeStarted if a successful start is made
     @signal nodeStartFailed if starting was not successful
     */
    void start(const configuration::Node & configuration, const NodeStarted & nodeStarted, const NodeStartFailed & nodeStartFailed);

    /**
     @brief All plugins are stopped, connections are then terminated,
            however they are still part present in session, and  BitTorrent and DHT daemons still run.
            Node must be in @State::started state.
            **Due to Libtorrent does not permit stopping daemons wihtout killing session**

     @param nodeStopped callback about being actually stopped
     @throws exception::StateIncompatibleOperation: if node is not already State::started.
     @throws all exceptions thrown by joystream::protocol_session::Stop
     @signal nodeStopped
    */
    void stop(const NodeStopped & nodeStopped);

    /**
     @brief Tries to add torrent.

     @param configuration
     @param addedTorrent
     @throws exception::TorrentAlreadyExists if torrent already has been added
     @throws exception::StateIncompatibleOperation if node not in mode @State::started
     @signal addedTorrent
     */
    void addTorrent(const configuration::Torrent & configuration, const AddedTorrent & addedTorrent);

    /**
     @brief Tries to remove torrent.

     @param info_hash info_hash of torrent
     @param RemoveTorrent callback when operation is completed
     @throws exception::StateIncompatibleOperation if node not in mode @State::started
     @throws exception::NoSuchTorrentExists if no such torrent exists
     @signal removedTorrent
     */
    void removeTorrent(const libtorrent::sha1_hash & info_hash, const RemovedTorrent & handler);

    /**NB: Move out of controller and onto wallet interface **/
    void syncWallet();

    /**
     * Stream management

    // If there is a torrent for the given info hash, then the given stream is added to the
    // torrents streams set, and lastly a handle for the torrent is removed. Otherwise,
    // same rules as getTorrentHandle apply.
    libtorrent::torrent_handle registerStream(Stream * stream);

    // Removes stream registration
    void unRegisterStream(Stream * stream);
    void unRegisterStream(Stream * stream, Stream::Error error);

    // If torrent corresponding to the given info hash has a buyer torrent plugin installed,
    // then the plugin is requested to alter download location
    void changeDownloadingLocationFromThisPiece(const libtorrent::sha1_hash & infoHash, int pieceIndex);
    */

    // Triggers an update of the status of all torrents,
    // and any changes in state will be emitted as signals.
    void updateStatus();

    // State of controller
    State state() const;

    // Get all info hashes
    std::set<libtorrent::sha1_hash> torrents() const;

    // Get torrents
    std::weak_ptr<Torrent> torrent(const libtorrent::sha1_hash & infoHash) const;    

    // Configuration for current controller
    configuration::Node configuration() const;

signals:

    // Signals are emitted for any change in state of the node.
    // While the callbacks associated with the spesific calls above
    // also report on potential failures with the corresponding calls only to the caller,
    // these signals only notify about successful state changes, and
    // anyone can subscribe. In other words, the former is for an actor,
    // while the latter is for an observer. E.g. a HTTP daemon wrapping joystream::core library
    // would use callbacks to service RPC calls, and signals to populate websocket streams.

    // Starting was successful, and node is listening on given endpoint
    void nodeStarted(const libtorrent::tcp::endpoint &);

    // Stopped node
    void nodeStopped();

    // Sent when libtorrent::add_torrent_alert is received from libtorrent
    void addedTorrent(const std::shared_ptr<Torrent> &);

    // Torrent with given info hash was removed
    void removedTorrent(const libtorrent::sha1_hash & info_hash);

private slots:

    /**
     * Handlers for TCP streamng server

    // Checks server for pending connections
    void handleConnection();

    // Checks server for pending error
    void handleAcceptError(QAbstractSocket::SocketError socketError);

    // Streaming server signals
    void registerStream(const Stream * handler);
    void handleFailedStreamCreation(QAbstractSocket::SocketError socketError);

    // Stream signal
    void registerRequestedPathOnStream(const Stream * handler, const QByteArray & requestedPath) const;

    // Will ask libtorrent to read given piece
    void readPiece(int piece);
    */

    // Move all of these out of controller later
    void scheduleReconnect();
    void onTransactionUpdated(Coin::TransactionId txid, int confirmations);
    void onWalletSynched();
    void onWalletSynchingHeaders();
    void onWalletSynchingBlocks();
    void onWalletConnected();

private:

    // State of controller
    State _state;
	
    // Indicates if we are shutting down
    bool _closing;
    bool _reconnecting;
    int _protocolErrorsCount;
    std::vector<Coin::Transaction> _transactionSendQueue;

    // Underlying libtorrent session
    libtorrent::session _session;

    // Wallet used
    joystream::bitcoin::SPVWallet * _wallet;

    // Plugin
    boost::shared_ptr<extension::Plugin> _plugin;

    // Torrents added to session
    // A shared pointer is used, in order to give weak pointers as
    // public handle. This is safe, sinc user and this object is maintained
    // by same thread.
    //
    // NB: This container should not be used as reliable indication of
    // state inside this class, it should simply be updated based on inbound
    // libtorrent alerts. To check on torrents, this object should use the libtorrent
    // session itself.
    std::map<libtorrent::sha1_hash, std::shared_ptr<Torrent>> _torrents;

    /// User supplied callbacks to be used as response in asynchronous method calls

    // Node::start
    NodeStarted _nodeStarted;
    NodeStartFailed _nodeStartFailed;

    // TCP streaming server
    //QTcpServer _streamingServer;

    /// Libtorrent entry points for libtorrent::alert processing
    // Entry point for callback from libtorrent, warning about 0->1 alert in queue.
    // NB: Do not under any circumstance have a call to libtorrent in this routine, since the network
    // thread in libtorrent will be making this call, and a new call will result in a dead lock.
    void libtorrent_alert_notification_entry_point();

    /// Alert processing routines

    // Process all pending alerts in the libtorrent queue
    Q_INVOKABLE void processAlertQueue();

    // Processes a spesific alert
    void processAlert(const libtorrent::alert * a);

    // Processing (standard) libtorrent alerts of given type
    void process(const libtorrent::listen_succeeded_alert *);
    void process(const libtorrent::listen_failed_alert *);
    void process(const libtorrent::metadata_received_alert *);
    void process(const libtorrent::metadata_failed_alert *);
    void process(const libtorrent::add_torrent_alert *);
    void process(const libtorrent::torrent_finished_alert *);
    void process(const libtorrent::state_update_alert *);
    void process(const libtorrent::torrent_removed_alert *);
    void process(const libtorrent::torrent_resumed_alert *);
    void process(const libtorrent::save_resume_data_alert *);
    void process(const libtorrent::save_resume_data_failed_alert * p);
    void process(const libtorrent::torrent_paused_alert *);
    void process(const libtorrent::torrent_checked_alert *);
    // peer connect: peer_connect_alert
    // peer disconnect: peer_disconnected_alert
    void processReadPieceAlert(const libtorrent::read_piece_alert *);
    void processPieceFinishedAlert(const libtorrent::piece_finished_alert *);

    // when is upload & download lmit refreshed ?

    // Processing (plugin) alerts
    void process(const extension::alert::RequestResult * p);
    void process(const extension::alert::BroadcastTransaction * p);
    void process(const extension::alert::PluginStatus * p);

    // torrent plugin added
    // torrent plugin removed

    void sendTransactions();

    // Generate session settings pack used with libtorrent
    static libtorrent::settings_pack session_settings() noexcept;

    // Generates dht settings used with libtorrent
    static libtorrent::dht_settings dht_settings() noexcept;
};

}
}

#endif // JOYSTREAM_CORE_NODE_HPP
