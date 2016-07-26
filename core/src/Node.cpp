/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/Node.hpp>
#include <core/Torrent.hpp>
#include <core/Exception.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/detail/detail.hpp>
#include <extension/extension.hpp>

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
#include <QTimer>
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

Node::Node(const BroadcastTransaction & broadcastTransaction)
    : _session(Node::session_settings(),
               libtorrent::session_handle::session_flags_t::start_default_features |
               libtorrent::session_handle::session_flags_t::add_default_plugins)
    , _broadcastTransaction(broadcastTransaction) {

    // Check if session was actually started properly
    libtorrent::error_code ec;

    if(ec)
        throw exception::FailedToStartNodeException(ec);
    else {

        unsigned short port = _session.listen_port();

        std::clog << "Node started with BitTorrent daemon on port: " << port << "." << std::endl;

        libtorrent::tcp::endpoint endPoint = libtorrent::parse_endpoint("0.0.0.0:" + std::to_string(port), ec);
        assert(!ec);

        emit startedListeningOnPort(endPoint);
    }

    // Set alerts notification callback in session
    // NB: This delayed setup of this notifcation handler w.r.t. creation of session
    // can lead to a race condition where the inition 0->1 event is missed, and so
    // the node misses all alerts until the session reaches its limit and dumps all alerts.
    // To avoid this, we make one initial call to processAlertQueue() at the end of the constructor.
    _session.set_alert_notify([this]() { this->libtorrent_alert_notification_entry_point(); });

    /// Setup DHT

    // Generate DHT settings
    libtorrent::dht_settings dht_settings = Node::dht_settings();

    // Apply DHT settings to session
    _session.set_dht_settings(dht_settings);

    // Add DHT routers
    _session.add_dht_router(std::make_pair(std::string("router.bittorrent.com"), 6881));
    _session.add_dht_router(std::make_pair(std::string("router.utorrent.com"), 6881));
    _session.add_dht_router(std::make_pair(std::string("router.bitcomet.com"), 6881));

    /// Setup plugin

    // Create and install plugin
    boost::shared_ptr<libtorrent::plugin> plugin(new extension::Plugin([broadcastTransaction](const libtorrent::sha1_hash &, const Coin::Transaction & tx) -> void {
                                                        // Call user provided broadcasting callback
                                                        broadcastTransaction(tx);
                                                     }, CORE_MINIMUM_EXTENDED_MESSAGE_ID));

    // Keep weak reference to plugin
    _plugin = boost::static_pointer_cast<extension::Plugin>(plugin);

    // Add plugin extension
    _session.add_extension(plugin);

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

    // Resolve potential race condition described above
    processAlertQueue();
}

void Node::pause(const NodePaused & nodePaused) {

    // Pause libtorrent session
    _plugin->submit(extension::request::PauseLibtorrent([this, nodePaused]() {

        std::clog << "Libtorrent session paused" << std::endl;

        // Stop all plugins
        _plugin->submit(extension::request::StopAllTorrentPlugins([this, nodePaused]() {

            std::clog << "All plugins stopped" << std::endl;

            // Make user callback
            nodePaused();

            emit paused();
        }));

    }));
}

void Node::addTorrent(const boost::optional<uint> & uploadLimit,
                      const boost::optional<uint> & downloadLimit,
                      const std::string & name,
                      const std::vector<char> & resumeData,
                      const std::string & savePath,
                      bool paused,
                      const TorrentIdentifier & torrentReference,
                      const AddedTorrent & addedTorrent) {

    // Check that torrent not already present
    libtorrent::sha1_hash infoHash = torrentReference.infoHash();

    if(_torrents.count(infoHash) != 0)
        throw exception::TorrentAlreadyExists(infoHash);

    // Create save_path on disk, return if it does not exist
    //if(!(QDir()).mkpath(params.save_path.c_str())) {
    //    qCCritical(_category) << "Could not create save_path: " << params.save_path.c_str();
    //    return false;
    // }

    // Convert to add torrent parameters
    libtorrent::add_torrent_params params = Node::toAddTorrentParams(uploadLimit,
                                                                     downloadLimit,
                                                                     name,
                                                                     resumeData,
                                                                     savePath,
                                                                     paused,
                                                                     torrentReference);

    // Add torrent to session
    _plugin->submit(extension::request::AddTorrent(params, addedTorrent));
}

void Node::removeTorrent(const libtorrent::sha1_hash & info_hash, const RemovedTorrent & handler) {

    // Find corresponding torrent
    libtorrent::torrent_handle h = _session.find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!h.is_valid())
        throw exception::NoSuchTorrentExists(info_hash);

    // Add torrent to session
    _plugin->submit(extension::request::RemoveTorrent(info_hash, handler));
}

void Node::updateStatus() {

    // Regular torrent level state update
    _session.post_torrent_updates();

    // Plugin level updates
    _plugin->submit(extension::request::UpdateStatus());

    // Update
    for(auto t : _torrents) {

        // Get handle for torrent
        libtorrent::sha1_hash infoHash = t.second->infoHash();
        libtorrent::torrent_handle h = _session.find_torrent(infoHash);

        // Get peer_info for peer, which unfortunately requires
        // getting it for all peers
        std::vector<libtorrent::peer_info> v;

        try {
            h.get_peer_info(v);
        } catch (const libtorrent::libtorrent_exception &) {
            // Handle was invalidated, drop torrent,
            // torrent-removed_alert will come in due time.
            continue;
        }

        // Update peer statuses on torrent
        t.second->updatePeerStatuses(v);
    }
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

        std::clog << "Requested path does not correspond to any presently active torrent." << std::endl;

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
    _session.pop_alerts(&alerts);

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

    std::clog << "Processing alert " << a->message() << std::endl;

    // Select alert type
    if(libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a))
        process(p);
    else if(libtorrent::metadata_failed_alert const * p = libtorrent::alert_cast<libtorrent::metadata_failed_alert>(a))
        process(p);
    else if(libtorrent::listen_succeeded_alert const * p = libtorrent::alert_cast<libtorrent::listen_succeeded_alert>(a))
        process(p);
    else if(libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a))
        process(p);
    else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a))
        process(p);
    else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a))
        process(p);
    else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a))
        process(p);
    else if(libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a))
        process(p);
    else if(libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a))
        process(p);
    else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a))
        process(p);
    else if(libtorrent::torrent_checked_alert const * p = libtorrent::alert_cast<libtorrent::torrent_checked_alert>(a))
        process(p);
    else if(libtorrent::read_piece_alert const * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a))
        processReadPieceAlert(p);
    else if(libtorrent::piece_finished_alert const * p = libtorrent::alert_cast<libtorrent::piece_finished_alert>(a))
        processPieceFinishedAlert(p);
    else if(extension::alert::RequestResult const * p = libtorrent::alert_cast<extension::alert::RequestResult>(a))
        process(p);
    else if(extension::alert::PluginStatus const * p = libtorrent::alert_cast<extension::alert::PluginStatus>(a))
        process(p);

}

void Node::process(const libtorrent::listen_succeeded_alert * p) {

    emit startedListeningOnPort(p->endpoint);
}

/**
int Node::requestResumeData() {

    // Keeps track of how many calls were made
    int resumeCallsMade = 0;

    for(auto mapping : _torrents) {

        // Grab torrent;
        detail::Torrent & t = mapping.second;

        // Skip torrents no yet added
        if (t.state == detail::Torrent::State::being_added)
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
    }

    return resumeCallsMade;
}
*/

void Node::process(const libtorrent::torrent_paused_alert * p) {

    // Get info hash
    libtorrent::sha1_hash infoHash = p->handle.info_hash();

    // Did we get a valid info hash
    if(!infoHash.is_all_zeros()) {

        // then mark as paused
        auto it = _torrents.find(infoHash);
        assert(it != _torrents.cend());

        it->second->paused();
    }
}

void Node::process(const libtorrent::torrent_removed_alert * p) {

    /*
     * NOTICE: Docs say p->handle may be invalid at this time - likely because this is a removal operation,
     * so we must use p->info_hash instead.
     */

    // Get torrent info hash
    libtorrent::sha1_hash info_hash = p->info_hash;

    // Then erase from container
    auto it = _torrents.find(info_hash);
    assert(it != _torrents.cend());
    _torrents.erase(it);

    // and send remove signal
    emit removedTorrent(info_hash);
}

void Node::process(const libtorrent::torrent_resumed_alert * p) {

    // Get info hash
    libtorrent::sha1_hash infoHash = p->handle.info_hash();

    // Did we get a valid info hash
    if(!infoHash.is_all_zeros()) {

        auto it = _torrents.find(infoHash);
        assert(it != _torrents.cend());

        // then mark as resmed
        it->second->resumed();
    }

}

void Node::process(const libtorrent::metadata_received_alert * p) {

    // Syncronous call which returns nullptr if call failes
    libtorrent::torrent_handle h = p->handle;
    boost::shared_ptr<const libtorrent::torrent_info> torrent_info = h.torrent_file();
    libtorrent::sha1_hash infoHash = h.info_hash();

    // If handle and info_hahs is still valid
    if(torrent_info && !infoHash.is_all_zeros()) {

        auto it = _torrents.find(infoHash);

        assert(it != _torrents.cend());

        // then set metadata
        it->second->setMetadata(torrent_info);

    } else
        std::clog << "Invalid handle for received metadata." << std::endl;
}

void Node::process(const libtorrent::metadata_failed_alert *) {

    // what to do?
}

void Node::process(const libtorrent::add_torrent_alert * p) {

    // Did adding succeed?
    if(!p->error) {

        // Get torrent info_hash
        libtorrent::torrent_handle h = p->handle;
        libtorrent::sha1_hash infoHash = h.info_hash();

        if(infoHash.is_all_zeros()) {
            std::clog << "Added torrent has already expired." << std::endl;
            return;
        }

        // This should always hold, as the latter of two outstanding
        // add_torrent_async call will have its add_torrent_alert::error value set.
        assert(_torrents.count(infoHash) == 0);

        // Get current status
        libtorrent::torrent_status status;

        try {
            status = h.status();
        } catch (const libtorrent::libtorrent_exception &) {
            std::clog << "Handle has already expired." << std::endl;
        }

        int uploadLimit = h.upload_limit();
        int downloadLimit = h.download_limit();

        // Create torrent
        std::shared_ptr<Torrent> plugin(new Torrent(h, status, p->params.resume_data, uploadLimit, downloadLimit, _plugin));

        // add to map
        _torrents.insert(std::make_pair(infoHash, plugin));

        // send notification signal
        emit addedTorrent(plugin);

        std::clog << "Adding torrent succeeded." << std::endl;
    } else
        std::clog << "Adding torrent failed:" << p->error.message() << std::endl;
}

void Node::process(const libtorrent::torrent_finished_alert *) {

    /// nothing to do?
}

void Node::process(const libtorrent::state_update_alert * p) {

    for(auto s : p->status) {

        // Try to get torrent refence
        auto it = _torrents.find(s.info_hash);

        // and update status if possible
        if(it != _torrents.cend())
            it->second->updateStatus(s);
        else
            std::clog << "Uknown torrent updated status." << std::endl;
    }

}

void Node::process(const libtorrent::save_resume_data_alert * p) {

    /// No guard required

    // Recover info_hash of torrent
    libtorrent::torrent_handle h = p->handle;

    libtorrent::sha1_hash infoHash = h.info_hash();

    if(infoHash.is_all_zeros()) {
        std::clog << "Handle already expired." << std::endl;
        return;
    }

    // Get reference ot corresponding torrent
    auto it = _torrents.find(h.info_hash());

    if(it == _torrents.cend()) {
        std::clog << "Dropped alert, no correspondign torrent found." << std::endl;
        return;
    }

    // Create resume data buffer
    std::vector<char> resumeData;

    // Write new content to it
    libtorrent::bencode(std::back_inserter(resumeData), *(p->resume_data));

    // Save resume data in torrent
    it->second->setResumeDataGenerationResult(resumeData);
}

void Node::process(const libtorrent::save_resume_data_failed_alert * p) {

    /// No guard required

    // Recover info_hash of torrent
    libtorrent::torrent_handle h = p->handle;

    libtorrent::sha1_hash infoHash = h.info_hash();

    if(infoHash.is_all_zeros()) {
        std::clog << "Handle already expired." << std::endl;
        return;
    }

    // Get reference ot corresponding torrent
    auto it = _torrents.find(h.info_hash());

    if(it == _torrents.cend()) {
        std::clog << "Dropped alert, no correspondign torrent found." << std::endl;
        return;
    }

    // Save empty resume data in torrent, is required
    // to trigger event signal
    it->second->setResumeDataGenerationResult(std::vector<char>());
}

void Node::process(const libtorrent::torrent_checked_alert *) {

    /// nothing to do
}

void Node::process(const libtorrent::peer_connect_alert * p) {

    // Get peer_info for peer, which unfortunately requires
    // getting it for all peers, and as such, we try to update
    // the all statuses.
    std::vector<libtorrent::peer_info> v;

    try {
        p->handle.get_peer_info(v);
    } catch (const libtorrent::libtorrent_exception &) {
        // Handle was invalidated, drop alert
        return;
    }

    // Get info_hash, drop alert if the handle gave us invalid info hash
    libtorrent::sha1_hash infoHash = p->handle.info_hash();

    if(infoHash.is_all_zeros())
        return;

    // Find torrent
    auto it = _torrents.find(infoHash);

    // If its not registerd, then we ignore alert
    if(it == _torrents.cend())
        return;

    // Update peer statuses on torrent
    it->second->updatePeerStatuses(v);
}

void Node::process(const libtorrent::peer_disconnected_alert * p) {

    // Get info_hash, drop alert if the handle gave us invalid info hash
    libtorrent::sha1_hash infoHash = p->handle.info_hash();

    if(infoHash.is_all_zeros())
        return;

    // Find torrent
    auto it = _torrents.find(infoHash);

    // If its not registerd, then we ignore alert
    if(it == _torrents.cend())
        return;

    // (try to) remove peer from torrent
    it->second->removePeer(p->ip);
}

void Node::processReadPieceAlert(const libtorrent::read_piece_alert *) {

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
//        std::clog << "There was some sort of error in reading a piece: " << QString::fromStdString(p->ec.message())  << std::endl;
//    } else {

//        // Notify torrent
//        _torrents[infoHash]->pieceRead(p->buffer, p->piece, p->size);
//    }
}

void Node::processPieceFinishedAlert(const libtorrent::piece_finished_alert *) {

//    // Get info hash for torrent from which this read piece comes from
//    const libtorrent::sha1_hash infoHash = p->handle.info_hash();

//    Q_ASSERT(_torrents.contains(infoHash));

//    // Notify torrent
//    _torrents[infoHash]->pieceFinished(p->piece_index);
}

void Node::process(const extension::alert::RequestResult * p) {

    /// No guard here, we allow user code to run

    // Make loaded callback
    p->loadedCallback();
}

void Node::process(const extension::alert::PluginStatus * p) {

    // Update torrent plugin statuses
    for(auto status: p->status.plugins) {

        auto it = _torrents.find(status.first);

        // If there is a torrent for this plugin, then update status
        if(it != _torrents.cend())
            it->second->updateTorrentPluginStatus(status.second);
    }

    // Do other stuff when plugin status is extended
}

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

    std::clog << "Stream unregistered due to some error. << std::endl;

    //emit some sort of signal about error
}
*/

/**
void Node::changeDownloadingLocationFromThisPiece(const libtorrent::sha1_hash & infoHash, int pieceIndex) {

    // Check that torrent exists
    if(!_torrents.contains(infoHash)) {

        std::clog << "Changing download location requested for torrent which does not exist." << std::endl;
        return;
    }

    // Check that
    if(_torrents[infoHash]->pluginInstalled() != PluginInstalled::Buyer) {

        std::clog << "Changing download location requested for with plugin which does not have a buyer torrent plugin installed on torrent." << std::endl;
        return;
    }

    // Ask torrent to relocate
    _plugin->submitTorrentPluginRequest(new ChangeDownloadLocation(infoHash, pieceIndex));
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
        return it->second;
}

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

libtorrent::add_torrent_params Node::toAddTorrentParams(const boost::optional<uint> & uploadLimit,
                                                        const boost::optional<uint> & downloadLimit,
                                                        const std::string & name,
                                                        const std::vector<char> & resumeData,
                                                        const std::string & savePath,
                                                        bool pause,
                                                        const TorrentIdentifier & torrentIdentifier) noexcept {

    // Initialize with default values
    libtorrent::add_torrent_params params;

    if(uploadLimit.is_initialized())
        params.upload_limit = uploadLimit.value();

    if(downloadLimit.is_initialized())
        params.download_limit = downloadLimit.value();

    params.name = name;
    params.resume_data = resumeData;
    params.save_path = savePath;

    // NB: initial flags set in params is
    // default_flags = flag_pinned | flag_update_subscribe | flag_auto_managed | flag_paused | flag_apply_ip_filter

    // Pause, if paused
    if(pause)
        params.flags += libtorrent::add_torrent_params::flags_t::flag_paused;

    // Remove auto-managing
    params.flags -= libtorrent::add_torrent_params::flags_t::flag_auto_managed;

    // Torrent refernce (usual, and safer, visitor pattern is a bit too heavy)
    switch(torrentIdentifier.type()) {

        case TorrentIdentifier::Type::Hash: params.info_hash = torrentIdentifier.infoHash(); break;

        case TorrentIdentifier::Type::MagnetLink: params.url = torrentIdentifier.magnetLink().toURI(); break;

        case TorrentIdentifier::Type::TorrentFile: params.ti = torrentIdentifier.torrentFile(); break;

        default:
            assert(false);
    }

    return params;
}

}
}
