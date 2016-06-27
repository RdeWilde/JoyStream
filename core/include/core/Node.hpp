/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <core/detail/detail.hpp>
//#include <core/controller/Stream.hpp>
#include <extension/extension.hpp>
#include <common/UnspentP2PKHOutput.hpp>
//#include <bitcoin/SPVWallet.hpp>

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
namespace detail {
    class Torrent;
}
namespace configuration {
    class Node;
    class Torrent;
}

class Torrent;

class Node : public QObject {

    Q_OBJECT

public:

    enum class State {

        // not yet started: ::start() not called
        stopped,

        //// all states below correpond to being started

        // after ::start(), but before libtorrent::listen_failed_alert or
        //
        waiting_to_listen,

        //
        //starting_libtorrent_session,

        //
        normal,

        /// States for when node is 'closing', as distinct from when it is actually
        /// closed, when it will be in State::stopped state.
        /// NB: Factor out into substate later

        //
        waiting_for_plugins_to_stop,

        //
        waiting_for_resume_data
    };

    typedef std::function<void(const libtorrent::tcp::endpoint &)> NodeStarted;
    typedef std::function<void(const libtorrent::tcp::endpoint &, libtorrent::error_code)> NodeStartFailed;

    typedef std::function<void()> NodeStopped;
    typedef std::function<void()> AddedTorrent;

    // typedef std::function<void()> ... something else

    Node(joystream::bitcoin::SPVWallet *);

    /**
     No public routines are *NOT* thread safe, so calls have to be on same thread as owner of controller.
     NB: Not strictly true, some may be, but there are no guarantees as of yet.
     */

    ~Node();

    /**
     start
     -------------

     DESCRIPTION
     Tries to start node, which results in
     both BitTorrent and DHT ports opening up, as well initializing the Bitcoin wallet.

     ARGUMENTS
     - configuration: Setup configurations for the node
     - nodeStarted: called if startup succeeds
     - nodeStartFailed: called if startup fails

     THROWS
     - exception::CanOnlyStartStoppedNode: if node is not in
     states Node::State::{stopped, waiting_for_plugins_to_stop,  waiting_for_resume_data}.

     SIGNALS
     - nodeStarted: if a successful start is made
     - nodeStartFailed: if starting was not successful
     */
    void start(const configuration::Node & configuration, const NodeStarted & nodeStarted, const NodeStartFailed & nodeStartFailed);

    /**
     stop
     -------------

     DESCRIPTION
     asynchrnous. .... All torrents are removed

     ARGUMENTS
     - callback about being actually stopped
     - callback about how stopping timedout out due to libtorrent foolishness

     THROWS
     - exception::CannotStopStoppedNode: if node is already being stopped

     SIGNALS
     - nodeStopped:
    */
    void stop(const NodeStopped &);

    /**
     addTorrent
     -------------

     DESCRIPTION
     Add torrent

     ARGUMENTS
     -
     -

     THROWS
     -

     SIGNALS
     -
     */
    void addTorrent(const configuration::Torrent &, const AddedTorrent &);

    /**
     removeTorrent
     -------------

     DESCRIPTION
     Remove torrent

     ARGUMENTS
     -
     -

     THROWS
     - no such torrent exists
     - is in the process of being removed?

     SIGNALS
     - removed: if
     -
     */
    void removeTorrent(const libtorrent::sha1_hash & info_hash);

    /**NB: Move out of controller and onto wallet interface **/
    void syncWallet();

    /**
     * Stram management stuff
     * THIS ROUTINE MUST BE HIDDEN FROM CONTROLLER USER IN THE FUTURE

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

    // State of controller
    State state() const;

    /**
    // Returns port server is presently listening on
    quint16 getServerPort() const;
    */

    // Get all info hashes
    std::set<libtorrent::sha1_hash> torrents() const;

    // Get torrents
    std::weak_ptr<Torrent> torrent(const libtorrent::sha1_hash & infoHash) const;

    // Configuration for current controller
    configuration::Node configuration() const;

signals:

    /**
     Signals are emitted for any change in state of the node.
     While the callbacks associated with the spesific calls above
     also report on potential failures with the corresponding calls only to the caller,
     these signals only notify about successful state changes, and
     anyone can subscribe. In other words, the former is for an actor,
     while the latter is for an observer. E.g. a HTTP daemon wrapping joystream::core library
     would use callbacks to service RPC calls, and signals to populate websocket streams.
     */

    // Starting was successful, and node is listening on given endpoint
    void nodeStarted(const libtorrent::tcp::endpoint &);

    // Starting was unsuccessful, and node could not listen to given endpoint for reason with given error
    void nodeStartFailed(const libtorrent::tcp::endpoint &, libtorrent::error_code);

    // Stopped node
    void nodeStopped();

    // Sent when libtorrent::add_torrent_alert is received from libtorrent
    void addedTorrent(const Torrent *);

    // A torrent was not added successfully according to libtorrent session giving
    // a libtorrent::add_torrent_alert p->error was done.
    void failedToAddTorrent(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);

    // Torrent with given info hash was removed
    void removedTorrent(const libtorrent::sha1_hash & info_hash);

    // Notify view
    // DROP!
    void torrentCheckedButHasNoPlugin(const libtorrent::torrent_info & torrentInfo, const libtorrent::torrent_status & torrentStatus);

    // Status update from underlying libtorrent session
    void pluginStatusUpdate(const extension::status::Plugin & status);

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
    void webSocketDisconnected();
    void scheduleReconnect();
    void onTransactionUpdated(Coin::TransactionId txid, int confirmations);
    void onWalletSynched();
    void onWalletSynchingHeaders();
    void onWalletSynchingBlocks();
    void onWalletConnected();

private:

    // State of controller
    State _state;

    static bool isStopping(State s);
	
    // Indicates if we are shutting down
    bool _closing;
    bool _reconnecting;
    int _protocolErrorsCount;
    std::vector<Coin::Transaction> _transactionSendQueue;

    // Underlying libtorrent session
    // Note: The only way to halt libtorrent::session
    // is to destruct the object, hence the only way for Node
    // to halt is to clear corresponding libtorrent::session,
    // this is why we keep pointer, rather than byvalue
    libtorrent::session * _session;

    // Wallet used
    joystream::bitcoin::SPVWallet * _wallet;

    // Plugin
    // We keep weak pointer ...
    std::weak_ptr<extension::Plugin> _plugin;

    // Timer which calls session.post_torrent_updates() at regular intervals
    // NB:**** use std:: alternative ***
    QTimer _torrentUpdateTimer;

    // Torrents added to session
    std::map<libtorrent::sha1_hash, detail::Torrent> _torrents;

    /// User supplied callbacks to be used as response in asynchronous method calls

    // Node::start
    NodeStarted _nodeStarted;
    NodeStartFailed _nodeStartFailed;

    // Node::stop
    NodeStopped _nodeStopped;

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
    // NB**: rename all to process(X), use overloading, is cleaner
    void processMetadataReceivedAlert(libtorrent::metadata_received_alert const * p);
    void processMetadataFailedAlert(libtorrent::metadata_failed_alert const * p);
    void process(libtorrent::listen_succeeded_alert const *p);
    void processListenFailedAlert(libtorrent::listen_failed_alert const * p);
    void processAddTorrentAlert(libtorrent::add_torrent_alert const * p);
    void processTorrentFinishedAlert(libtorrent::torrent_finished_alert const * p);
    void processStatusUpdateAlert(libtorrent::state_update_alert const * p);
    void processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p);
    void process(libtorrent::save_resume_data_alert const * p);
    void processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p);
    void processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p);
    void processTorrentCheckedAlert(libtorrent::torrent_checked_alert const * p);
    void processReadPieceAlert(const libtorrent::read_piece_alert * p);
    void processPieceFinishedAlert(const libtorrent::piece_finished_alert * p);

    // Processing (custome) plugin alerts
    template<class T>
    void process(const extension::alert::RequestResult<T> * p);
    void process(const extension::alert::BroadcastTransaction * p);
    void process(const extension::alert::PluginStatus * p);

    /**
    void processStartedSellerTorrentPlugin(const StartedSellerTorrentPlugin * p);
    void processStartedBuyerTorrentPlugin(const StartedBuyerTorrentPlugin * p);
    void processBuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert * p);
    void processSellerTorrentPluginStatusAlert(const SellerTorrentPluginStatusAlert * p);
    void processPluginStatusAlert(const PluginStatusAlert * p);
    void processSellerPeerAddedAlert(const SellerPeerAddedAlert * p);
    void processBuyerPeerAddedAlert(const BuyerPeerAddedAlert * p);
    void processSellerPeerPluginRemovedAlert(const SellerPeerPluginRemovedAlert * p);
    void processBuyerPeerPluginRemovedAlert(const BuyerPeerPluginRemovedAlert * p);
    */

    ///

    // Status
    void update(const std::vector<libtorrent::torrent_status> & statuses);
    void update(const libtorrent::torrent_status & status);

    // Start torrent plugin
    void startTorrentPlugin(const libtorrent::sha1_hash & info_hash);

    // Tell libtorrent try save resume data for all torrents needing it
    // Assumes 
    int requestResumeData();

    // Routine called after all resume data has been saved as part of an initlal begin_close() call
    void finalize_stop();

    // Save state of controller to file
    void saveStateToFile(const char * file);

    void sendTransactions();

    ///

    // Gives raw pointer to plugin
    extension::Plugin *  plugin();
};

}
}

#endif // CONTROLLER_HPP
