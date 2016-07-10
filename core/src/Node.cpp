/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/Node.hpp>
#include <core/Configuration.hpp>
#include <core/Exception.hpp>
#include <core/detail/detail.hpp>
#include <extension/extension.hpp>
#include <bitcoin/SPVWallet.hpp>

#include <libtorrent/alert_types.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/peer_connection.hpp>

#include <QObject>
#include <QElapsedTimer>
#include <QtGlobal>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QThread>
//#include <QNetworkAccessManager>

/**
#ifndef Q_MOC_RUN
#include <boost/bind.hpp>
#endif Q_MOC_RUN

// Register types for signal and slots: LATER CHECK WHICH ONE OF THESE ARE ACTUALLY REQUIRED
Q_DECLARE_METATYPE(libtorrent::sha1_hash)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(libtorrent::error_code)
Q_DECLARE_METATYPE(std::vector<libtorrent::torrent_status>)
Q_DECLARE_METATYPE(libtorrent::torrent_status)
Q_DECLARE_METATYPE(Coin::Transaction) // Probably should not be here
Q_DECLARE_METATYPE(const libtorrent::alert*) // Register type for QMetaObject::invokeMethod
*/

namespace joystream {
namespace core {

Node::Node(joystream::bitcoin::SPVWallet * wallet)
    : _state(State::stopped)
    , _closing(false)
    , _reconnecting(false)
    , _protocolErrorsCount(0)
    , _session(nullptr)
    , _wallet(wallet) {

    /**
    // Comment out for now, not sure we need this

    // Register types for signal and slots
    qRegisterMetaType<libtorrent::sha1_hash>();
    qRegisterMetaType<std::string>();
    qRegisterMetaType<libtorrent::error_code>();
    qRegisterMetaType<std::vector<libtorrent::torrent_status>>();
    qRegisterMetaType<libtorrent::torrent_status>();
    qRegisterMetaType<Coin::Transaction>(); // Probably should not be here

    // Register type for QMetaObject::invokeMethod
    qRegisterMetaType<const libtorrent::alert*>();
    */

    /**
    // Connect streaming server signals
    QObject::connect(&_server,
                     SIGNAL(streamStarted(const Stream*)),
                     this,
                     SLOT(registerStream(const Stream*)));

    QObject::connect(&_server,
                     SIGNAL(streamCreationError(QAbstractSocket::SocketError socketError)),
                     this,
                     SLOT(handleFailedStreamCreation(QAbstractSocket::SocketError socketError)));


    // Connect server signals to corresponding controller slots
    QObject::connect(&_streamingServer,
                     SIGNAL(newConnection()),
                     this,
                     SLOT(handleConnection()));

    QObject::connect(&_streamingServer,
                     SIGNAL(acceptError(QAbstractSocket::SocketError)),
                     this,
                     SLOT(handleAcceptError(QAbstractSocket::SocketError)));

    // Start listening
    bool success = _streamingServer.listen(QHostAddress::Any); // auto selects port by removing port arg

    if(success)
        std::clog << "Started streaming server on port:" << _streamingServer.serverPort();
    else
        std::clog << "Could not start streaming server on port:" << _streamingServer.serverPort();
    */

    // Commenting out for now
    //qRegisterMetaType<Coin::TransactionId>("Coin::TransactionId");
    //QObject::connect(_wallet, SIGNAL(txUpdated(Coin::TransactionId, int)), this, SLOT(onTransactionUpdated(Coin::TransactionId ,int)));
}

Node::~Node() {

    // NB
    // we must handle case where ::stop is not caused, but
    // we simply go out of scope. In that case we must
    // do blocking exit, waiting on closed signal
    //
    // if we are already stopped, then we can easily close down

    _closing = true;

    _wallet->stopSync();
}

void Node::start(const configuration::Node & configuration, const NodeStarted & started, const NodeStartFailed & failed) {

    if(_state != State::stopped)
        throw exception::CanOnlyStartStoppedNode(_state);

    _state = State::starting;

    assert(_session == nullptr);
    assert(_wallet != nullptr);

    // Generate session settings
    libtorrent::settings_pack sessionSettings = Node::session_settings();

    // Create new libtorrent session wiht settings
    _session = new libtorrent::session(sessionSettings,
                                       libtorrent::session_handle::session_flags_t::start_default_features |
                                       libtorrent::session_handle::session_flags_t::add_default_plugins);

    // Generate DHT settings
    libtorrent::dht_settings dht_settings = Node::dht_settings();

    // Apply DHT settings to session
    _session->set_dht_settings(dht_settings);

    // Add DHT routers
    _session->add_dht_router(std::make_pair(std::string("router.bittorrent.com"), 6881));
    _session->add_dht_router(std::make_pair(std::string("router.utorrent.com"), 6881));
    _session->add_dht_router(std::make_pair(std::string("router.bitcomet.com"), 6881));

    // From libtorrent docs:
    // the ``set_alert_notify`` function lets the client set a function object
    // to be invoked every time the alert queue goes from having 0 alerts to
    // 1 alert. This function is called from within libtorrent, it may be the
    // main thread, or it may be from within a user call. The intention of
    // of the function is that the client wakes up its main thread, to poll
    // for more alerts using ``pop_alerts()``. If the notify function fails
    // to do so, it won't be called again, until ``pop_alerts`` is called for
    // some other reason.
    _session->set_alert_notify([this]() { this->libtorrent_alert_notification_entry_point(); });

    // Create and install plugin
    boost::shared_ptr<libtorrent::plugin> plugin(new extension::Plugin(CORE_MINIMUM_EXTENDED_MESSAGE_ID));

    // Keep weak reference to plugin
    _plugin = boost::static_pointer_cast<extension::Plugin>(plugin);

    // Add plugin extension
    _session->add_extension(plugin);

    // Try to start listening
    unsigned short port = _session->listen_port();

    std::clog << "Trying to listen on port" << std::to_string(port) << std::endl;

    // Store callbacks
    _nodeStarted = started;
    _nodeStartFailed = failed;
}

void Node::stop(const NodeStopped & nodeStopped) {

    if(_state == State::stopped || isStopping(_state))
        throw exception::CannotStopStoppedNode();

    // Pause all torrents
    _session->pause();

    // Stop all torrent plugins
    detail::SharedCounter counter;

    for(auto t: _torrents) {

        libtorrent::sha1_hash infoHash = t.first;

        // Add to stop counter
        counter.increment();

        // Send stop request to plugin on torrent
        t.second.stop([this, counter, infoHash](const std::exception_ptr & e) {

            // There should have been no exception in any scenario
            //handler.throwSetException();

            std::clog << "Stopped plugin on " << infoHash.to_string() << std::endl;

            // Are we done?
            if(counter.decrement()) {

                // Save resume data for all
                int numberOutStanding = this->requestResumeData();

                // If there are no outstanding, then just close right away
                if(numberOutStanding == 0)
                    this->finalize_stop();
                else {

                    std::clog << "Attempting to generate resume data for " << numberOutStanding << " torrents." << std::endl;

                    // Update state
                    _state = State::waiting_for_resume_data;
                }
            }
        });
    }

    // Store user callback to be called when resume data is generated
    _nodeStopped = nodeStopped;

    // Update state
    _state = State::stopping;
}

void Node::syncWallet() {

    if(_closing) return;

    _wallet->stopSync();

    _protocolErrorsCount = 0;

    std::clog << "connecting to bitcoin network...";
    std::clog << "peer timeout value used:" << CORE_CONTROLLER_SPV_KEEPALIVE_TIMEOUT;

    _wallet->sync("testnet-seed.bitcoin.petertodd.org", 18333, CORE_CONTROLLER_SPV_KEEPALIVE_TIMEOUT);

    _reconnecting = false;
}

/**
void Controller::handleConnection() {

    // Create handler for each pending connection
    // socket is owned by _server
    while(QTcpSocket * socket = _streamingServer.nextPendingConnection()) {

        std::clog << "New connection opened.";

        // Create stream
        Stream * stream = new Stream(socket, this);
    }
}

void Controller::handleAcceptError(QAbstractSocket::SocketError socketError) {
    std::clog << "Failed to accept connection.";
}
*/

void Node::scheduleReconnect() {

    if(_closing) return;

    if(_reconnecting) return;

    _reconnecting = true;

    // Retry connection
    QTimer::singleShot(CORE_CONTROLLER_RECONNECT_DELAY, this, SLOT(syncWallet()));
}

/**
void Controller::registerStream(const Stream * handler) {

    // Make sure to handle stream path announcement signal synchronously
    QObject::connect(handler,
                     SIGNAL(requestedPathAnnounced(const Stream*,QByteArray)),
                     this,
                     SLOT(registerRequestedPathOnStream(const Stream*,QByteArray)),
                     Qt::DirectConnection);
}

void Controller::handleFailedStreamCreation(QAbstractSocket::SocketError socketError) {
    std::clog << "handleFailedStreamCreation";
}

void Controller::registerRequestedPathOnStream(const Stream * stream, const QByteArray & requestedPath) const {

    // Turn into info hash
    libtorrent::sha1_hash infoHash(requestedPath.toStdString());

    if(_torrents.contains(infoHash)) {

        const TorrentViewModel * viewModel = _torrents[infoHash]->model();

        // Connect stream signals to view model slots
        QObject::connect(stream,
                         SIGNAL(rangeRequested(int, int)),
                         viewModel,
                         SLOT(getRange(int,int)));

        QObject::connect(stream,
                         SIGNAL(startRequested(int)),
                         viewModel,
                         SLOT(getStart(int)));

        QObject::connect(stream,
                         SIGNAL(errorOccured(Stream::Error)),
                         viewModel,
                         SLOT(errorOccured(Stream::Error)));

        // Connect view model signals to stream slots
        QObject::connect(viewModel,
                         SIGNAL(dataRangeRead(QString,int,int,int,QVector<Stream::Piece>,int,int)),
                         stream,
                         SLOT(sendDataRange(QString,int,int,int,QVector<Stream::Piece>,int,int)));

        QObject::connect(viewModel,
                         SIGNAL(receivedInvalidRange(int)),
                         stream,
                         SLOT(invalidRangeRequested(int)));


    } else {

        std::clog << "Requested path does not correspond to any presently active torrent.";

        Q_ASSERT(false); // <== no clean way to deal with this, as this slot is called synchronously
    }
}

void Controller::readPiece(int piece) {
    //_session->

    _session->get_torrents()

    // We must ask plugin sto download from a givne position in response
    // to a range reset

    // and the nonly read pieces ones we actually know they have been
    // downloaded...
}
*/

/**
void Controller::addPeerPlugin(libtorrent::sha1_hash info_hash, libtorrent::tcp::endpoint endPoint) {
    view.addPeerPlugin(info_hash, endPoint);
}

void Controller::updatePeerPluginStatus(PeerPluginStatus status) {
    view.updatePeerPluginStatus(status);
}

void Controller::removePeerPlugin(libtorrent::sha1_hash info_hash, libtorrent::tcp::endpoint endPoint) {
    view.removePeerPlugin(info_hash, endPoint);
}
*/

void Node::libtorrent_alert_notification_entry_point() {

    // Process alert queue on main qt thread, not on libtorrent thread
    QMetaObject::invokeMethod(this, "processAlertQueue");
}

void Node::processAlertQueue() {

    // Populate vector with alerts
    std::vector<libtorrent::alert *> alerts;
    _session->pop_alerts(&alerts);

    // NOTE on owernship of alerts (alert.hpp)
    // Alerts returned by pop_alerts() are only valid until the next call to
    // pop_alerts(). You may not copy an alert object to access it after the next
    // call to pop_alerts(). Internal members of alerts also become invalid once
    // pop_alerts() is called again.

    // Process alerts in queue
    for(libtorrent::alert * alert : alerts)
        processAlert(alert);

}

void Node::processAlert(const libtorrent::alert * a) {

    std::clog << "Processing alert" << a->message() << std::endl;

    // Select alert type
    if(libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a))
        processMetadataReceivedAlert(p);
    else if(libtorrent::metadata_failed_alert const * p = libtorrent::alert_cast<libtorrent::metadata_failed_alert>(a))
        processMetadataFailedAlert(p);
    else if(libtorrent::listen_succeeded_alert const * p = libtorrent::alert_cast<libtorrent::listen_succeeded_alert>(a))
        process(p);
    else if(libtorrent::listen_failed_alert const * p = libtorrent::alert_cast<libtorrent::listen_failed_alert>(a))
        process(p);
    else if(libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a))
        process(p);
    else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a))
        processTorrentFinishedAlert(p);
    else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a))
        processTorrentPausedAlert(p);
    else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a))
        processStatusUpdateAlert(p);
    else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a))
        processTorrentRemovedAlert(p);
    else if(libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a))
        process(p);
    else if(libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a))
        process(p);
    else if(libtorrent::torrent_checked_alert const * p = libtorrent::alert_cast<libtorrent::torrent_checked_alert>(a))
        processTorrentCheckedAlert(p);
    else if(libtorrent::read_piece_alert const * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a))
        processReadPieceAlert(p);
    else if(libtorrent::piece_finished_alert const * p = libtorrent::alert_cast<libtorrent::piece_finished_alert>(a))
        processPieceFinishedAlert(p);
    else if(const extension::alert::RequestResult * p = libtorrent::alert_cast<extension::alert::RequestResult>(a))
        process(p);
    else if(const extension::alert::BroadcastTransaction * p = libtorrent::alert_cast<extension::alert::BroadcastTransaction>(a))
        process(p);
    else if(extension::alert::PluginStatus const * p = libtorrent::alert_cast<extension::alert::PluginStatus>(a))
        process(p);
    else
        assert(false);

    /**
    else if(const StartedSellerTorrentPlugin * p = libtorrent::alert_cast<StartedSellerTorrentPlugin>(a))
        processStartedSellerTorrentPlugin(p);
    else if(const StartedBuyerTorrentPlugin * p = libtorrent::alert_cast<StartedBuyerTorrentPlugin>(a))
        processStartedBuyerTorrentPlugin(p);
    else if(const SellerTorrentPluginStatusAlert * p = libtorrent::alert_cast<SellerTorrentPluginStatusAlert>(a))
        processSellerTorrentPluginStatusAlert(p);
    else if(const BuyerTorrentPluginStatusAlert * p = libtorrent::alert_cast<BuyerTorrentPluginStatusAlert>(a))
        processBuyerTorrentPluginStatusAlert(p);
    else if(const SellerPeerAddedAlert * p = libtorrent::alert_cast<SellerPeerAddedAlert>(a))
        processSellerPeerAddedAlert(p);
    else if(const BuyerPeerAddedAlert * p = libtorrent::alert_cast<BuyerPeerAddedAlert>(a))
        processBuyerPeerAddedAlert(p);
    else if(const SellerPeerPluginRemovedAlert * p = libtorrent::alert_cast<SellerPeerPluginRemovedAlert>(a))
        processSellerPeerPluginRemovedAlert(p);
    else if(const BuyerPeerPluginRemovedAlert * p = libtorrent::alert_cast<BuyerPeerPluginRemovedAlert>(a))
        processBuyerPeerPluginRemovedAlert(p);
    else if(const BroadcastTransactionAlert * p = libtorrent::alert_cast<BroadcastTransactionAlert>(a))
        processBroadcastTransactionAlert(p);
    //else if(const TorrentPluginStartedAlert * p = libtorrent::alert_cast<TorrentPluginStartedAlert>(a))
    //    processTorrentPluginStartedAlert(p);
    */
}


void Node::process(const libtorrent::listen_succeeded_alert * p) {

    assert(_state == State::starting);

    _state = State::started;

    std::clog << "Listening on endpoint" << p->endpoint.address().to_string() << std::endl;

    // Make callback to user
    _nodeStarted(p->endpoint);

    emit nodeStarted(p->endpoint);
}

void Node::process(const libtorrent::listen_failed_alert * p) {

    assert(_state == State::starting);

    _state = State::stopped;

    std::clog << "Failed to start listening on endpoint"
              << p->endpoint.address().to_string()
              << "due to error"
              << p->error.message();

    // Make callback to user
    _nodeStartFailed(p->endpoint, p->error);
}

int Node::requestResumeData() {

    // Keeps track of how many calls were made
    int resumeCallsMade = 0;

    for(auto mapping : _torrents) {

        // Grab torrent;
        detail::Torrent & t = mapping.second;

        // are in wrong state
        if (t.state != detail::Torrent::State::normal)
            continue;

        // Get handle
        libtorrent::torrent_handle h = _session->find_torrent(mapping.first);

        // Dont save data if
        if (!h.is_valid() || // dont have valid handle
            !h.need_save_resume_data() || // dont need to
            !h.status().has_metadata) // or dont have metadata
            continue;

        // Save resume data
        h.save_resume_data();

        // Count call
        resumeCallsMade++;

        // Change expected event of torrent
        t.state = detail::Torrent::State::waiting_for_resume_data;
    }

    return resumeCallsMade;
}

void Node::processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p) {

    /**
    // Get handle
    libtorrent::torrent_handle torrentHandle = p->handle;

    // Dont save data if we dont need to or can
    if (!torrentHandle.need_save_resume_data() || !torrentHandle.status().has_metadata)
        return;

    // Set state
    //_sourceForLastResumeDataCall = TORRENT_PAUSE;
    //_numberOfOutstandingResumeDataCalls = 1;

    // Save resume data
    torrentHandle.save_resume_data();

    // Get info hash
    libtorrent::sha1_hash info_hash = torrentHandle.info_hash();

    // Grab torrent and set next event to be arrival of save_resume_data alert
    Q_ASSERT(_torrents.contains(info_hash));

    _torrents[info_hash]->setStatus(Torrent::Status::asked_for_resume_data);
    */
}

void Node::processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p) {

    /*
     * NOTICE: Docs say p->handle may be invalid at this time,
     * so we must use p->info_hash instead.
     */

    // Get torrent info hash
    libtorrent::sha1_hash info_hash = p->info_hash;

    // Remove from view
    removedTorrent(info_hash);

    std::clog << "Found match and removed it.";
}

void Node::processMetadataReceivedAlert(libtorrent::metadata_received_alert const * p) {

    // Get handle for torrent
    libtorrent::torrent_handle h = p->handle;

    // Process if handle is valid
    if (h.is_valid()) {

        // get torrent info
        //boost::shared_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();

        //boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();

        //const libtorrent::torrent_info & torrentInfo = h.get_torrent_info();

        // USE THIS INFORMATION FOR SOMETHING LATER

        // Put in model later
        std::clog << "Metadata saved.";

    } else
        std::clog << "Invalid handle for received metadata.";
}

void Node::processMetadataFailedAlert(libtorrent::metadata_failed_alert const * p) {

    // WHAT DO WE DO HERE?
    std::clog << "Invalid metadata received.";
    throw std::runtime_error("Invalid metadata");
}

void Node::process(libtorrent::add_torrent_alert const * p) {

    /**
    Q_ASSERT(_state == State::normal);
    Q_ASSERT(_torrents.contains(p->params.info_hash));

    // Get torrent
    Torrent * torrent = _torrents[p->params.info_hash];

    Q_ASSERT(torrent->status() == Torrent::Status::being_async_added_to_session);

    // Check if there was an error
    if (p->error) {

        std::clog << "Adding torrent failed, must be removed.";

        // Remove torrent here, so that it does not hanga around and cause problems.

        // old name arg: p->params.ti.get() != 0 ? p->params.ti->name() : name = p->params.name
        emit failedToAddTorrent(p->params.name, p->params.info_hash, p->error);

    } else {

        std::clog << "Adding torrent succeeded.";

//		h.set_max_connections(max_connections_per_torrent);
//		h.set_max_uploads(-1);
//		h.set_upload_limit(torrent_upload_limit);
//		h.set_download_limit(torrent_download_limit);
//		h.use_interface(outgoing_interface.c_str());

        // Give copy of handle
        torrent->setHandle(p->handle);

        // Update expected event on torrent
        torrent->setStatus(Torrent::Status::torrent_checked);

        // Send notification signal
        emit addedTorrent(torrent->model());
	}
*/
}

void Node::processTorrentFinishedAlert(libtorrent::torrent_finished_alert const * p) {

    /*
    p->handle.set_max_connections(max_connections_per_torrent / 2);

    // write resume data for the finished torrent
    // the alert handler for save_resume_data_alert
    // will save it to disk
    torrent_handle h = p->handle;
    h.save_resume_data();
    ++num_outstanding_resume_data;
    */
}

void Node::processStatusUpdateAlert(libtorrent::state_update_alert const * p) {
    update(p->status);
}

void Node::process(const libtorrent::save_resume_data_alert * p) {

    std::clog << "libtorrent::save_resume_data_alert" << std::endl;

    // Get reference to corresponding torrent
    libtorrent::torrent_handle h = p->handle;

    auto it = _torrents.find(h.info_hash());

    if(it == _torrents.cend()) {
        std::clog << "Dropped alert, no correspondign torrent found.";
        return;
    }

    detail::Torrent & torrent = it->second;

    // Check that alert was expected
    assert(torrent.state == detail::Torrent::State::waiting_for_resume_data);

    // Reset expected event
    torrent.state = detail::Torrent::State::normal;

    // Create resume data buffer
    std::vector<char> resumeData;

    // Write new content to it
    libtorrent::bencode(std::back_inserter(resumeData), *(p->resume_data));

    // Save resume data in torrent
    torrent.resumeData = resumeData;

    // If this is part of closing client, then close client
    // if there are no torrents still waiting for resume data.
    if(_state == State::stopping) {

        for(auto mapping : _torrents) {
            if(mapping.second.state == detail::Torrent::State::waiting_for_resume_data)
                return;
        }

        // Close client
        finalize_stop();
    }
}

void Node::process(const libtorrent::save_resume_data_failed_alert * p) {

    /// Not sure why this would ever happen?

    std::clog << "libtorrent::save_resume_data_failed_alert: "
              << p->message() << std::endl;

    // Get reference to corresponding torrent
    libtorrent::torrent_handle h = p->handle;

    auto it = _torrents.find(h.info_hash());

    if(it == _torrents.cend()) {
        std::clog << "Dropped alert, no correspondign torrent found.";
        return;
    }

    detail::Torrent & torrent = it->second;

    // Check that alert was expected
    assert(torrent.state == detail::Torrent::State::waiting_for_resume_data);

    // Reset expected event
    torrent.state = detail::Torrent::State::normal;

    // If this is part of closing client, then close client
    // if there are no torrents still waiting for resume data.
    if(_state == State::stopping) {

        for(auto mapping : _torrents) {
            if(mapping.second.state == detail::Torrent::State::waiting_for_resume_data)
                return;
        }

        // Close client
        finalize_stop();
    }
}

void Node::processTorrentCheckedAlert(libtorrent::torrent_checked_alert const * p) {

//    // Get handle for torrent
//    libtorrent::torrent_handle h = p->handle;

//    // Process if handle is valid
//    if(h.is_valid()) {

//        // Get info hash of torrent
//        libtorrent::sha1_hash infoHash = h.info_hash();

//        //qCCritical(_category) << QString::fromStdString(h.info_hash().to_string()); // infoHash.to_string()
//        //qCCritical(_category) << QString::fromStdString(_torrents.first()->infoHash().to_string());

//        // Make sure the torrent exists
//        Q_ASSERT(_torrents.contains(infoHash));

//        // Grab torrent
//        Torrent * torrent = _torrents[infoHash];

//        // Assert that torrent_checked_alert was expected
//        Q_ASSERT(torrent->status() == Torrent::Status::torrent_checked);

//        /**
//        // if a configuration was saved, i.e. started from disk, then we just use it
//        if(_pendingConfigurations.contains(info_hash)) {

//            // Remove torrent plugin configuration
//            const TorrentPlugin::Configuration * configuration = _pendingConfigurations.take(info_hash);

//            // Send configuration to plugin
//            _plugin->submitPluginRequest(new StartTorrentPlugin(info_hash, configuration));

//            // Reset event
//            torrent.setEvent(Torrent::ExpectedEvent::nothing);

//        } */

//        // if a configuration was saved, i.e. started from disk, then we just use it
//        if(_pendingSellerTorrentPluginConfigurations.contains(infoHash)) {

//            // Remove torrent plugin configuration
//            const SellerTorrentPlugin::Configuration configuration = _pendingSellerTorrentPluginConfigurations.take(infoHash);

//            // Send configuration to plugin
//            _plugin->submitPluginRequest(new StartSellerTorrentPlugin(infoHash, configuration));

//            // Reset event
//            torrent->setStatus(Torrent::Status::nothing);

//        } else if(_pendingBuyerTorrentPluginConfigurationAndUtxos.contains(infoHash)) {

//            // Remove torrent plugin configuration and utxo
//            QPair<BuyerTorrentPlugin::Configuration, Coin::UnspentP2PKHOutput> p = _pendingBuyerTorrentPluginConfigurationAndUtxos.take(infoHash);

//            // Send configuration to plugin
//            _plugin->submitPluginRequest(new StartBuyerTorrentPlugin(infoHash, p.first, p.second));

//            // Reset event
//            torrent->setStatus(Torrent::Status::nothing);

//        } /** else if(_pendingObserverTorrentPluginConfigurations.contains(info_hash)) {

//            // Remove torrent plugin configuration
//            const ObserverTorrentPlugin::Configuration configuration = _pendingObserverTorrentPluginConfigurations.take(info_hash);

//            // Send configuration to plugin
//            _plugin->submitPluginRequest(new StartObserverTorrentPlugin(info_hash, configuration));

//            // Reset event
//            torrent.setEvent(Torrent::ExpectedEvent::nothing);

//        }*/ else {

//            // Get torrent information
//            //boost::shared_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();
//            boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();
//            const libtorrent::torrent_info * torrentInfoPtr = torrentInfoIntrusivePtr.get();
//            libtorrent::torrent_info torrentInfo = *torrentInfoPtr; //h.get_torrent_info();

//            // Get torrent status
//            libtorrent::torrent_status torrentStatus = h.status();

//            // No longer used: Expect user to set configurations
//            //torrent->setStatus(Torrent::Status::torrent_plugin_configuration_from_user);

//            // Send signal
//            emit torrentCheckedButHasNoPlugin(torrentInfo, torrentStatus);
//        }

//    } else {

//        std::clog << "Invalid handle for checked torrent.";
//        Q_ASSERT(false);
//    }
}

void Node::processReadPieceAlert(const libtorrent::read_piece_alert * p) {

//    // Get info hash for torrent from which this read piece comes from
//    const libtorrent::sha1_hash infoHash = p->handle.info_hash();

//    Q_ASSERT(_torrents.contains(infoHash));

//    /**
//    // Notify torrent view model
//    _torrents[infoHash]->model()->pieceRead(p->ec,
//                                            p->buffer,
//                                            p->piece,
//                                            p->size);
//    */

//    if(p->ec) {
//        std::clog << "There was some sort of error in reading a piece: " << QString::fromStdString(p->ec.message());
//    } else {

//        // Notify torrent
//        _torrents[infoHash]->pieceRead(p->buffer, p->piece, p->size);
//    }
}

void Node::processPieceFinishedAlert(const libtorrent::piece_finished_alert * p) {

//    // Get info hash for torrent from which this read piece comes from
//    const libtorrent::sha1_hash infoHash = p->handle.info_hash();

//    Q_ASSERT(_torrents.contains(infoHash));

//    // Notify torrent
//    _torrents[infoHash]->pieceFinished(p->piece_index);
}

void Node::process(const extension::alert::RequestResult * p) {

    // Call handler with result
    //p->resultHandler(p->result);

}

void Node::process(const extension::alert::BroadcastTransaction * p) {

    // Enqueue transaction
    _transactionSendQueue.push_back(p->tx);

    // try to send immediately
    try {
        _wallet->broadcastTx(p->tx);
    } catch(std::exception & e) {
        // wallet is offline
    }
}

/**
void Node::processStartedSellerTorrentPlugin(const StartedSellerTorrentPlugin * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    // Get torrent
    Torrent * torrent = _torrents[p->infoHash()];

    Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::None);

    // Update information about plugin installed on torrent
    torrent->addPlugin(p->status());

    // Send signal
    // *** DISCONTINUED, THESE ARE INSTALLED AS SIGNALS ON TORRENT VIEW MODELS
    //emit startedSellerTorrentPlugin(torrent->model()->sellerTorrentPluginViewModel());
}

void Node::processStartedBuyerTorrentPlugin(const StartedBuyerTorrentPlugin * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    Torrent * torrent = _torrents[p->infoHash()];

    Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::None);

    // Update information about plugin installed on torrent
    torrent->addPlugin(p->status());

    // Send signal
    // *** DISCONTINUED, THESE ARE INSTALLED AS SIGNALS ON TORRENT VIEW MODELS
    //emit startedBuyerTorrentPlugin(torrent->model()->buyerTorrentPluginViewModel());
}

void Node::processBuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    // Get view model for torrent
    TorrentViewModel * model = _torrents[p->infoHash()]->model();

    Q_ASSERT(model->pluginInstalled() == PluginInstalled::Buyer);

    // Update
    model->update(p->status());
}

void Node::processSellerTorrentPluginStatusAlert(const SellerTorrentPluginStatusAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    // Get view model for torrent
    TorrentViewModel * model = _torrents[p->infoHash()]->model();

    Q_ASSERT(model->pluginInstalled() == PluginInstalled::Seller);

    // Update
    model->update(p->status());
}
*/
void Node::process(const extension::alert::PluginStatus * p) {

    //
    emit pluginStatusUpdate(p->status);
}
/*
void Node::processSellerPeerAddedAlert(const SellerPeerAddedAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    Torrent * torrent = _torrents[p->infoHash()];

    torrent->model()->addPeer(p->endPoint(), p->status());
}

void Node::processBuyerPeerAddedAlert(const BuyerPeerAddedAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    Torrent * torrent = _torrents[p->infoHash()];

    torrent->model()->addPeer(p->endPoint(), p->status());
}

void Node::processSellerPeerPluginRemovedAlert(const SellerPeerPluginRemovedAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    // Grab torrent
    Torrent * torrent = _torrents[p->infoHash()];

    Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::Seller);

    // Notify view model
    torrent->model()->removePeer(p->endPoint());
}

void Node::processBuyerPeerPluginRemovedAlert(const BuyerPeerPluginRemovedAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    // Grab torrent
    Torrent * torrent = _torrents[p->infoHash()];

    Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::Buyer);

    // Notify view model
    torrent->model()->removePeer(p->endPoint());
}
*/

// called on timer signal, to periodically try to resend transactions to the network
void Node::sendTransactions() {

    for(auto tx : _transactionSendQueue) {
        try {
            _wallet->broadcastTx(tx);
        } catch(std::exception & e) {
            // wallet is offline
            return;
        }
    }
}

extension::Plugin * Node::plugin() {

    boost::shared_ptr<extension::Plugin> plugin = _plugin.lock();

    assert(plugin);

    return plugin.get();
}

// when wallet sees a transaction, either 0, 1 or 2 confirmations
void Node::onTransactionUpdated(Coin::TransactionId txid, int confirmations) {

    //remove matching transaction from the send queue
    std::vector<Coin::Transaction>::iterator it;

    it = std::find_if(_transactionSendQueue.begin(), _transactionSendQueue.end(), [&txid](Coin::Transaction &tx){
       return txid == Coin::TransactionId::fromTx(tx);
    });

    if(it!= _transactionSendQueue.end()){
        _transactionSendQueue.erase(it);
    }
}

void Node::onWalletSynched() {
    std::clog << "Wallet Synched";
}

void Node::onWalletSynchingHeaders() {
    std::clog << "Wallet Synching Headers";
}

void Node::onWalletSynchingBlocks() {
    std::clog << "Wallet Synching Blocks";
}

void Node::onWalletConnected() {
    std::clog << "Wallet Connected";
    sendTransactions();
}

void Node::update(const std::vector<libtorrent::torrent_status> & statuses) {

    for(std::vector<libtorrent::torrent_status>::const_iterator
        i = statuses.begin(),
        end = statuses.end(); i != end;i++)
        update(*i);
}

void Node::update(const libtorrent::torrent_status & status) {

    /**
    Q_ASSERT(_torrents.contains(status.info_hash));

    // Get view model
    TorrentViewModel * model = _torrents[status.info_hash]->model();

    // Update status
    model->update(status);
    */
}

void Node::removeTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle torrentHandle = _session->find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        Q_ASSERT(false);

    // Remove from session
    // Session will send us torrent_removed_alert alert when torrent has been removed
    // at which point we can remove torrent from model in alert handler
    _session->remove_torrent(torrentHandle);
}

/**
void Node::pauseTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle torrentHandle = _session->find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        Q_ASSERT(false);

    // Turn off auto managing
    torrentHandle.auto_managed(false);

    // Pause
    // We save resume data when the pause alert is issued by libtorrent
    torrentHandle.pause(libtorrent::torrent_handle::graceful_pause);
}

void Node::startTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle torrentHandle = _session->find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        Q_ASSERT(false);

    // Turn on auto managing
    torrentHandle.auto_managed(true);

    // Start
    torrentHandle.resume();
}
*/

void Node::addTorrent(const configuration::Torrent &, const AddedTorrent &) {
/**
    // make sure to check _state guard

    // Convert to add torrent parameters
    libtorrent::add_torrent_params params = configuration.toAddTorrentParams();

    // Create save_path on disk, return if it does not exist
    if(!(QDir()).mkpath(params.save_path.c_str())) {
        qCCritical(_category) << "Could not create save_path: " << params.save_path.c_str();
        return false;
    }

    // Get info hash
    libtorrent::sha1_hash info_hash = configuration.infoHash();

    // Create torrent
    Torrent * torrent = new Torrent(info_hash,
                                    configuration.name(),
                                    configuration.name(),
                                    configuration.resumeData(),
                                    configuration.flags(),
                                    configuration.torrentFile(), //.torrentInfo(),
                                    //libtorrent::torrent_handle(), // proper handle is set when torrent has been added
                                    Torrent::Status::being_async_added_to_session);

    // Warn user if torrent has already been added
    if(_torrents.contains(info_hash)) {
        qCCritical(_category) << "Torrent with given info_hash has already been added.";
        return false;
    }

    // !_torrents.contains(info_hash) =>
    Q_ASSERT(!_pendingSellerTorrentPluginConfigurations.contains(info_hash));
    Q_ASSERT(!_pendingBuyerTorrentPluginConfigurationAndUtxos.contains(info_hash));
    //Q_ASSERT(!_pendingObserverTorrentPluginConfigurations.contains(info_hash));

    // NOTE:
    // The remaining signals model
    // * pluginInstalledChanged(PluginInstalled pluginInstalled)
    // * torrentStatusChanged(const libtorrent::torrent_status & status)
    // should be wired up by controller users upon
    // capturing addedTorrent(const TorrentViewModel *) signal.

    // Save torrent
    _torrents[info_hash] = torrent;

    // Add torrent to session
    _session->async_add_torrent(params);
    */
}
/**
bool Node::addTorrent(const Torrent::Configuration & configuration, const SellerTorrentPlugin::Configuration & pluginConfiguration) {

    // Try to add torrent
    if(!addTorrent(configuration))
        return false;

    // Save plugin configuration
    _pendingSellerTorrentPluginConfigurations[configuration.infoHash()] = pluginConfiguration;

    return true;
}

bool Node::addTorrent(const Torrent::Configuration & configuration, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo) {

    // Try to add torrent
    if(!addTorrent(configuration))
        return false;

    // Save plugin configuration and utxo
    _pendingBuyerTorrentPluginConfigurationAndUtxos[configuration.infoHash()] = QPair<BuyerTorrentPlugin::Configuration, Coin::UnspentP2PKHOutput>(pluginConfiguration, utxo);

    return true;
}
*/

/**
void Controller::startTorrentPlugin(const libtorrent::sha1_hash & info_hash, const TorrentPlugin::Configuration * configuration) {

    // Check that torrent exists with given info hash
    if(!_torrents.contains(info_hash)) {
        qCCritical(_category) << "No torrent registered with given info hash.";
        return;
    }

    // Grab torrent
    Torrent & torrent = _torrents[info_hash];

    Q_ASSERT(torrent->status() != Torrent::Status::being_async_added_to_session &&
            torrent->status() != Torrent::Status::torrent_added_but_not_checked);

    // Send configuration to plugin
    _plugin->submitPluginRequest(new StartTorrentPlugin(info_hash, configuration));

    // Reset event
    torrent.setEvent(Torrent::ExpectedEvent::nothing);
}
*/

/**
void Node::startSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPlugin::Configuration & pluginConfiguration) {

    Q_ASSERT(_torrents.contains(info_hash));

    // Grab torrent
    Torrent * torrent = _torrents[info_hash];

    Q_ASSERT(torrent->status() != Torrent::Status::being_async_added_to_session &&
            torrent->status() != Torrent::Status::torrent_added_but_not_checked);

    // Reset event
    torrent->setStatus(Torrent::Status::nothing);

    // Send configuration to plugin
    _plugin->submitPluginRequest(new StartSellerTorrentPlugin(info_hash, pluginConfiguration));
}

void Node::startBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo) {

    Q_ASSERT(_torrents.contains(info_hash));

    // Grab torrent
    Torrent * torrent = _torrents[info_hash];

    Q_ASSERT(torrent->status() != Torrent::Status::being_async_added_to_session &&
            torrent->status() != Torrent::Status::torrent_added_but_not_checked);

    // Reset event
    torrent->setStatus(Torrent::Status::nothing);

    // Send configuration to plugin
    _plugin->submitPluginRequest(new StartBuyerTorrentPlugin(info_hash, pluginConfiguration, utxo));
}
*/

void Node::saveStateToFile(const char * file) {

    // NOT DONE!

    /**

	// Save present state of session in entry
	libtorrent::entry libtorrentSessionSettingsEntry;
    _session->save_state(libtorrentSessionSettingsEntry);

    // Torrent configurations
    std::vector<TorrentConfiguration *> torrentConfigurations;

    // Grab torrent handles
    std::vector<libtorrent::torrent_handle> torrents = _session->get_torrents();

    for(std::vector<libtorrent::torrent_handle>::iterator i = torrents.begin(),
            end(torrents.end()); i != end;i++) {

        // Check if torrent handle is valid, if not, we can't save it
        if((*i).is_valid()) {

            // We need to have torrent plugin configurations already available,
            // which is done asynch, perhaps make a parameter of this method
            // which is called by a callback routine.

            // Create torrent configuration
            TorrentConfiguration * torrentConfiguration = new TorrentConfiguration();

            // Add to vector of configurations
            torrentConfigurations.push_back(torrentConfiguration);

        } else
            qCCritical(_category) << "Could not recover handle for torrent for saving, skipping it.";
    }

    // DHT routers
    std::vector<std::pair<std::string, int>> dhtRouters;

    // Create controller configuration
    ControllerConfiguration controllerConfiguration(libtorrentSessionSettingsEntry
                                                    ,_portRange
                                                    ,torrentConfigurations
                                                    ,dhtRouters);

	// Save to file
    controllerConfiguration.saveToFile(file);
    */
}

/**
joystream::bitcoin::SPVWallet * Node::wallet() {
    return _wallet;
}
*/

/**
libtorrent::torrent_handle Controller::getTorrentHandle(const libtorrent::sha1_hash & infoHash) const {

    if(_torrents.contains(infoHash))
        return _torrents[infoHash]->handle();
    else
        return libtorrent::torrent_handle();
}
*/

/**
libtorrent::torrent_handle Controller::registerStream(Stream * stream) {

    // Get info hash of torrent requested in stream
    libtorrent::sha1_hash infoHash = stream->infoHash();

    // If no torrent exist with given info hash, just return default handle
    if(!_torrents.contains(infoHash))
        return libtorrent::torrent_handle();

     // Get torrent
     Torrent * torrent = _torrents[infoHash];

     // Add stream
     torrent->addStream(stream);

     // Get handle
     libtorrent::torrent_handle h = torrent->handle();

     Q_ASSERT(h.is_valid());

     return h;
}

void Controller::unRegisterStream(Stream * stream) {

    libtorrent::sha1_hash infoHash = stream->infoHash();

    if(_torrents.contains(infoHash))
        _torrents[infoHash]->removeStream(stream);
    else
        std::clog << "Failed unregistering stream, torrent does not exist.";
}

void Controller::unRegisterStream(Stream * stream, Stream::Error error) {

    unRegisterStream(stream);

    std::clog << "Stream unregistered due to some error.";

    //emit some sort of signal about error
}
*/

/**
void Node::changeDownloadingLocationFromThisPiece(const libtorrent::sha1_hash & infoHash, int pieceIndex) {

    // Check that torrent exists
    if(!_torrents.contains(infoHash)) {

        std::clog << "Changing download location requested for torrent which does not exist.";
        return;
    }

    // Check that
    if(_torrents[infoHash]->pluginInstalled() != PluginInstalled::Buyer) {

        std::clog << "Changing download location requested for with plugin which does not have a buyer torrent plugin installed on torrent.";
        return;
    }

    // Ask torrent to relocate
    _plugin->submitTorrentPluginRequest(new ChangeDownloadLocation(infoHash, pieceIndex));
}
*/

void Node::updateStatus() {

    //guard state?


    // Regular torrent level state update
    _session->post_torrent_updates();

    // Plugin level updates
    plugin()->submit(extension::request::UpdateStatus());
}

Node::State Node::state() const {
    return _state;
}

/**
quint16 Controller::getServerPort() const {
   return _streamingServer.serverPort();
}
*/

std::set<libtorrent::sha1_hash> Node::torrents() const {

    std::set<libtorrent::sha1_hash> infoHashes;

    for(auto mapping : _torrents)
        infoHashes.insert(mapping.first);

    return infoHashes;
}

std::weak_ptr<Torrent> Node::torrent(const libtorrent::sha1_hash & infoHash) const {

    auto it = _torrents.find(infoHash);

    if(it == _torrents.cend())
        throw exception::NoSuchTorrentExists(infoHash);
    else
        return it->second.model;
}

void Node::finalize_stop() {

    std::clog << "Dropping libtorrent session." << std::endl;

    // Close session and delete instance
    // This call will block, and may take a while to complete,
    // as it involves on communicating with trackers - which may time out.
    // Asynchronous shut-down is possible using libtorrent::session::abort(),
    // however we opted for avoiding, since not other calls to
    delete _session;
    _session = nullptr;

    // Update state
    _state = State::stopped;

    // Make user callback
    _nodeStopped();

    // Tell runner that controller is done
    emit nodeStopped();
}

/**
void Controller::fundWallet(uint64_t value) {
    _restClient->fundWalletFromFaucet(_wallet->getReceiveAddress().toBase58CheckEncoding(), value);
}
*/


libtorrent::settings_pack Node::session_settings() noexcept {

    // Initialize with default values
    libtorrent::settings_pack pack;

    // This is the client identification to the tracker.
    // The recommended format of this string is: "ClientName/ClientVersion libtorrent/libtorrentVersion".
    // This name will not only be used when making HTTP requests, but also when sending BEP10 extended handshake
    // if handshake_client_version is left blank.
    // default: "libtorrent/" LIBTORRENT_VERSION
    pack.set_str(libtorrent::settings_pack::user_agent, std::string(CORE_USER_AGENT_NAME) +
                                                        std::string("/") +
                                                        std::to_string(CORE_VERSION_MAJOR) +
                                                        std::string(".") +
                                                        std::to_string(CORE_VERSION_MINOR));

    // Client name and version identifier sent to peers in the BEP10 handshake message.
    // If this is an empty string, the user_agent is used instead.
    // default: <user_agent>
    //pack.set_str(libtorrent::settings_pack::handshake_client_version, std::string(CORE_USER_AGENT_NAME) + CORE_VERSION_MAJOR + "." + CORE_VERSION_MINOR);

    // Fingerprint for the client.
    // It will be used as the prefix to the peer_id.
    // If this is 20 bytes (or longer) it will be used as the peer-id
    // There are two encoding styles, we use Azureus style, which is most popular:
    // '-', two characters for client id, four ascii digits for version number, '-', followed by random numbers.
    // For example: '-AZ2060-'...
    // default: "-LT1100-"
    std::string peerIdString = libtorrent::fingerprint(CORE_PEER_ID, CORE_VERSION_MAJOR, CORE_VERSION_MINOR, 0, 0).to_string();
    assert(peerIdString.length() == 20);

    pack.set_str(libtorrent::settings_pack::peer_fingerprint, peerIdString);

    // Determines if connections from the same IP address as existing
    // connections should be rejected or not. Multiple connections from
    // the same IP address is not allowed by default, to prevent abusive behavior by peers.
    // It may be useful to allow such connections in cases where simulations
    // are run on the same machie, and all peers in a swarm has the same IP address.
    pack.set_bool(libtorrent::settings_pack::allow_multiple_connections_per_ip, true);


    return pack;
}

libtorrent::dht_settings Node::dht_settings() noexcept {

    // Initialize with default values
    libtorrent::dht_settings settings;

    // For now we use default settings!

    return settings;
}

}
}
