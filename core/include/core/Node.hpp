/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

//#include <core/controller/Stream.hpp>
#include <common/UnspentP2PKHOutput.hpp>

#include <libtorrent/session.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

#include <QObject>
#include <QTimer>
#include <QAbstractSocket> // is nested enum QAbstractSocket::SocketError

/**
class QNetworkAccessManager;
class TorrentStatus;
class TorrentPluginStartedAlert;
class PluginStatusAlert;
class BuyerTorrentPluginStatusAlert;
class SellerTorrentPluginStatusAlert;
class StartedSellerTorrentPlugin;
class StartedBuyerTorrentPlugin;
class SellerPeerAddedAlert;
class BuyerPeerAddedAlert;
class SellerPeerPluginRemovedAlert;
class BuyerPeerPluginRemovedAlert;
class BroadcastTransactionAlert;
*/

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
    class Controller;
    class Torrent;
}
namespace viewmodel {
    // class Controller? <--- do we need this
    class Torrent;
}

class Controller : public QObject {

    Q_OBJECT

public:

    enum class State {

        // not yet started: ::start() not called
        stopped,

        //// all states below correpond to being started **/

        //
        //starting_libtorrent_session,

        //
        normal,

        //
        waiting_for_resume_data_while_closing
    };

    Controller(const configuration::Controller &,
               joystream::bitcoin::SPVWallet *);

    //// No public routines are thread safe, so calls have to be on same thread as owner of controller.

    ~Controller();

    // Start session
    void start(const configuration::Controller &);

    // Stop session
    void stop(); // substitute close, add callback,

    // Add torrent
    bool addTorrent(const configuration::Torrent &);
    bool addTorrent(const configuration::Torrent &, const SellerTorrentPlugin::Configuration & pluginConfiguration);
    bool addTorrent(const configuration::Torrent &, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo);
    //bool addTorrent(const configuration::Torrent &, const ObserverTorrentPlugin::Configuration & pluginConfiguration);

    // Start torrent plugin
    //void startTorrentPlugin(const libtorrent::sha1_hash & info_hash, const TorrentPlugin::Configuration * configuration);
    void startSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPlugin::Configuration & pluginConfiguration);
    void startBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo);
    //void startObserverTorrentPlugin(const libtorrent::sha1_hash & info_hash, const ObserverTorrentPlugin::Configuration & pluginConfiguration);

    // Stops libtorrent session, and tries to save_resume data, when all resume data is saved, finalize_close() is called.
    void begin_close();

    // Returns torrent handle for torrent with give info hash, if no such torrent has been registered
    // then an invalid handle is passed
    //libtorrent::torrent_handle getTorrentHandle(const libtorrent::sha1_hash & infoHash) const;

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

    //// Getters

    // State of controller
    State state() const;

    /**
    // Returns port server is presently listening on
    quint16 getServerPort() const;
    */

    // Mapping of info_hashes to torrents
    //std::set<libtorrent::sha1_hash, detail::Torrent *> torrents() const;

    // Get torrents
    const viewmodel::Torrent * torrent(const libtorrent::sha1_hash & infoHash) const;

    // Configuration for current controller
    configuration::Controller configuration() const;

    /**
    // Fund the wallet
    void fundWallet(uint64_t value);
    */

    bool closing() const { return _closing; }

public slots:

    /**
     * View entry points
     * =================
     * Are not thread safe. Primarily called by view objects on the same thread as controller thread,
     * buy also good routines to use for testing.
     */

    void pauseTorrent(const libtorrent::sha1_hash & info_hash);
    void startTorrent(const libtorrent::sha1_hash & info_hash);
    void removeTorrent(const libtorrent::sha1_hash & info_hash);

    void syncWallet();

private slots:

    /**
     * Intra Controller entry points
     * =================
     * Typically parts of controller,
     * remove this at a later time
     */

    // Tells session to post updates, is signaled by timer
    void callPostTorrentUpdates();

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

    void webSocketDisconnected();
    void scheduleReconnect();
    void onTransactionUpdated(Coin::TransactionId txid, int confirmations);
    void onWalletSynched();
    void onWalletSynchingHeaders();
    void onWalletSynchingBlocks();
    void onWalletConnected();

signals:

    // Sent when libtorrent::add_torrent_alert is received from libtorrent
    void addedTorrent(const viewmodel::Torrent *);

    // Torrent with given info hash was removed
    void torrentRemoved(const libtorrent::sha1_hash & info_hash);

    // A torrent was not added successfully according to libtorrent session giving
    // a libtorrent::add_torrent_alert p->error was done.
    void failedToAddTorrent(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);

    // A torrent plugin was started
    // *** DISCONTINUED, THESE ARE INSTALLED AS SIGNALS ON TORRENT VIEW MODELS
    //void torrentPluginStarted();
    //void startedSellerTorrentPlugin(SellerTorrentPluginViewModel * model);
    //void startedBuyerTorrentPlugin(BuyerTorrentPluginViewModel * model);

    // Notify view
    void torrentCheckedButHasNoPlugin(const libtorrent::torrent_info & torrentInfo, const libtorrent::torrent_status & torrentStatus);

    // Status update from underlying libtorrent session
    void pluginStatusUpdate(const Plugin::Status & status);

    // Emitted after finalize_close(), that is when controller is 100% done
    void closed();

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
    // We keep weak pointer
    std::weak_ptr<extension::Plugin> _plugin;

    // Timer which calls session.post_torrent_updates() at regular intervals
    // NB:**** use std:: alternative ***
    QTimer _statusUpdateTimer;

    // Torrents added to session
    // Has to be pointer since since its Torrent::model (TorrentViewModel) isQObject type.
    // Object is entirely owned by this.
    std::set<libtorrent::sha1_hash, detail::Torrent *> _torrents;

    // TCP streaming server
    //QTcpServer _streamingServer;

    /**
     *
     * SIMPLIFY LATER: Put TorrentPlugin::Configuration pointer into Torrent.
     * Do not refer to it as *pending*, this will bethe configurations the
     * view is given when any editing of configruations is done, and
     * which will be pased on to plugin as alert.
     *
     * In the case of buyer, put utxo in QMap for keeping pending utxo. They
     * are indeed pending, and are therefore not part of Torrent.
     *
     *
     */

    /**
    * Libtorrent entry points for libtorrent::alert processing
    * =================
    * All Q_INVOKABLE routines are not thread safe, and must be invoke through event loop
    * or on same thread as controller owner, the rest should be thread safe.
    */
    // Process all pending alerts in the libtorrent queue
    //Q_INVOKABLE void processAlertQueue();

    // Process a spesific request
    Q_INVOKABLE void processAlert(const libtorrent::alert * a);

    // Callback routine called by libtorrent dispatcher routine
    //
    // CRITICAL:
    // Do not under any circumstance have a call to libtorrent in this routine, since the network
    // thread in libtorrent will be making this call, and a new call will result in a dead lock.
    //
    void libtorrent_alert_dispatcher_callback(const std::auto_ptr<libtorrent::alert> & alertAutoPtr);
    void libtorrent_entry_point_alert_notification();

    // Configurations are placed in these maps when corresponding torrent is added to session,
    // and they are used to start a plugin on the given torrent once a torrent_checked_alert has been
    // issued by session.
    //QMap<libtorrent::sha1_hash, const TorrentPlugin::Configuration *> _pendingConfigurations;
    QMap<libtorrent::sha1_hash, SellerTorrentPlugin::Configuration> _pendingSellerTorrentPluginConfigurations;
    QMap<libtorrent::sha1_hash, QPair<BuyerTorrentPlugin::Configuration, Coin::UnspentP2PKHOutput> > _pendingBuyerTorrentPluginConfigurationAndUtxos;
    //QMap<libtorrent::sha1_hash, UnspentP2PKHOutput> _pendingBuyerTorrentPluginUtxos;
    //QMap<libtorrent::sha1_hash, ObserverTorrentPlugin::Configuration> _pendingObserverTorrentPluginConfigurations;

    // Routine for processig libtorrent alerts
    // NB**: rename all to process(X), use overloading, is cleaner
    void processMetadataReceivedAlert(libtorrent::metadata_received_alert const * p);
    void processMetadataFailedAlert(libtorrent::metadata_failed_alert const * p);
    void processListenFailedAlert(libtorrent::listen_failed_alert const * p);
    void processAddTorrentAlert(libtorrent::add_torrent_alert const * p);
    void processTorrentFinishedAlert(libtorrent::torrent_finished_alert const * p);
    void processStatusUpdateAlert(libtorrent::state_update_alert const * p);
    void processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p);
    void processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p);
    void processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p);
    void processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p);
    void processTorrentCheckedAlert(libtorrent::torrent_checked_alert const * p);
    void processReadPieceAlert(const libtorrent::read_piece_alert * p);
    void processPieceFinishedAlert(const libtorrent::piece_finished_alert * p);
    void processStartedSellerTorrentPlugin(const StartedSellerTorrentPlugin * p);
    void processStartedBuyerTorrentPlugin(const StartedBuyerTorrentPlugin * p);
    void processBuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert * p);
    void processSellerTorrentPluginStatusAlert(const SellerTorrentPluginStatusAlert * p);
    void processPluginStatusAlert(const PluginStatusAlert * p);
    void processSellerPeerAddedAlert(const SellerPeerAddedAlert * p);
    void processBuyerPeerAddedAlert(const BuyerPeerAddedAlert * p);
    void processSellerPeerPluginRemovedAlert(const SellerPeerPluginRemovedAlert * p);
    void processBuyerPeerPluginRemovedAlert(const BuyerPeerPluginRemovedAlert * p);
    void processBroadcastTransactionAlert(const BroadcastTransactionAlert *p);

    // Status
    void update(const std::vector<libtorrent::torrent_status> & statuses);
    void update(const libtorrent::torrent_status & status);

    // Start torrent plugin
    void startTorrentPlugin(const libtorrent::sha1_hash & info_hash);

    // Tell libtorrent try save resume data for all torrents needing it
    int makeResumeDataCallsForAllTorrents();

    // Routine called after all resume data has been saved as part of an initlal begin_close() call
    void finalize_close();

    // Save state of controller to file
    void saveStateToFile(const char * file);

    void sendTransactions();
};

}
}

#endif // CONTROLLER_HPP
