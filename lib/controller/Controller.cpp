
#include "Controller.hpp"
#include "TorrentStatus.hpp"
#include "Config.hpp"
//#include "view/addtorrentdialog.hpp"
#include "controller/Exceptions/ListenOnException.hpp"
#include "controller/TorrentConfiguration.hpp"
#include "extension/Alert/TorrentPluginStatusAlert.hpp"
#include "extension/Alert/PluginStatusAlert.hpp"
//#include "extension/Request/SetConfigurationTorrentPluginRequest.hpp"
//#include "extension/Request/StartPluginTorrentPluginRequest.hpp"

#include <libtorrent/alert_types.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/peer_connection.hpp>

#include <QObject>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QtGlobal>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QThread>

#ifndef Q_MOC_RUN
#include <boost/bind.hpp>
#endif Q_MOC_RUN

// Register types for signal and slots: LATER CHECK WHICH ONE OF THESE ARE ACTUALLY REQUIRED
Q_DECLARE_METATYPE(libtorrent::sha1_hash)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(libtorrent::error_code)
Q_DECLARE_METATYPE(std::vector<libtorrent::torrent_status>)
Q_DECLARE_METATYPE(libtorrent::torrent_status)

// Register type for QMetaObject::invokeMethod
Q_DECLARE_METATYPE(const libtorrent::alert*)

Controller::Controller(const ControllerConfiguration & controllerConfiguration, bool showView, QNetworkAccessManager & manager, QString bitcoindAccount, QLoggingCategory & category)
    : _session(libtorrent::fingerprint(CLIENT_FINGERPRINT
                                      ,BITSWAPR_VERSION_MAJOR
                                      ,BITSWAPR_VERSION_MINOR
                                      ,0
                                      ,0)
              ,libtorrent::session::add_default_plugins
              ,libtorrent::alert::error_notification
              +libtorrent::alert::tracker_notification
              +libtorrent::alert::debug_notification
              +libtorrent::alert::status_notification
              +libtorrent::alert::progress_notification
              +libtorrent::alert::performance_warning
              +libtorrent::alert::stats_notification)
    , _sourceForLastResumeDataCall(NONE)
    , _category(category)
    , _manager(manager)
    , _plugin(new Plugin(this, _manager, bitcoindAccount, _category))
    , _portRange(controllerConfiguration.getPortRange())
    , _view(this, _category)
    , _numberOfOutstandingResumeDataCalls(0) {

    // Register types for signal and slots
    qRegisterMetaType<libtorrent::sha1_hash>();
    qRegisterMetaType<std::string>();
    qRegisterMetaType<libtorrent::error_code>();
    qRegisterMetaType<std::vector<libtorrent::torrent_status>>();
    qRegisterMetaType<libtorrent::torrent_status>();

    // Register type for QMetaObject::invokeMethod
    qRegisterMetaType<const libtorrent::alert*>();

    // Set libtorrent to call processAlert when alert is created
    _session.set_alert_dispatch(boost::bind(&Controller::libtorrent_alert_dispatcher_callback, this, _1));

	// Set session settings - these acrobatics with going back and forth seem to indicate that I may have done it incorrectly
	std::vector<char> buffer;
    libtorrent::bencode(std::back_inserter(buffer), controllerConfiguration.getLibtorrentSessionSettingsEntry());
	libtorrent::lazy_entry settingsLazyEntry;
    libtorrent::error_code lazyBdecodeEc;
	libtorrent::lazy_bdecode(&buffer[0], &buffer[0] + buffer.size(), settingsLazyEntry, lazyBdecodeEc);
    _session.load_state(settingsLazyEntry);

	// Add DHT routing nodes
    const std::vector<std::pair<std::string, int>> & dhtRouters = controllerConfiguration.getDhtRouters();

    for(std::vector<std::pair<std::string, int>>::const_iterator i = dhtRouters.begin(),
            end(dhtRouters.end());i != end; ++i)
        _session.add_dht_router(*i); // Add router to session

    // Add plugin extension
    boost::shared_ptr<libtorrent::plugin> plugin_shared_ptr(_plugin);
    _session.add_extension(plugin_shared_ptr);

    // Start DHT node
    _session.start_dht();

    // Start timer which calls session.post_torrent_updates at regular intervals
    _statusUpdateTimer.setInterval(POST_TORRENT_UPDATES_DELAY);

    QObject::connect(&_statusUpdateTimer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(callPostTorrentUpdates()));

    _statusUpdateTimer.start();

	// Start listening
	boost::system::error_code listenOnErrorCode;
    _session.listen_on(_portRange, listenOnErrorCode);

    // Throw
	if(listenOnErrorCode)
		throw ListenOnException(listenOnErrorCode);

    // Add all torrents, but this ust be AFTER session.listen_on(),
    // because otherwise adding to session won't work.
    for(std::vector<TorrentConfiguration *>::const_iterator i = controllerConfiguration.getBeginTorrentConfigurationsIterator(),
            end(controllerConfiguration.getEndTorrentConfigurationsIterator());i != end; ++i) {

        // Try to add torrent, without prompting user
        if(!addTorrent(*(*i), false)) {

            qCCritical(_category) << "Unable to add torrent configuration to session";
            return;
        }
    }

    // Show view
    if(showView) {
        _view.show();

        // Set port in title if we are logging
        if(QString(_category.categoryName()).compare("default"))
            _view.setWindowTitle(_view.windowTitle() + ", Port:" + QString::number(_session.listen_port()));
    }
}

void Controller::callPostTorrentUpdates() {
    _session.post_torrent_updates();
}

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

void Controller::libtorrent_alert_dispatcher_callback(std::auto_ptr<libtorrent::alert> alertAutoPtr) {

    /**
     * CRITICAL: Do not under any circumstance make a new call to libtorrent in this routine, since the network
     * thread in libtorrent will be making this call, and a new call will result in a dead lock.
     */

    // Grab alert pointer and release the auto pointer, this way the alert is not automatically
    // deleted when alertAutoPtr goes out of scope. Registering auto_ptr with MOC is not worth trying.
    const libtorrent::alert * a = alertAutoPtr.release();

    // Tell bitswapr thread to run processAlert later with given alert as argument
    QMetaObject::invokeMethod(this, "processAlert", Q_ARG(const libtorrent::alert*, a));
}

void Controller::processAlert(const libtorrent::alert * a) {

    // Check that alert has been stuck in event queue and corresponds to recenty
    // removed torrent.

    // if(something)
    //    something;

    //qCDebug(_category) << a->what();

    // In each case, tell bitswapr thread to run the given method
    if(libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a)) {
        //qCDebug(_category) << "Metadata received alert";
        processMetadataReceivedAlert(p);
    } else if(libtorrent::metadata_failed_alert const * p = libtorrent::alert_cast<libtorrent::metadata_failed_alert>(a)) {
        qCDebug(_category) << "Metadata failed alert";
        processMetadataFailedAlert(p);
    } else if(libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a)) {
        //qCDebug(_category) << "Add torrent alert";
        processAddTorrentAlert(p);
    } else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a)) {
        //qCDebug(_category) << "torrent_finished_alert";
        processTorrentFinishedAlert(p);
    } else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a)) {
        //qCDebug(_category) << "Torrent paused alert.";
        processTorrentPausedAlert(p);
    } else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a)) {
        //qCDebug(_category) << "State update alert.";
        processStatusUpdateAlert(p);
    } else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a)) {
        //qCDebug(_category) << "Torrent removed alert.";
        processTorrentRemovedAlert(p);
    } else if(libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a)) {
        //qCDebug(_category) << "Save resume data alert.";
        processSaveResumeDataAlert(p);
    } else if(libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a)) {
        //qCDebug(_category) << "Save resume data failed alert.";
        processSaveResumeDataFailedAlert(p);
    } else if(libtorrent::torrent_checked_alert const * p = libtorrent::alert_cast<libtorrent::torrent_checked_alert>(a)) {
        //qCDebug(_category) << "Torrent checked alert.";
        processTorrentCheckedAlert(p);
    } else if(TorrentPluginStatusAlert const * p = libtorrent::alert_cast<TorrentPluginStatusAlert>(a)) {
        //qCDebug(_category) << "Torrent plugin status alert.";
        processTorrentPluginStatusAlert(p);
    } else if(PluginStatusAlert const * p = libtorrent::alert_cast<PluginStatusAlert>(a)) {
        //qCDebug(_category) << "Plugin status alert.";
        processPluginStatusAlert(p);
    }

    // Delete alert
    delete a;
}

// NB!: Alreayd assumes session is paused and does not check sourceForLastResumeDataCall and numberOfOutstandingResumeDataCalls before starting,
// assumes they equal NONE,0 respectively.
int Controller::makeResumeDataCallsForAllTorrents() {

    // Get all handles (should be same torrents as int addTorrentParameters, but we dont check this here
    std::vector<libtorrent::torrent_handle> handles = _session.get_torrents();

    // Iterate all torrents, and try to save
    for (std::vector<libtorrent::torrent_handle>::iterator i = handles.begin(),
         end(handles.end());i != end; ++i) {

        // Get handle
        libtorrent::torrent_handle & h = *i;

        // Dont save data if we dont need to or can
        if (!h.is_valid() || !h.need_save_resume_data() || !h.status().has_metadata)
            continue;

        // Save resume data
        h.save_resume_data();

        // Count towards number of outstanding calls
        _numberOfOutstandingResumeDataCalls++;
    }

    return _numberOfOutstandingResumeDataCalls;
}

void Controller::processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p) {

    // Save resume data if we can!
    if(_sourceForLastResumeDataCall == NONE) {

        // Check that we have a valid state
        if(_numberOfOutstandingResumeDataCalls != 0) {

            qCCritical(_category) << "Invalid state, sourceForLastResumeDataCall == NONE && numberOfOutstandingResumeDataCalls != 0, can't save resume data.";
            return;
        }

        // Get handle
        libtorrent::torrent_handle torrentHandle = p->handle;

        // Dont save data if we dont need to or can
        if (!torrentHandle.need_save_resume_data() || !torrentHandle.status().has_metadata)
            return;

        // Set state
        _sourceForLastResumeDataCall = TORRENT_PAUSE;
        _numberOfOutstandingResumeDataCalls = 1;

        // Save resume data
        torrentHandle.save_resume_data();
    } else
        qCWarning(_category) << "Could not save resume data, so we won't.";
}

void Controller::processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p) {

    /*
     * NOTICE: Docs say p->handle may be invalid at this time,
     * so we must use p->info_hash instead.
     */

    // Get torrent info hash
    libtorrent::sha1_hash info_hash = p->info_hash;

    // Remove from view
    _view.removeTorrent(info_hash);

    qCDebug(_category) << "Found match and removed it.";
}

void Controller::processMetadataReceivedAlert(libtorrent::metadata_received_alert const * p) {

    // Get handle for torrent
    libtorrent::torrent_handle h = p->handle;

    // Process if handle is valid
    if (h.is_valid()) {

        // get torrent info
        const libtorrent::torrent_info & torrentInfo = h.get_torrent_info();

        // USE THIS INFORMATION FOR SOMETHING LATER

    } else
        qCDebug(_category) << "Invalid handle for received metadata.";
}

void Controller::processMetadataFailedAlert(libtorrent::metadata_failed_alert const * p) {
    // WHAT DO WE DO HERE?
}

void Controller::processAddTorrentAlert(libtorrent::add_torrent_alert const * p) {

    // Name of torrent
    std::string name("N/A");
    int totalSize = 0;
    if(p->params.ti.get() != 0) {
        name = p->params.ti->name();
        totalSize = (p->params.ti)->total_size();
    } else
        name = p->params.name;

    // Check if there was an error
    if (p->error) {
        _view.addTorrentFailed(name, p->params.info_hash, p->error);
    } else {

        /*
		h.set_max_connections(max_connections_per_torrent);
		h.set_max_uploads(-1);
		h.set_upload_limit(torrent_upload_limit);
		h.set_download_limit(torrent_download_limit);
		h.use_interface(outgoing_interface.c_str());
        */

        // Add torrent to view
        _view.addTorrent(p->handle.info_hash(), QString(name.c_str()), totalSize);
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

    //qCCritical(_category) << "Number of state_update_alert alerts" << p->status.size();
    _view.updateTorrentStatus(p->status);
}

void Controller::processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p) {

    // Check that state of controller is compatible with the arrival of this alert
    if(_sourceForLastResumeDataCall == NONE || _numberOfOutstandingResumeDataCalls == 0) {
        qCCritical(_category) << "Received resume data alert, despite no outstanding calls, hence droping alert, but this is a bug.";
        return;
    }

    // Decrease outstanding count
    _numberOfOutstandingResumeDataCalls--;

    // Get info hash for torrent
    libtorrent::sha1_hash info_hash = p->handle.info_hash();

    // Find reference to resume data for torrent
    std::map<libtorrent::sha1_hash, TorrentStatus *>::iterator i = _torrentStatuses.find(info_hash);

    if(i == _torrentStatuses.end()) {

        qCCritical(_category) << "Resume data received for missing torrent.";
        return;
    }

    std::vector<char> & resumeData = (i->second)->_resumeData;

    // Dump old content
    resumeData.clear();

    // Write new content to it
    bencode(std::back_inserter(resumeData), *(p->resume_data));

    // If this was last outstanding resume data save, the   n do relevant callback
    if(_numberOfOutstandingResumeDataCalls == 0) {

        switch(_sourceForLastResumeDataCall) {

            case CLIENT_PAUSE:

                break;
            case TORRENT_PAUSE:

                break;
            case CLIENT_CLOSE:

                // Close application
                finalize_close();

                break;
            default:
                qCCritical(_category) << "Invalid value of sourceForLastResumeDataCall.";
        }

        // Reset state
        _sourceForLastResumeDataCall = NONE;
    }
}

void Controller::processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p) {

    // Check that state of controller is compatible with the arrival of this alert
    if(_sourceForLastResumeDataCall == NONE || _numberOfOutstandingResumeDataCalls == 0) {

        qCCritical(_category) << "Received resume data alert, despite no outstanding calls, hence droping alert, but this is a bug.";
        return;
    }

    qCWarning(_category) << "Received resume data failed alert, something went wrong.";

    // Decrease outstanding count
    _numberOfOutstandingResumeDataCalls--;
}

void Controller::processTorrentCheckedAlert(libtorrent::torrent_checked_alert const * p) {

    // Get handle for torrent
    libtorrent::torrent_handle h = p->handle;

    // Process if handle is valid
    if (h.is_valid()) {

        // Get info hash of torrent
        libtorrent::sha1_hash infoHash = h.info_hash();

        // Get torrent status
        std::map<libtorrent::sha1_hash, TorrentStatus *>::iterator i = _torrentStatuses.find(infoHash);

        // if no status is registered, then we exit
        if(i == _torrentStatuses.end()) {

            qCCritical(_category) << "Torrent checked alert issued for torrent without status.";
            return;
        }

        // otherwise grab status
        TorrentStatus * torrentStatus = i->second;

        // and if torrent configuration plugin is needed, then tell view
        if(torrentStatus->_needsTorrentPluginConfigurationAfterTorrentCheckedAlert) {

            // get torrent information
            libtorrent::torrent_info torrentInfo = h.get_torrent_info();

            // get torrent status
            libtorrent::torrent_status torrentStatus = h.status();

            // and tell view to prompt user based on this
            _view.showAddTorrentPluginConfigurationDialog(torrentInfo, torrentStatus);

        } else // otherwise just tell torrent plugin to begin with existing torrent plugin configuratio, hence we pass NULL
            updateTorrentPluginConfiguration(infoHash, NULL);

    } else
        qCDebug(_category) << "Invalid handle for checked torrent.";
}

void Controller::processTorrentPluginStatusAlert(const TorrentPluginStatusAlert * p) {
    _view.updateTorrentPluginStatus(p);
}

void Controller::processPluginStatusAlert(const PluginStatusAlert * p) {
    _view.updatePluginStatus(p);
}

bool Controller::removeTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle & torrentHandle = _session.find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        return false;

    // Remove from session
    // Session will send us torrent_removed_alert alert when torrent has been removed
    // at which point we can remove torrent from model in alert handler
    _session.remove_torrent(torrentHandle);

    // It worked
    return true;
}

bool Controller::pauseTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle & torrentHandle = _session.find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        return false;

    // Turn off auto managing
    torrentHandle.auto_managed(false);

    // Pause
    // We save resume data when the pause alert is issued by libtorrent
    torrentHandle.pause(libtorrent::torrent_handle::graceful_pause);

    // It worked
    return true;
}

bool Controller::startTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle & torrentHandle = _session.find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        return false;

    // Turn on auto managing
    torrentHandle.auto_managed(true);

    // Start
    torrentHandle.resume();

    // It worked
    return true;
}

void Controller::updateTorrentPluginConfiguration(const libtorrent::sha1_hash & infoHash, TorrentPluginConfiguration * torrentPluginConfiguration) {
    //_plugin->submitTorrentPluginRequest(new StartPluginTorrentPluginRequest(infoHash, torrentPluginConfiguration));
    qCCritical(_category) << "updating torretn plugin configuration disabled!";
}

bool Controller::addTorrent(const TorrentConfiguration & torrentConfiguration, bool promptUserForTorrentPluginConfiguration) {

    // Convert to add torrent parameters
    libtorrent::add_torrent_params params = torrentConfiguration.toAddTorrentParams();

    // Create save_path on disk, return if it does not exist
    if(!(QDir()).mkpath(params.save_path.c_str())) {

        qCCritical(_category) << "Could not create save_path: " << params.save_path.c_str();
        return false;
    }

    // Create initial torrent status
    TorrentStatus * s = new TorrentStatus(*params.resume_data, promptUserForTorrentPluginConfiguration);

    // Save status in map
    _torrentStatuses.insert(std::make_pair(params.info_hash, s));

    // Add torrent to session
    _session.async_add_torrent(params);

    // Delete std::vector<char> memory allocated in TorrentConfiguration::toAddTorrentParams
    // Do something differet later, e.g. have params point to same resume_data as in torrentConfiguration?
    delete params.resume_data;

    // Indicate that we added to session
    return true;
}

void Controller::saveStateToFile(const char * file) {

    // NOT DONE!

    /**

	// Save present state of session in entry
	libtorrent::entry libtorrentSessionSettingsEntry;
    _session.save_state(libtorrentSessionSettingsEntry);

    // Torrent configurations
    std::vector<TorrentConfiguration *> torrentConfigurations;

    // Grab torrent handles
    std::vector<libtorrent::torrent_handle> torrents = _session.get_torrents();

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

void Controller::begin_close() {

    // Check if we can actually save resume data at this time
    if(_sourceForLastResumeDataCall != NONE) {
        qCCritical(_category) << "ERROR: Cannot save resume data at this time due to outstanding resume_data calls, try again later.";
        return;
    } else  if(_numberOfOutstandingResumeDataCalls != 0) {
        qCCritical(_category) << "ERROR: Invalid state found, numberOfOutstandingResumeDataCalls != 0, despite sourceForLastResumeDataCall == NONE. Resume data cannot be saved.";
        return;
    }

    // Note that controller is being stopped,
    // this prevents any processing of Qt events
    // from libtorrent or view pending in event queue.
    //stoppingController = true;

    // Pause all torrents
    _session.pause();

    // Save state of controller (includes full libtorrent state) to parameter file
    QString file = QDir::current().absolutePath () + QDir::separator() + PARAMETER_FILE_NAME;
    saveStateToFile(file.toStdString().c_str());

    // Save resume data for all
    _sourceForLastResumeDataCall = CLIENT_CLOSE; // Note source of a new set of resume data calls
    int numberOutStanding = makeResumeDataCallsForAllTorrents();

    // If there are no outstanding, then just close right away
    if(numberOutStanding == 0)
        finalize_close();
    else
        qCDebug(_category) << "Attempting to generate resume data for " << numberOutStanding << " torrents.";
}

void Controller::finalize_close() {

    qCDebug(_category) << "finalize_close() run.";

    // Stop timer
    _statusUpdateTimer.stop();

    // Tell runner that controller is done
    emit closed();
}
