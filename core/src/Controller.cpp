/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/Controller.hpp>

namespace joystream {
namespace core {

    Controller::Torrent::Torrent(const libtorrent::sha1_hash & infoHash,
                                 const std::string & name,
                                 const std::string & savePath,
                                 const std::vector<char> & resumeData,
                                 quint64 flags,
                                 //libtorrent::torrent_info * torrentInfo,
                                 const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile,
                                 Status event)
        : _infoHash(infoHash)
        , _name(name)
        , _savePath(savePath)
        , _resumeData(resumeData)
        , _flags(flags)
        //, _handle(handle)
        //, _torrentInfo(torrentInfo)
        , _status(event)
        , _pluginInstalled(PluginInstalled::None)
        , _model(infoHash,
                 name,
                 savePath,
                 torrentFile) {
    }

    void Controller::Torrent::addPlugin(const SellerTorrentPlugin::Status & status) {

        Q_ASSERT(_pluginInstalled == PluginInstalled::None);
        _pluginInstalled = PluginInstalled::Seller;
        _model.addPlugin(status);
    }

    void Controller::Torrent::addPlugin(const BuyerTorrentPlugin::Status & status) {

        Q_ASSERT(_pluginInstalled == PluginInstalled::None);
        _pluginInstalled = PluginInstalled::Buyer;
        _model.addPlugin(status);
    }

    libtorrent::sha1_hash Controller::Torrent::infoHash() const {
        return _infoHash;
    }

    std::string Controller::Torrent::name() const {
        return _name;
    }

    void Controller::Torrent::setName(const std::string & name) {
        _name = name;
    }

    std::string Controller::Torrent::savePath() const {
        return _savePath;
    }

    void Controller::Torrent::setSavePath(const std::string & savePath) {
        _savePath = savePath;
    }

    std::vector<char> Controller::Torrent::resumeData() const {
        return _resumeData;
    }

    void Controller::Torrent::setResumeData(const std::vector<char> & resumeData) {
        _resumeData = resumeData;
    }

    quint64 Controller::Torrent::flags() const {
        return _flags;
    }

    void Controller::Torrent::setFlags(quint64 flags) {
        _flags = flags;
    }

    /**
    libtorrent::torrent_info * Controller::Torrent::torrentInfo() {
        return _torrentInfo;
    }
    */

    libtorrent::torrent_handle Controller::Torrent::handle() const {
        return _handle;
    }

    void Controller::Torrent::setHandle(const libtorrent::torrent_handle & handle) {
        _handle = handle;
    }

    Controller::Torrent::Status Controller::Torrent::status() const {
        return _status;
    }

    void Controller::Torrent::setStatus(Status event) {
        _status = event;
    }

    PluginInstalled Controller::Torrent::pluginInstalled() const {
        return _pluginInstalled;
    }

    TorrentViewModel * Controller::Torrent::model() {
        return &_model;
    }

    void Controller::Torrent::addStream(Stream * stream) {
        _streams.insert(stream);
    }

    void Controller::Torrent::removeStream(Stream * stream) {
        _streams.remove(stream);
    }

    void Controller::Torrent::pieceRead(const boost::shared_array<char> & buffer,
                                        int pieceIndex,
                                        int size) {

        // Iterate streams and notify them
        for(QSet<Stream *>::iterator i = _streams.begin(),
            end = _streams.end();
            i != end;i++)
            (*i)->pieceRead(buffer, pieceIndex, size);
    }

    void Controller::Torrent::pieceFinished(int piece) {

        // Iterate streams and notify them
        for(QSet<Stream *>::iterator i = _streams.begin(),
            end = _streams.end();
            i != end;i++)
            (*i)->pieceDownloaded(piece);
    }
}
}


/**
 * Controller
 */

#include <core/exceptions/ListenOnException.hpp>
#include <core/Stream.hpp>

#include <extension/Alert/StartedSellerTorrentPlugin.hpp>
#include <extension/Alert/StartedBuyerTorrentPlugin.hpp>
#include <extension/Alert/BuyerTorrentPluginStatusAlert.hpp>
#include <extension/Alert/SellerTorrentPluginStatusAlert.hpp>
#include <extension/Alert/PluginStatusAlert.hpp>
#include <extension/Alert/SellerPeerAddedAlert.hpp>
#include <extension/Alert/BuyerPeerAddedAlert.hpp>
#include <extension/Alert/SellerPeerPluginRemovedAlert.hpp>
#include <extension/Alert/BuyerPeerPluginRemovedAlert.hpp>
#include <extension/Request/StartSellerTorrentPlugin.hpp>
#include <extension/Request/StartBuyerTorrentPlugin.hpp>
#include <extension/Request/StartObserverTorrentPlugin.hpp>
#include <extension/Request/ChangeDownloadLocation.hpp>
#include <wallet/Manager.hpp>

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
#include <QNetworkAccessManager>

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

// Register type for QMetaObject::invokeMethod
Q_DECLARE_METATYPE(const libtorrent::alert*)

namespace joystream {
namespace core {

    Controller::Controller(const Configuration & configuration, Wallet::Manager * wallet, QNetworkAccessManager * manager, QLoggingCategory & category)
        : _state(State::normal)
        , _session(new libtorrent::session(libtorrent::fingerprint(CORE_EXTENSION_FINGERPRINT, CORE_VERSION_MAJOR, CORE_VERSION_MINOR, 0, 0),
                       libtorrent::session::add_default_plugins + libtorrent::session::start_default_features,
                       libtorrent::alert::error_notification +
                       libtorrent::alert::tracker_notification +
                       libtorrent::alert::debug_notification +
                       libtorrent::alert::status_notification +
                       libtorrent::alert::progress_notification +
                       libtorrent::alert::performance_warning +
                       libtorrent::alert::stats_notification))
        , _wallet(wallet) // add autosave to configuration later?? does user even need to control that?
        , _category(category)
        , _manager(manager)
        , _plugin(new Plugin(wallet, _category))
        , _portRange(configuration.getPortRange()) {
        //, _server(9999, this) {

        qCDebug(_category) << "Libtorrent session started on port" << QString::number(_session->listen_port());

        // Register types for signal and slots
        qRegisterMetaType<libtorrent::sha1_hash>();
        qRegisterMetaType<std::string>();
        qRegisterMetaType<libtorrent::error_code>();
        qRegisterMetaType<std::vector<libtorrent::torrent_status>>();
        qRegisterMetaType<libtorrent::torrent_status>();
        qRegisterMetaType<Coin::Transaction>(); // Probably should not be here

        // Register type for QMetaObject::invokeMethod
        qRegisterMetaType<const libtorrent::alert*>();

        // Set libtorrent to call processAlert when alert is created
        _session->set_alert_dispatch(boost::bind(&Controller::libtorrent_alert_dispatcher_callback, this, _1));

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
        */

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
            qCDebug(_category) << "Started streaming server on port:" << _streamingServer.serverPort();
        else
            qCDebug(_category) << "Could not start streaming server on port:" << _streamingServer.serverPort();

        ////////////////////////////////////////////////////
        // Create settings for sesion
        // this has all been hard coded last moment due to new
        // libtorrent changes.
        ////////////////////////////////////////////////////
        /**libtorrent::settings_pack settings;


        settings.set_int(libtorrent::settings_pack::active_loaded_limit, 20);
        settings.set_int(libtorrent::settings_pack::choking_algorithm, libtorrent::settings_pack::rate_based_choker);
        //settings.set_int(libtorrent::settings_pack::half_open_limit, atoi(arg))
        settings.set_bool(libtorrent::settings_pack::allow_multiple_connections_per_ip, true);
        //settings.set_bool(libtorrent::settings_pack::use_disk_read_ahead, false);
        //settings.set_int(libtorrent::settings_pack::suggest_mode, libtorrent::settings_pack::suggest_read_cache);
        //settings.set_int(libtorrent::settings_pack::peer_timeout, atoi(arg));
        //settings.set_bool(libtorrent::settings_pack::announce_to_all_tiers, true);
        //settings.set_int(libtorrent::settings_pack::hashing_threads, atoi(arg));
        //settings.set_int(libtorrent::settings_pack::download_rate_limit, atoi(arg) * 1000);
        //settings.set_int(libtorrent::settings_pack::upload_rate_limit, atoi(arg) * 1000);
        //settings.set_int(libtorrent::settings_pack::unchoke_slots_limit, atoi(arg));
        //settings.set_int(libtorrent::settings_pack::urlseed_wait_retry, atoi(arg));
        settings.set_bool(libtorrent::settings_pack::enable_dht, true);
        //settings.set_int(libtorrent::settings_pack::listen_queue_size, atoi(arg));
        //settings.set_int(libtorrent::settings_pack::out_enc_policy, libtorrent::settings_pack::pe_forced);
        //settings.set_int(libtorrent::settings_pack::in_enc_policy, libtorrent::settings_pack::pe_forced);
        //settings.set_int(libtorrent::settings_pack::allowed_enc_level, libtorrent::settings_pack::pe_rc4);
        //settings.set_bool(libtorrent::settings_pack::prefer_rc4, true);
        //settings.set_int(libtorrent::settings_pack::max_peerlist_size, atoi(arg));
        //settings.set_int(libtorrent::settings_pack::max_paused_peerlist_size, atoi(arg) / 2);
        //settings.set_int(libtorrent::settings_pack::connections_limit, atoi(arg));
        //settings.set_str(libtorrent::settings_pack::i2p_hostname, arg);
        //settings.set_int(libtorrent::settings_pack::i2p_port, 7656);
        //settings.set_int(libtorrent::settings_pack::proxy_type, libtorrent::settings_pack::i2p_proxy);
        //settings.set_int(libtorrent::settings_pack::cache_size, atoi(arg));
        //settings.set_bool(libtorrent::settings_pack::use_read_cache, atoi(arg) > 0);
        //settings.set_int(libtorrent::settings_pack::cache_buffer_chunk_size, atoi(arg) / 100);
        //settings.set_int(libtorrent::settings_pack::allowed_fast_set_size, atoi(arg));
        //settings.set_int(libtorrent::settings_pack::read_cache_line_size, atoi(arg));
        //settings.set_int(libtorrent::settings_pack::mixed_mode_algorithm, libtorrent::settings_pack::prefer_tcp);
        //settings.set_bool(libtorrent::settings_pack::enable_outgoing_tcp, true);
        //settings.set_bool(libtorrent::settings_pack::enable_incoming_tcp, true);
        //settings.set_str(libtorrent::settings_pack::proxy_hostname, arg);
        //settings.set_int(libtorrent::settings_pack::proxy_port, atoi(port));

        //if (settings.get_int(libtorrent::settings_pack::proxy_type) == libtorrent::settings_pack::none)
        //        settings.set_int(libtorrent::settings_pack::proxy_type, libtorrent::settings_pack::socks5);

        //settings.set_str(libtorrent::settings_pack::proxy_username, arg);
        //settings.set_str(libtorrent::settings_pack::proxy_password, pw);
        //settings.set_int(libtorrent::settings_pack::proxy_type, settings_pack::socks5_pw);
        //settings.set_str(libtorrent::settings_pack::outgoing_interfaces, arg);
        settings.set_bool(libtorrent::settings_pack::enable_upnp, true);
        settings.set_bool(libtorrent::settings_pack::enable_natpmp, true);
        settings.set_bool(libtorrent::settings_pack::enable_lsd, true);
        //settings.set_str(libtorrent::settings_pack::mmap_cache, arg);
        //settings.set_bool(libtorrent::settings_pack::contiguous_recv_buffer, false);
        //settings.set_int(libtorrent::settings_pack::active_downloads, atoi(arg));
        //settings.set_int(libtorrent::settings_pack::active_limit, atoi(arg) * 2);
        //settings.set_int(libtorrent::settings_pack::active_seeds, atoi(arg));
        //settings.set_int(libtorrent::settings_pack::active_limit, atoi(arg) * 2);


        // setup default values
        libtorrent::high_performance_seed(settings);

        // Listening settings
        //settings.set_str(libtorrent::settings_pack::listen_interfaces, "0.0.0.0:6881");

        //settings.set_str(libtorrent::settings_pack::user_agent, CLIENT_FINGERPRINT); //  + JOYSTREAM_VERSION_MAJOR + JOYSTREAM_VERSION_MINOR
        //settings.set_str(libtorrent::settings_pack::user_agent, "client_test/" LIBTORRENT_VERSION);

        settings.set_int(libtorrent::settings_pack::alert_mask,
                         //libtorrent::alert::all_categories
                         libtorrent::alert::error_notification +
                         libtorrent::alert::tracker_notification +
                         libtorrent::alert::debug_notification +
                         libtorrent::alert::status_notification +
                         libtorrent::alert::progress_notification +
                         libtorrent::alert::performance_warning +
                         libtorrent::alert::stats_notification
                         );

        // Create session, which immediately starts server
        // ======================================
        _session = new libtorrent::session(settings);
        */

        qCDebug(_category) << "Libtorrent session started";

        // Set session settings - these acrobatics with going back and forth seem to indicate that I may have done it incorrectly
        std::vector<char> buffer;
        libtorrent::bencode(std::back_inserter(buffer), configuration.getLibtorrentSessionSettingsEntry());
        libtorrent::lazy_entry settingsLazyEntry;
        libtorrent::error_code lazyBdecodeEc;
        libtorrent::lazy_bdecode(&buffer[0], &buffer[0] + buffer.size(), settingsLazyEntry, lazyBdecodeEc);
        _session->load_state(settingsLazyEntry);

        // Add DHT routing nodes
        // ======================================
        const std::vector<std::pair<std::string, int>> & dhtRouters = configuration.getDhtRouters();
        for(std::vector<std::pair<std::string, int>>::const_iterator i = dhtRouters.begin(),
                end(dhtRouters.end());i != end; ++i)
            _session->add_dht_router(*i); // Add router to session

        /**
        // Start dht node?
        libtorrent::dht_settings dht;
        //dht.privacy_lookups = true;
        _session->set_dht_settings(dht);

        settings.set_bool(libtorrent::settings_pack::use_dht_as_fallback, false); // use as main?

        _session->add_dht_router(std::make_pair(std::string("router.bittorrent.com"), 6881));
        _session->add_dht_router(std::make_pair(std::string("router.utorrent.com"), 6881));
        _session->add_dht_router(std::make_pair(std::string("router.bitcomet.com"), 6881));

        // Setup alert processing callback
        _session->set_alert_notify(boost::bind(&Controller::libtorrent_entry_point_alert_notification, this));
        */

        // Add plugin extension
        _session->add_extension(boost::shared_ptr<libtorrent::plugin>(_plugin));

        // Start timer which calls session.post_torrent_updates at regular intervals
        _statusUpdateTimer.setInterval(CORE_CONTROLLER_POST_TORRENT_UPDATES_DELAY);

        QObject::connect(&_statusUpdateTimer,
                         SIGNAL(timeout()),
                         this,
                         SLOT(callPostTorrentUpdates()));

        _statusUpdateTimer.start();

        // Add all torrents, but this ust be AFTER session.listen_on(),
        // because otherwise adding to session won't work.
        QVector<Torrent::Configuration> torrents = configuration.torrents();

        for(QVector<Torrent::Configuration>::const_iterator i = torrents.begin(),
                end(torrents.end());i != end; ++i) {

            /**
            // Try to add torrent, without prompting user
            if(!addTorrent(*i, false)) {
                qCCritical(_category) << "Unable to add torrent configuration to session";
                return;
            }
            */
        }
    }

    Controller::~Controller() {

        // Delete all torrents
        for(QMap<libtorrent::sha1_hash, Torrent *>::const_iterator
            i = _torrents.constBegin(),
            end = _torrents.constEnd();
            i != end;i++)
            delete i.value();

        // Delete session
        delete _session;
    }

    void Controller::callPostTorrentUpdates() {
        _session->post_torrent_updates();
    }

    void Controller::handleConnection() {

        // Create handler for each pending connection
        // socket is owned by _server
        while(QTcpSocket * socket = _streamingServer.nextPendingConnection()) {

            qDebug(_category) << "New connection opened.";

            // Create stream
            Stream * stream = new Stream(socket, this);
        }
    }

    void Controller::handleAcceptError(QAbstractSocket::SocketError socketError) {

        qDebug(_category) << "Failed to accept connection.";
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
        qCDebug(_category) << "handleFailedStreamCreation";
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

            qCDebug(_category) << "Requested path does not correspond to any presently active torrent.";

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

    /*
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

    void Controller::libtorrent_alert_dispatcher_callback(const std::auto_ptr<libtorrent::alert> & alertAutoPtr) {

        // Grab alert pointer and release the auto pointer, this way the alert is not automatically
        // deleted when alertAutoPtr goes out of scope.
        // **Registering auto_ptr with MOC is not worth trying**
        //const libtorrent::alert * a = alertAutoPtr.release();

        const libtorrent::alert * a = alertAutoPtr.get();

        std::auto_ptr<libtorrent::alert> stdAutoPtr = a->clone();

        libtorrent::alert * freedPointer = stdAutoPtr.release();

        // Tell bitswapr thread to run processAlert later with given alert as argument
        QMetaObject::invokeMethod(this, "processAlert", Q_ARG(const libtorrent::alert*, freedPointer));
    }

    void Controller::libtorrent_entry_point_alert_notification() {
        QMetaObject::invokeMethod(this, "processAlertQueue");
    }

    /**
    void Controller::processAlertQueue() {

        // Populate vector with alerts
        std::vector<libtorrent::alert *> alerts;
        _session->pop_alerts(&alerts);

        // NOTE on owernship of alerts (alert.hpp)
        // Alerts returned by pop_alerts() are only valid until the next call to
        // pop_alerts(). You may not copy an alert object to access it after the next
        // call to pop_alerts(). Internal members of alerts also become invalid once
        // pop_alerts() is called again.

        for(std::vector<libtorrent::alert *>::const_iterator i = alerts.begin(),
            end = alerts.end();
            i != end;
            i++) {

            // Get alert
            libtorrent::alert * alert = *i;

            qCDebug(_category) << "Processing alert" << QString::fromStdString(alert->message());

            // Process
            processAlert(alert);
        }

    }
    */

    void Controller::processAlert(const libtorrent::alert * a) {

        // Check that alert has been stuck in event queue and corresponds to recenty
        // removed torrent.

        // if(something)
        //    something;

        // In each case, tell bitswapr thread to run the given method
        /**
         * Alerts from libtorrent core
         */
        if(libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a))
            processMetadataReceivedAlert(p);
        else if(libtorrent::metadata_failed_alert const * p = libtorrent::alert_cast<libtorrent::metadata_failed_alert>(a))
            processMetadataFailedAlert(p);
        else if(libtorrent::listen_failed_alert const * p = libtorrent::alert_cast<libtorrent::listen_failed_alert>(a))
            processListenFailedAlert(p);
        else if(libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a))
            processAddTorrentAlert(p);
        else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a))
            processTorrentFinishedAlert(p);
        else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a))
            processTorrentPausedAlert(p);
        else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a))
            processStatusUpdateAlert(p);
        else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a))
            processTorrentRemovedAlert(p);
        else if(libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a))
            processSaveResumeDataAlert(p);
        else if(libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a))
            processSaveResumeDataFailedAlert(p);
        else if(libtorrent::torrent_checked_alert const * p = libtorrent::alert_cast<libtorrent::torrent_checked_alert>(a))
            processTorrentCheckedAlert(p);
        else if(libtorrent::read_piece_alert const * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a))
            processReadPieceAlert(p);
        else if(libtorrent::piece_finished_alert const * p = libtorrent::alert_cast<libtorrent::piece_finished_alert>(a))
            processPieceFinishedAlert(p);
        else if(PluginStatusAlert const * p = libtorrent::alert_cast<PluginStatusAlert>(a))
            processPluginStatusAlert(p);
        /**
         * Alerts from plugin
         */
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
        //else if(const TorrentPluginStartedAlert * p = libtorrent::alert_cast<TorrentPluginStartedAlert>(a))
        //    processTorrentPluginStartedAlert(p);

        // Delete alert ** DELETE WHEN GOING TO NEW RELEASE ***
        // ===============================================
        delete a;
    }

    void Controller::sellerPeerPluginRemoved(const libtorrent::sha1_hash & infoHash, const libtorrent::tcp::endpoint & endPoint) {

    }

    /**
    // NB!: Alreayd assumes session is paused and does not check sourceForLastResumeDataCall and numberOfOutstandingResumeDataCalls before starting,
    // assumes they equal NONE,0 respectively.
    */

    int Controller::makeResumeDataCallsForAllTorrents() {

        // Get all handles (should be same torrents as int addTorrentParameters, but we dont check this here
        std::vector<libtorrent::torrent_handle> handles = _session->get_torrents();

        // Keeps track of how many calls were made
        int resumeCallsMade = 0;

        // Iterate all torrents, and try to save
        for (std::vector<libtorrent::torrent_handle>::iterator i = handles.begin(),
             end(handles.end());i != end; ++i) {

            // Get handle
            libtorrent::torrent_handle & h = *i;

            // Get torrent info hash
            libtorrent::sha1_hash infoHash = h.info_hash();

            Q_ASSERT(_torrents.contains(infoHash));

            // Grab torrent;
            Torrent * torrent = _torrents[infoHash];

            // Dont save data if
            if (torrent->status() != Torrent::Status::nothing // are in wrong state
                || !h.is_valid() // dont have valid handle
                || !h.need_save_resume_data() // dont need to
                || !h.status().has_metadata) // or dont have metadata
                continue;

            // Save resume data
            h.save_resume_data();

            // Count call
            resumeCallsMade++;

            // Change expected event of torrent
            _torrents[infoHash]->setStatus(Torrent::Status::asked_for_resume_data);
        }

        return resumeCallsMade;
    }

    void Controller::processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p) {

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
    }

    void Controller::processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p) {

        /*
         * NOTICE: Docs say p->handle may be invalid at this time,
         * so we must use p->info_hash instead.
         */

        // Get torrent info hash
        libtorrent::sha1_hash info_hash = p->info_hash;

        // Remove from view
        torrentRemoved(info_hash);

        qCDebug(_category) << "Found match and removed it.";
    }

    void Controller::processMetadataReceivedAlert(libtorrent::metadata_received_alert const * p) {

        // Get handle for torrent
        libtorrent::torrent_handle h = p->handle;

        // Process if handle is valid
        if (h.is_valid()) {

            // get torrent info
            //boost::shared_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();
            boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();

            //const libtorrent::torrent_info & torrentInfo = h.get_torrent_info();

            // USE THIS INFORMATION FOR SOMETHING LATER

            // Put in model later
            qCDebug(_category) << "Metadata saved.";

        } else
            qCDebug(_category) << "Invalid handle for received metadata.";
    }

    void Controller::processMetadataFailedAlert(libtorrent::metadata_failed_alert const * p) {
        // WHAT DO WE DO HERE?
        qCDebug(_category) << "Invalid metadata received.";
        throw std::runtime_error("Invalid metadata");
    }

    void Controller::processListenFailedAlert(libtorrent::listen_failed_alert const * p) {
        throw std::runtime_error("Failed to start listening"); // p->listen_interface()
    }

    void Controller::processAddTorrentAlert(libtorrent::add_torrent_alert const * p) {

        Q_ASSERT(_state == State::normal);
        Q_ASSERT(_torrents.contains(p->params.info_hash));

        // Get torrent
        Torrent * torrent = _torrents[p->params.info_hash];

        Q_ASSERT(torrent->status() == Torrent::Status::being_async_added_to_session);

        // Check if there was an error
        if (p->error) {

            qCDebug(_category) << "Adding torrent failed, must be removed.";

            /**
             * Remove torrent here, so that it does not hanga around and cause problems.
             */

            // old name arg: p->params.ti.get() != 0 ? p->params.ti->name() : name = p->params.name
            emit failedToAddTorrent(p->params.name, p->params.info_hash, p->error);

        } else {

            qCDebug(_category) << "Adding torrent succeeded.";

            /*
            h.set_max_connections(max_connections_per_torrent);
            h.set_max_uploads(-1);
            h.set_upload_limit(torrent_upload_limit);
            h.set_download_limit(torrent_download_limit);
            h.use_interface(outgoing_interface.c_str());
            */

            // Give copy of handle
            torrent->setHandle(p->handle);

            // Update expected event on torrent
            torrent->setStatus(Torrent::Status::torrent_checked);

            // Send notification signal
            emit addedTorrent(torrent->model());
        }
    }

    void Controller::processTorrentFinishedAlert(libtorrent::torrent_finished_alert const * p) {

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

    void Controller::processStatusUpdateAlert(libtorrent::state_update_alert const * p) {
        update(p->status);
    }

    void Controller::processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p) {

        // Get info hash for torrent
        libtorrent::sha1_hash info_hash = p->handle.info_hash();

        // Grab torrent
        Q_ASSERT(_torrents.contains(info_hash));
        Torrent * torrent = _torrents[info_hash];

        // Check that alert was expected
        Q_ASSERT(torrent->status() == Torrent::Status::asked_for_resume_data);

        // Reset expected event
        torrent->setStatus(Torrent::Status::nothing);

        // Create resume data buffer
        std::vector<char> resumeData;

        // Write new content to it
        bencode(std::back_inserter(resumeData), *(p->resume_data));

        // Save resume data in torrent
        torrent->setResumeData(resumeData);

        // If this is part of closing client, then
        if(_state == State::waiting_for_resume_data_while_closing) {

            // Check if all there are any pending resume data requests
            for(QMap<libtorrent::sha1_hash, Torrent *>::const_iterator
                i = _torrents.begin(),
                end = _torrents.end(); i != end;i++) {

                // End processing if alert is expected
                if((i.value())->status() == Torrent::Status::asked_for_resume_data)
                    return;
            }

            // Close client
            finalize_close();
        }
    }

    void Controller::processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p) {

        qCCritical(_category) << "Failed to generate resume data for some reason.";
    }

    void Controller::processTorrentCheckedAlert(libtorrent::torrent_checked_alert const * p) {

        // Get handle for torrent
        libtorrent::torrent_handle h = p->handle;

        // Process if handle is valid
        if(h.is_valid()) {

            // Get info hash of torrent
            libtorrent::sha1_hash infoHash = h.info_hash();

            //qCCritical(_category) << QString::fromStdString(h.info_hash().to_string()); // infoHash.to_string()
            //qCCritical(_category) << QString::fromStdString(_torrents.first()->infoHash().to_string());

            // Make sure the torrent exists
            Q_ASSERT(_torrents.contains(infoHash));

            // Grab torrent
            Torrent * torrent = _torrents[infoHash];

            // Assert that torrent_checked_alert was expected
            Q_ASSERT(torrent->status() == Torrent::Status::torrent_checked);

            /**
            // if a configuration was saved, i.e. started from disk, then we just use it
            if(_pendingConfigurations.contains(info_hash)) {

                // Remove torrent plugin configuration
                const TorrentPlugin::Configuration * configuration = _pendingConfigurations.take(info_hash);

                // Send configuration to plugin
                _plugin->submitPluginRequest(new StartTorrentPlugin(info_hash, configuration));

                // Reset event
                torrent.setEvent(Torrent::ExpectedEvent::nothing);

            } */

            // if a configuration was saved, i.e. started from disk, then we just use it
            if(_pendingSellerTorrentPluginConfigurations.contains(infoHash)) {

                // Remove torrent plugin configuration
                const SellerTorrentPlugin::Configuration configuration = _pendingSellerTorrentPluginConfigurations.take(infoHash);

                // Send configuration to plugin
                _plugin->submitPluginRequest(new StartSellerTorrentPlugin(infoHash, configuration));

                // Reset event
                torrent->setStatus(Torrent::Status::nothing);

            } else if(_pendingBuyerTorrentPluginConfigurationAndUtxos.contains(infoHash)) {

                // Remove torrent plugin configuration and utxo
                QPair<BuyerTorrentPlugin::Configuration, Coin::UnspentP2PKHOutput> p = _pendingBuyerTorrentPluginConfigurationAndUtxos.take(infoHash);

                // Send configuration to plugin
                _plugin->submitPluginRequest(new StartBuyerTorrentPlugin(infoHash, p.first, p.second));

                // Reset event
                torrent->setStatus(Torrent::Status::nothing);

            } /** else if(_pendingObserverTorrentPluginConfigurations.contains(info_hash)) {

                // Remove torrent plugin configuration
                const ObserverTorrentPlugin::Configuration configuration = _pendingObserverTorrentPluginConfigurations.take(info_hash);

                // Send configuration to plugin
                _plugin->submitPluginRequest(new StartObserverTorrentPlugin(info_hash, configuration));

                // Reset event
                torrent.setEvent(Torrent::ExpectedEvent::nothing);

            }*/ else {

                // Get torrent information
                //boost::shared_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();
                boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();
                const libtorrent::torrent_info * torrentInfoPtr = torrentInfoIntrusivePtr.get();
                libtorrent::torrent_info torrentInfo = *torrentInfoPtr; //h.get_torrent_info();

                // Get torrent status
                libtorrent::torrent_status torrentStatus = h.status();

                // No longer used: Expect user to set configurations
                //torrent->setStatus(Torrent::Status::torrent_plugin_configuration_from_user);

                // Send signal
                emit torrentCheckedButHasNoPlugin(torrentInfo, torrentStatus);
            }

        } else {

            qCDebug(_category) << "Invalid handle for checked torrent.";
            Q_ASSERT(false);
        }
    }

    void Controller::processReadPieceAlert(const libtorrent::read_piece_alert * p) {

        // Get info hash for torrent from which this read piece comes from
        const libtorrent::sha1_hash infoHash = p->handle.info_hash();

        Q_ASSERT(_torrents.contains(infoHash));

        /**
        // Notify torrent view model
        _torrents[infoHash]->model()->pieceRead(p->ec,
                                                p->buffer,
                                                p->piece,
                                                p->size);
        */

        if(p->ec) {
            qCDebug(_category) << "There was some sort of error in reading a piece: " << QString::fromStdString(p->ec.message());
        } else {

            // Notify torrent
            _torrents[infoHash]->pieceRead(p->buffer, p->piece, p->size);
        }
    }

    void Controller::processPieceFinishedAlert(const libtorrent::piece_finished_alert * p) {

        // Get info hash for torrent from which this read piece comes from
        const libtorrent::sha1_hash infoHash = p->handle.info_hash();

        Q_ASSERT(_torrents.contains(infoHash));

        // Notify torrent
        _torrents[infoHash]->pieceFinished(p->piece_index);
    }

    void Controller::processStartedSellerTorrentPlugin(const StartedSellerTorrentPlugin * p) {

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

    void Controller::processStartedBuyerTorrentPlugin(const StartedBuyerTorrentPlugin * p) {

        Q_ASSERT(_torrents.contains(p->infoHash()));

        Torrent * torrent = _torrents[p->infoHash()];

        Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::None);

        // Update information about plugin installed on torrent
        torrent->addPlugin(p->status());

        // Send signal
        // *** DISCONTINUED, THESE ARE INSTALLED AS SIGNALS ON TORRENT VIEW MODELS
        //emit startedBuyerTorrentPlugin(torrent->model()->buyerTorrentPluginViewModel());
    }

    void Controller::processBuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert * p) {

        Q_ASSERT(_torrents.contains(p->infoHash()));

        // Get view model for torrent
        TorrentViewModel * model = _torrents[p->infoHash()]->model();

        Q_ASSERT(model->pluginInstalled() == PluginInstalled::Buyer);

        // Update
        model->update(p->status());
    }

    void Controller::processSellerTorrentPluginStatusAlert(const SellerTorrentPluginStatusAlert * p) {

        Q_ASSERT(_torrents.contains(p->infoHash()));

        // Get view model for torrent
        TorrentViewModel * model = _torrents[p->infoHash()]->model();

        Q_ASSERT(model->pluginInstalled() == PluginInstalled::Seller);

        // Update
        model->update(p->status());
    }

    void Controller::processPluginStatusAlert(const PluginStatusAlert * p) {

        //
        emit pluginStatusUpdate(p->status());
    }

    void Controller::processSellerPeerAddedAlert(const SellerPeerAddedAlert * p) {

        Q_ASSERT(_torrents.contains(p->infoHash()));

        Torrent * torrent = _torrents[p->infoHash()];

        torrent->model()->addPeer(p->endPoint(), p->status());
    }

    void Controller::processBuyerPeerAddedAlert(const BuyerPeerAddedAlert * p) {

        Q_ASSERT(_torrents.contains(p->infoHash()));

        Torrent * torrent = _torrents[p->infoHash()];

        torrent->model()->addPeer(p->endPoint(), p->status());
    }

    void Controller::processSellerPeerPluginRemovedAlert(const SellerPeerPluginRemovedAlert * p) {

        Q_ASSERT(_torrents.contains(p->infoHash()));

        // Grab torrent
        Torrent * torrent = _torrents[p->infoHash()];

        Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::Seller);

        // Notify view model
        torrent->model()->removePeer(p->endPoint());
    }

    void Controller::processBuyerPeerPluginRemovedAlert(const BuyerPeerPluginRemovedAlert * p) {

        Q_ASSERT(_torrents.contains(p->infoHash()));

        // Grab torrent
        Torrent * torrent = _torrents[p->infoHash()];

        Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::Buyer);

        // Notify view model
        torrent->model()->removePeer(p->endPoint());
    }

    void Controller::update(const std::vector<libtorrent::torrent_status> & statuses) {

        for(std::vector<libtorrent::torrent_status>::const_iterator
            i = statuses.begin(),
            end = statuses.end(); i != end;i++)
            update(*i);
    }

    void Controller::update(const libtorrent::torrent_status & status) {

        Q_ASSERT(_torrents.contains(status.info_hash));

        // Get view model
        TorrentViewModel * model = _torrents[status.info_hash]->model();

        // Update status
        model->update(status);
    }

    void Controller::removeTorrent(const libtorrent::sha1_hash & info_hash) {

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

    void Controller::pauseTorrent(const libtorrent::sha1_hash & info_hash) {

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

    void Controller::startTorrent(const libtorrent::sha1_hash & info_hash) {

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

    bool Controller::addTorrent(const Torrent::Configuration & configuration) {
                                //, bool promptUserForTorrentPluginConfiguration) {

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

        /**
         * Setup signals and slots
         */

        // Connect view model signals to controller slots
        const TorrentViewModel * viewModel = torrent->model();

        QObject::connect(viewModel,
                         SIGNAL(pause(libtorrent::sha1_hash)),
                         this,
                         SLOT(pauseTorrent(libtorrent::sha1_hash)));

        QObject::connect(viewModel,
                         SIGNAL(start(libtorrent::sha1_hash)),
                         this,
                         SLOT(startTorrent(libtorrent::sha1_hash)));

        QObject::connect(viewModel,
                         SIGNAL(remove(libtorrent::sha1_hash)),
                         this,
                         SLOT(removeTorrent(libtorrent::sha1_hash)));

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

        // Indicate that we added to session
        return true;
    }

    bool Controller::addTorrent(const Torrent::Configuration & configuration, const SellerTorrentPlugin::Configuration & pluginConfiguration) {

        // Try to add torrent
        if(!addTorrent(configuration))
            return false;

        // Save plugin configuration
        _pendingSellerTorrentPluginConfigurations[configuration.infoHash()] = pluginConfiguration;

        return true;
    }

    bool Controller::addTorrent(const Torrent::Configuration & configuration, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo) {

        // Try to add torrent
        if(!addTorrent(configuration))
            return false;

        // Save plugin configuration and utxo
        _pendingBuyerTorrentPluginConfigurationAndUtxos[configuration.infoHash()] = QPair<BuyerTorrentPlugin::Configuration, Coin::UnspentP2PKHOutput>(pluginConfiguration, utxo);

        return true;
    }

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

    void Controller::startSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPlugin::Configuration & pluginConfiguration) {

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

    void Controller::startBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo) {

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


    void Controller::saveStateToFile(const char * file) {

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

    Wallet::Manager * Controller::wallet() {
        return _wallet;
    }

    const TorrentViewModel * Controller::torrentViewModel(const libtorrent::sha1_hash & infoHash) const {

        if(!_torrents.contains(infoHash))
            return NULL;
        else
            return _torrents[infoHash]->model();
    }

    void Controller::begin_close() {

        // Note that controller is being stopped,
        // this prevents any processing of Qt events
        // from libtorrent or view pending in event queue.
        //stoppingController = true;

        // Pause all torrents
        _session->pause();

        /**
         * DO NOT SAVE STATE TO DISK,
         * RATHER EMIT CONFIGURATION WITH CLOSING EVENT OR SOMETHING
         */
        // Save state of controller (includes full libtorrent state) to parameter file
        //QString file = QDir::current().absolutePath () + QDir::separator() + PARAMETER_FILE_NAME;
        //saveStateToFile(file.toStdString().c_str());

        // Save resume data for all
        int numberOutStanding = makeResumeDataCallsForAllTorrents();

        // If there are no outstanding, then just close right away
        if(numberOutStanding == 0)
            finalize_close();
        else {

            // Update controller state
            _state = State::waiting_for_resume_data_while_closing;

            qCDebug(_category) << "Attempting to generate resume data for " << numberOutStanding << " torrents.";
        }
    }

    /**
    libtorrent::torrent_handle Controller::getTorrentHandle(const libtorrent::sha1_hash & infoHash) const {

        if(_torrents.contains(infoHash))
            return _torrents[infoHash]->handle();
        else
            return libtorrent::torrent_handle();
    }
    */

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
            qCDebug(_category) << "Failed unregistering stream, torrent does not exist.";
    }

    void Controller::unRegisterStream(Stream * stream, Stream::Error error) {

        unRegisterStream(stream);

        qCDebug(_category) << "Stream unregistered due to some error.";

        //emit some sort of signal about error
    }

    void Controller::changeDownloadingLocationFromThisPiece(const libtorrent::sha1_hash & infoHash, int pieceIndex) {

        // Check that torrent exists
        if(!_torrents.contains(infoHash)) {

            qCDebug(_category) << "Changing download location requested for torrent which does not exist.";
            return;
        }

        // Check that
        if(_torrents[infoHash]->pluginInstalled() != PluginInstalled::Buyer) {

            qCDebug(_category) << "Changing download location requested for with plugin which does not have a buyer torrent plugin installed on torrent.";
            return;
        }

        // Ask torrent to relocate
        _plugin->submitTorrentPluginRequest(new ChangeDownloadLocation(infoHash, pieceIndex));
    }

    quint16 Controller::getServerPort() const {
       return _streamingServer.serverPort();
    }

    void Controller::finalize_close() {

        qCDebug(_category) << "finalize_close() run.";

        // Stop timer
        _statusUpdateTimer.stop();

        // Tell runner that controller is done
        emit closed();
    }

}
}
