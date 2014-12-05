
#include "Controller.hpp"
#include "Config.hpp"
#include "view/addtorrentdialog.hpp"
#include "controller/Exceptions/ListenOnException.hpp"

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

Controller::Controller(const PersistentControllerState & persistentControllerState, bool showView, QLoggingCategory & category)
    : session(libtorrent::fingerprint(CLIENT_FINGERPRINT
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
    , numberOfOutstandingResumeDataCalls(0)
    , sourceForLastResumeDataCall(NONE)
    , persistentControllerState_(persistentControllerState)
    , category_(category)
    , plugin(new Plugin(this, category_))
    , view(this, category_) {

    // Register types for signal and slots
    qRegisterMetaType<libtorrent::sha1_hash>();
    qRegisterMetaType<std::string>();
    qRegisterMetaType<libtorrent::error_code>();
    qRegisterMetaType<std::vector<libtorrent::torrent_status>>();
    qRegisterMetaType<libtorrent::torrent_status>();

    // Register type for QMetaObject::invokeMethod
    qRegisterMetaType<const libtorrent::alert*>();

    // Set libtorrent to call processAlert when alert is created
    session.set_alert_dispatch(boost::bind(&Controller::libtorrent_alert_dispatcher_callback, this, _1));

	// Set session settings - these acrobatics with going back and forth seem to indicate that I may have done it incorrectly
	std::vector<char> buffer;
    libtorrent::bencode(std::back_inserter(buffer), persistentControllerState_.getLibtorrentSessionSettingsEntry());
	libtorrent::lazy_entry settingsLazyEntry;
    libtorrent::error_code lazyBdecodeEc;
	libtorrent::lazy_bdecode(&buffer[0], &buffer[0] + buffer.size(), settingsLazyEntry, lazyBdecodeEc);
	session.load_state(settingsLazyEntry);

	// Add DHT routing nodes
    const std::vector<std::pair<std::string, int>> & dhtRouters = persistentControllerState_.getDhtRouters();

    for(std::vector<std::pair<std::string, int>>::const_iterator i = dhtRouters.begin(),
            end(dhtRouters.end());i != end; ++i)
        session.add_dht_router(*i); // Add router to session

    // Add plugin extension
    session.add_extension(plugin);

    // Start DHT node
    session.start_dht();

    // Start timer which calls session.post_torrent_updates at regular intervals
    statusUpdateTimer.setInterval(POST_TORRENT_UPDATES_DELAY);

    QObject::connect(&statusUpdateTimer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(callPostTorrentUpdates()));

    statusUpdateTimer.start();

	// Start listening
	boost::system::error_code listenOnErrorCode;
    session.listen_on(persistentControllerState_.getPortRange(), listenOnErrorCode);

    // Throw
	if(listenOnErrorCode)
		throw ListenOnException(listenOnErrorCode);

    // Add torrents to session and to controller: Wisdom of Sindre indicates this must be AFTER session.listen_on()
    std::map<libtorrent::sha1_hash, PersistentTorrentState> & models = persistentControllerState_.getPersistentTorrentStates();

    for(std::map<libtorrent::sha1_hash, PersistentTorrentState>::iterator i = models.begin(),
            end(models.end());i != end; ++i) {

        // Get state of torrent
        PersistentTorrentState & persistentState = i->second;

        // Create add_torrent_params for adding
        libtorrent::add_torrent_params params;

        params.info_hash = persistentState.getInfoHash();
        params.name = persistentState.getName();
        params.save_path = persistentState.getSavePath();
        params.resume_data = &(persistentState.getResumeData()); // Pointer to the original std::vector which persists
        params.flags = persistentState.getFlags();

        // Add torrent
        addTorrent(params);
    }

    // Show view
    if(showView) {
        view.show();

        // Set port in title if we are logging
        if(QString(category_.categoryName()).compare("default"))
            view.setWindowTitle(view.windowTitle() + ", Port:" + QString::number(session.listen_port()));
    }
}

void Controller::callPostTorrentUpdates() {
    session.post_torrent_updates();
}

void Controller::extensionPeerAdded(PeerPlugin * peerPlugin) {
    view.addPeerPlugin(peerPlugin);
}

void Controller::updateTorrentPluginStatus(TorrentPluginStatus status) {
    view.updateTorrentPluginStatus(status);
}

void Controller::updatePeerPluginStatus(PeerPluginStatus status) {
    view.updatePeerPluginStatus(status);
}

void Controller::libtorrent_alert_dispatcher_callback(std::auto_ptr<libtorrent::alert> alertAutoPtr) {

    /*
     * CRITICAL: Do not under any circumstance make a new call to libtorrent in this routine here, since the network
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


    // In each case, tell bitswapr thread to run the given method
	if (libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a))
	{
        //qCDebug(category_) << "metadata_received_alert";

		/*
		// if we have a monitor dir, save the .torrent file we just received in it
		// also, add it to the files map, and remove it from the non_files list
		// to keep the scan dir logic in sync so it's not removed, or added twice
		torrent_handle h = p->handle;
		if (h.is_valid()) {
			if (!ti) ti = h.torrent_file();
			create_torrent ct(*ti);
			entry te = ct.generate();
			std::vector<char> buffer;
			bencode(std::back_inserter(buffer), te);
			std::string filename = ti->name() + "." + to_hex(ti->info_hash().to_string()) + ".torrent";
			filename = combine_path(monitor_dir, filename);
			save_file(filename, buffer);

			files.insert(std::pair<std::string, libtorrent::torrent_handle>(filename, h));
			non_files.erase(h);
		}
		*/
	}
    else if (libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a)) {
        //qCDebug(category_) << "Add torrent alert.";
        processAddTorrentAlert(p);
    } else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a)) {
        //qCDebug(category_) << "torrent_finished_alert";

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
    else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a)) {
        //qCDebug(category_) << "Torrent paused alert.";
        processTorrentPausedAlert(p);
    } else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a)) {
        //qCDebug(category_) << "State update alert.";
        processStatusUpdateAlert(p);
    } else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a)) {
        //qCDebug(category_) << "Torrent removed alert.";
        processTorrentRemovedAlert(p);
    } else if(libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a)) {
        //qCDebug(category_) << "Save resume data alert.";
        processSaveResumeDataAlert(p);
    } else if(libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a)) {
        //qCDebug(category_) << "Save resume data failed alert.";
        processSaveResumeDataFailedAlert(p);
    }

    // Delete alert
    delete a;
}

// NB!: Alreayd assumes session is paused and does not check sourceForLastResumeDataCall and numberOfOutstandingResumeDataCalls before starting,
// assumes they equal NONE,0 respectively.
int Controller::makeResumeDataCallsForAllTorrents() {

    // Get all handles (should be same torrents as int addTorrentParameters, but we dont check this here
    std::vector<libtorrent::torrent_handle> handles = session.get_torrents();

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
        numberOfOutstandingResumeDataCalls++;
    }

    return numberOfOutstandingResumeDataCalls;
}

void Controller::processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p) {

    // Save resume data if we can!
    if(sourceForLastResumeDataCall == NONE) {

        // Check that we have a valid state
        if(numberOfOutstandingResumeDataCalls != 0) {

            qCCritical(category_) << "Invalid state, sourceForLastResumeDataCall == NONE && numberOfOutstandingResumeDataCalls != 0, can't save resume data.";
            return;
        }

        // Get handle
        libtorrent::torrent_handle torrentHandle = p->handle;

        // Dont save data if we dont need to or can
        if (!torrentHandle.need_save_resume_data() || !torrentHandle.status().has_metadata)
            return;

        // Set state
        sourceForLastResumeDataCall = TORRENT_PAUSE;
        numberOfOutstandingResumeDataCalls = 1;

        // Save resume data
        torrentHandle.save_resume_data();
    } else
        qCWarning(category_) << "Could not save resume data, so we won't.";
}

void Controller::processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p) {

    /*
     * NOTICE: Docs say p->handle may be invalid at this time,
     * so we must use p->info_hash instead.
     */

    std::map<libtorrent::sha1_hash, PersistentTorrentState> & torrentStates = persistentControllerState_.getPersistentTorrentStates();
    std::map<libtorrent::sha1_hash, PersistentTorrentState>::iterator & mapIterator = torrentStates.find(p->info_hash);

    // Did we find match
    if(mapIterator == torrentStates.end()) {
        qCCritical(category_) << "No matching info hash found.";
    }

    // Erase from map, which also calls destructor
    torrentStates.erase(mapIterator);

    // Remove from view
    view.removeTorrent(p->info_hash);

    qCDebug(category_) << "Found match and removed it.";
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
        view.addTorrentFailed(name, p->params.info_hash, p->error);
    } else {

        /*
		h.set_max_connections(max_connections_per_torrent);
		h.set_max_uploads(-1);
		h.set_upload_limit(torrent_upload_limit);
		h.set_download_limit(torrent_download_limit);
		h.use_interface(outgoing_interface.c_str());
        */

        // Add torrent to view
        view.addTorrent(p->handle.info_hash(), QString(name.c_str()), totalSize);
	}
}

void Controller::processStatusUpdateAlert(libtorrent::state_update_alert const * p) {
    view.updateTorrentStatus(p->status);
}

void Controller::processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p) {

    // Check that state of controller is compatible with the arrival of this alert
    if(sourceForLastResumeDataCall == NONE || numberOfOutstandingResumeDataCalls == 0) {
        qCCritical(category_) << "Received resume data alert, despite no outstanding calls, hence droping alert, but this is a bug.";
        return;
    }

    // Decrease outstanding count
    numberOfOutstandingResumeDataCalls--;

    // Get persistent torrent state to save resume data
    std::map<libtorrent::sha1_hash, PersistentTorrentState> & torrentStates = persistentControllerState_.getPersistentTorrentStates();
    std::map<libtorrent::sha1_hash, PersistentTorrentState>::iterator & mapIterator = torrentStates.find(p->handle.info_hash());

    // Did we find match
    if(mapIterator == torrentStates.end()) {
        qCCritical(category_) << "No matching info hash found.";
        return;
    }

    PersistentTorrentState & torrentState = mapIterator->second;

    // Get reference to resume data vector
    std::vector<char> & resume_data = torrentState.getResumeData();

    // Dump old content
    resume_data.clear();

    // Write new content to it
    bencode(std::back_inserter(resume_data), *(p->resume_data));

    // If this was last outstanding resume data save, then do relevant callback
    if(numberOfOutstandingResumeDataCalls == 0) {

        switch(sourceForLastResumeDataCall) {

            case CLIENT_PAUSE:

                break;
            case TORRENT_PAUSE:

                break;
            case CLIENT_CLOSE:

                // Close application
                finalize_close();

                break;
            default:
                qCCritical(category_) << "Invalid value of sourceForLastResumeDataCall.";
        }

        // Reset state
        sourceForLastResumeDataCall = NONE;
    }
}

void Controller::processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p) {

    // Check that state of controller is compatible with the arrival of this alert
    if(sourceForLastResumeDataCall == NONE || numberOfOutstandingResumeDataCalls == 0) {

        qCCritical(category_) << "Received resume data alert, despite no outstanding calls, hence droping alert, but this is a bug.";
        return;
    }

    qCWarning(category_) << "Received resume data failed alert, something went wrong.";

    // Decrease outstanding count
    numberOfOutstandingResumeDataCalls--;
}

void Controller::addTorrentFromTorrentFile(const QString & torrentFile) {

    // Check that torrent file exists
    if(!QFile::exists(torrentFile)) {
        qCCritical(category_) << "Torrent file " << torrentFile.toStdString().c_str() << " does not exist.";
        return;
    }

    // Setup parameters
    libtorrent::add_torrent_params params;

    // Load torrent file
    libtorrent::error_code ec;
    boost::intrusive_ptr<libtorrent::torrent_info> torrentInfoPointer = new libtorrent::torrent_info(torrentFile.toStdString().c_str(), ec);
    if(ec) {
        qCCritical(category_) << "Invalid torrent file: " << ec.message().c_str();
        return;
    }

    // Set torrent info in parameters
    params.ti = torrentInfoPointer;

    // Show window for adding torrent
    AddTorrentDialog * addTorrentDialog = new AddTorrentDialog(this, params);

    // Starts new event loop,
    // no more libtorrent alerts are processed in mean time,
    // change at a later time
    addTorrentDialog->exec();

    // Delete window
    delete addTorrentDialog;
}

void Controller::addTorrentFromMagnetLink(const QString & magnetLink) {

    // Setup parameters
    libtorrent::add_torrent_params params;
    params.url = magnetLink.toStdString();

    // Parse link to get info_hash, so that resume data can be loaded
    libtorrent::error_code ec;
    libtorrent::parse_magnet_uri(magnetLink.toStdString(), params, ec);

    // Exit if link is malformed
    if(ec) {
        qCWarning(category_) << "Malformed magnet link: " << ec.message().c_str();
        return;
    }

    // Show window for adding torrent
    AddTorrentDialog * addTorrentDialog = new AddTorrentDialog(this, params);
    addTorrentDialog->exec();

    // Delete window resources
    delete addTorrentDialog;
}

bool Controller::removeTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle & torrentHandle = session.find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        return false;

    // Remove from session
    session.remove_torrent(torrentHandle);

    // It worked
    return true;
}

bool Controller::pauseTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle & torrentHandle = session.find_torrent(info_hash);

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
    libtorrent::torrent_handle & torrentHandle = session.find_torrent(info_hash);

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

void Controller::addTorrent(libtorrent::add_torrent_params & params) {

    /*
    * If info_hash is not set, we try and set it.
    * This would typically be the case if torrent was added through torrent
    * file rather than magnet link. The primary reason for this constraint is because searching
    * addTorrentParameters is based on info_hashes
    */
    if(params.info_hash.is_all_zeros()) {

        // Is torrent info set, use it
        if(params.ti.get() != 0 && !params.ti->info_hash().is_all_zeros()) {
            libtorrent::sha1_hash info_hash = params.ti->info_hash();
            params.info_hash = info_hash;
        } else {
            // Throw exception in future
            qCDebug(category_) << "no valid info_hash set.";
            return;
        }
    }

    // Create save_path on disk if it does not exist
    if(!(QDir()).mkpath(params.save_path.c_str())) {

        qCCritical(category_) << "Could not create save_path: " << params.save_path.c_str();
        return;
    }

    // Add to session
    session.async_add_torrent(params);
}

void Controller::saveStateToFile(const char * file) {

	// Save present state of session in entry
	libtorrent::entry libtorrentSessionSettingsEntry;
	session.save_state(libtorrentSessionSettingsEntry);

    persistentControllerState_.setLibtorrentSessionSettingsEntry(libtorrentSessionSettingsEntry);

	// Save to file
    persistentControllerState_.saveToFile(file);
}

void Controller::begin_close() {

    // Check if we can actually save resume data at this time
    if(sourceForLastResumeDataCall != NONE) {
        qCCritical(category_) << "ERROR: Cannot save resume data at this time due to outstanding resume_data calls, try again later.";
        return;
    } else  if(numberOfOutstandingResumeDataCalls != 0) {
        qCCritical(category_) << "ERROR: Invalid state found, numberOfOutstandingResumeDataCalls != 0, despite sourceForLastResumeDataCall == NONE. Resume data cannot be saved.";
        return;
    }

    // Note that controller is being stopped,
    // this prevents any processing of Qt events
    // from libtorrent or view pending in event queue.
    //stoppingController = true;

    // Pause all torrents
    session.pause();

    // Save state of controller (includes full libtorrent state) to parameter file
    QString file = QDir::current().absolutePath () + QDir::separator() + PARAMETER_FILE_NAME;
    this->saveStateToFile(file.toStdString().c_str());

    // Save resume data for all
    sourceForLastResumeDataCall = CLIENT_CLOSE; // Note source of a new set of resume data calls
    int numberOutStanding = makeResumeDataCallsForAllTorrents();

    // If there are no outstanding, then just close right away
    if(numberOutStanding == 0)
        finalize_close();
    else
        qCDebug(category_) << "Attempting to generate resume data for " << numberOutStanding << " torrents.";
}

void Controller::finalize_close() {

    qCDebug(category_) << "finalize_close() run.";

    // Stop timer
    statusUpdateTimer.stop();

    /*
    // Immediately stop event loop for controller
    // ==========================================
    // NB: It is very important that exit() is used,
    // because this ensures that not a single additional
    // event is processed by event loop, which will not
    // work since controller has been deleted.
    QThread::currentThread()->exit();
    */

    // Tell runner that controller is done
    emit closed();
}
