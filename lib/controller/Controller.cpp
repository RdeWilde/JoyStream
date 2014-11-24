
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

Controller::Controller(const ControllerState & state, bool showView, QLoggingCategory * category)
    : session(libtorrent::fingerprint("BR"
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
    , view(this)
    , numberOfOutstandingResumeDataCalls(0)
    , sourceForLastResumeDataCall(NONE)
    , portRange(state.getPortRange())
    , dhtRouters(state.getDhtRouters())
    , category_(category == 0 ? QLoggingCategory::defaultCategory() : category)
    , plugin(new BitSwaprPlugin(this, category_)) {

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
	libtorrent::bencode(std::back_inserter(buffer), state.getLibtorrentSessionSettingsEntry());
	libtorrent::lazy_entry settingsLazyEntry;
    libtorrent::error_code lazyBdecodeEc;
	libtorrent::lazy_bdecode(&buffer[0], &buffer[0] + buffer.size(), settingsLazyEntry, lazyBdecodeEc);
	session.load_state(settingsLazyEntry);

	// Add DHT routing nodes
    for(std::vector<std::pair<std::string, int>>::iterator i = dhtRouters.begin();i != dhtRouters.end(); ++i)
        session.add_dht_router(*i); // Add router to session

    // Add plugin extension
    session.add_extension(plugin);

	// Start DHT node
    //if(state.useDht)
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
    session.listen_on(portRange, listenOnErrorCode);

    // Throw
	if(listenOnErrorCode)
		throw ListenOnException(listenOnErrorCode);

    // Add torrents to session and to controller: Wisdom of Sindre indicates this must be AFTER session.listen_on()
    std::vector<libtorrent::add_torrent_params> & params = state.getTorrentParameters();
    for(std::vector<libtorrent::add_torrent_params>::iterator i = params.begin();i != params.end(); ++i)
        addTorrent(*i);

    // Show view
    if(showView)
        view.show();
}

/*
void Controller::connectToView(MainWindow * view) {

    // connect: view -> controller
    QObject::connect(view, SIGNAL(addTorrentFromMagnetLink(const QString &)),
                     this, SLOT(addTorrentFromMagnetLink(const QString &)));


}
*/

void Controller::callPostTorrentUpdates() {
    session.post_torrent_updates();
}

void Controller::extensionPeerAdded(libtorrent::peer_connection * peerConnection) {

    qCDebug(CATEGORY) << "extende peer added!! \n";
}

void Controller::libtorrent_alert_dispatcher_callback(std::auto_ptr<libtorrent::alert> alertAutoPtr) {

    /*
     * CRITICAL: Do not under any circumstance make a new call to libtorrent in this routine here, since the network
     * thread in libtorrent will be making this call, and a new call will result in a dead lock.
     */

    // Grab alert pointer and release the auto pointer, this way the alert is not automatically
    // deleted when alertAutoPtr goes out of scope.
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
        //qCDebug(CATEGORY) << "metadata_received_alert";

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
        //qCDebug(CATEGORY) << "Add torrent alert.";
        processAddTorrentAlert(p);
    } else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a)) {
        //qCDebug(CATEGORY) << "torrent_finished_alert";

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
        //qCDebug(CATEGORY) << "Torrent paused alert.";
        processTorrentPausedAlert(p);
    } else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a)) {
        //qCDebug(CATEGORY) << "State update alert.";
        processStatusUpdateAlert(p);
    } else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a)) {
        //qCDebug(CATEGORY) << "Torrent removed alert.";
        processTorrentRemovedAlert(p);
    } else if(libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a)) {
        //qCDebug(CATEGORY) << "Save resume data alert.";
        processSaveResumeDataAlert(p);
    } else if(libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a)) {
        //qCDebug(CATEGORY) << "Save resume data failed alert.";
        processSaveResumeDataFailedAlert(p);
    }

    // Delete alert
    delete a;
}

// REMOVE AUTO POINTER, PASS ITERATOR ADDRESS OR REF INSTED
std::auto_ptr<std::vector<libtorrent::add_torrent_params>::iterator> Controller::findTorrentParamsFromInfoHash(const libtorrent::sha1_hash & info_hash) {

    // Allocate iterator copy on heap
    std::vector<libtorrent::add_torrent_params>::iterator * i_heap = new std::vector<libtorrent::add_torrent_params>::iterator();

    // Declare auto pointer pointing to iterator on heap
    std::auto_ptr<std::vector<libtorrent::add_torrent_params>::iterator> a_ptr(i_heap);

    // Check if vector is empty, in which case we cannot dereference .begin() iterator
    if(!addTorrentParameters.empty()) {

        // Iterate, using iterator on heap, to find match
        *i_heap = addTorrentParameters.begin();
        std::vector<libtorrent::add_torrent_params>::iterator end = addTorrentParameters.end();

        for(; *i_heap != end;(*i_heap)++) {

            libtorrent::add_torrent_params & params = *(*i_heap);

            // Check for match, and return
            if(params.info_hash == info_hash)
                return a_ptr;
        }
    }

    // Free heap iterator and set auto pointer to 0
    a_ptr.reset();

    // Return null auto_ptr
    return a_ptr;
}

bool Controller::loadResumeDataForTorrent(QString const & save_path, QString const & file_name, std::vector<char> & resume_data) const {

    // Check that file exists
    QString resumeFile = save_path + QDir::separator() + file_name;
    QFile file(resumeFile);

    if(file.exists()) {

        // Open file
        if(!file.open(QIODevice::ReadOnly)) {
            qCWarning(CATEGORY) << "Could not open : " << resumeFile.toStdString().c_str();
            return false;
        }

        // Read entire file
        QByteArray fullFile = file.readAll();

        // Close file
        file.close();

        // Populate resume_data vector
        for(QByteArray::iterator i = fullFile.begin(), end(fullFile.end()); i != end; i++)
            resume_data.push_back(*i);

        return true;
    } else
        return false;
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

bool Controller::saveResumeDataForTorrent(QString const & save_path, QString const & file_name, std::vector<char> const & resume_data) const {

    // Check that save_path still exists, if not create it,
    // because it could have been deleted by user since addTorrent()
    // If you cant create it, then return false
    if(!(QDir().exists(save_path) || QDir().mkpath(save_path))) {

        qCWarning(CATEGORY) << "Could not create save_path: " << save_path.toStdString().c_str();
        return false;
    }

    // Open file
    QString resumeFile = save_path + QDir::separator() + file_name;
    QFile file(resumeFile);

    if(!file.open(QIODevice::WriteOnly)) {

        qCCritical(CATEGORY) << "Could not open : " << resumeFile.toStdString().c_str();
        return false;
    }

    // Write to file
    file.write(&(resume_data[0]), resume_data.size());

    // Close file
    file.close();

    return true;
}

QString Controller::resumeFileNameForTorrent(libtorrent::sha1_hash & info_hash) const {
    return QString((libtorrent::to_hex(info_hash.to_string()) + ".resume").c_str());
}

void Controller::processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p) {

    // Save resume data if we can!
    if(sourceForLastResumeDataCall == NONE) {

        // Check that we have a valid state
        if(numberOfOutstandingResumeDataCalls != 0) {

            qCCritical(CATEGORY) << "Invalid state, sourceForLastResumeDataCall == NONE && numberOfOutstandingResumeDataCalls != 0, can't save resume data.";
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
        qCWarning(CATEGORY) << "Could not save resume data, so we won't.";
}

void Controller::processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p) {

    /*
     * NOTICE: Docs say p->handle may be invalid at this time,
     * so we must use p->info_hash instead.
     */

    // Find torrent params in addTorrentParameters
    std::auto_ptr<std::vector<libtorrent::add_torrent_params>::iterator> a_ptr = findTorrentParamsFromInfoHash(p->info_hash);

    // Did we find a match
    if(a_ptr.get() != 0) {

        // Remove torrent
        addTorrentParameters.erase(*a_ptr.get());

        // Notify view to remove torrent
        view.removeTorrent(p->info_hash);

        qCDebug(CATEGORY) << "Found match and removed it.";

    } else
        qCCritical(CATEGORY) << "We found no matching torrent for this.";
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
        view.addTorrent(p->handle.info_hash(), name, totalSize);
	}
}

void Controller::processStatusUpdateAlert(libtorrent::state_update_alert const * p) {
    view.updateTorrentStatus(p->status);
}

void Controller::processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p) {

    // Check that state of controller is compatible with the arrival of this alert
    if(sourceForLastResumeDataCall == NONE || numberOfOutstandingResumeDataCalls == 0) {

        qCCritical(CATEGORY) << "Received resume data alert, despite no outstanding calls, hence droping alert, but this is a bug.";
        return;
    }

    // Decrease outstanding count
    numberOfOutstandingResumeDataCalls--;

    // Get torrent params to get save_path
    std::auto_ptr<std::vector<libtorrent::add_torrent_params>::iterator> ptr = findTorrentParamsFromInfoHash(p->handle.info_hash());
    std::vector<libtorrent::add_torrent_params>::iterator & ptr_iterator = *ptr;
    libtorrent::add_torrent_params & params = *ptr_iterator;
    QString save_path = params.save_path.c_str();

    // Save resume data for torrent to disk
    std::vector<char> resume_data;
    bencode(std::back_inserter(resume_data), *(p->resume_data));
    saveResumeDataForTorrent(save_path, resumeFileNameForTorrent(p->handle.info_hash()), resume_data);

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
                qCCritical(CATEGORY) << "Invalid value of sourceForLastResumeDataCall.";
        }

        // Reset state
        sourceForLastResumeDataCall = NONE;
    }
}

void Controller::processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p) {

    // Check that state of controller is compatible with the arrival of this alert
    if(sourceForLastResumeDataCall == NONE || numberOfOutstandingResumeDataCalls == 0) {

        qCCritical(CATEGORY) << "Received resume data alert, despite no outstanding calls, hence droping alert, but this is a bug.";
        return;
    }

    qCWarning(CATEGORY) << "Received resume data failed alert, something went wrong.";

    // Decrease outstanding count
    numberOfOutstandingResumeDataCalls--;
}

void Controller::addTorrentFromTorrentFile(const QString & torrentFile) {

    // Check that torrent file exists
    if(!QFile::exists(torrentFile)) {
        qCCritical(CATEGORY) << "Torrent file " << torrentFile.toStdString().c_str() << " does not exist.";
        return;
    }

    // Setup parameters
    libtorrent::add_torrent_params params;

    // Load torrent file
    libtorrent::error_code ec;
    boost::intrusive_ptr<libtorrent::torrent_info> torrentInfoPointer = new libtorrent::torrent_info(torrentFile.toStdString().c_str(), ec);
    if(ec) {
        qCCritical(CATEGORY) << "Invalid torrent file: " << ec.message().c_str();
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
        qCWarning(CATEGORY) << "Malformed magnet link: " << ec.message().c_str();
        return;
    }

    // Show window for adding torrent
    AddTorrentDialog * addTorrentDialog = new AddTorrentDialog(this, params);
    addTorrentDialog->exec();

    // Delete window resources
    delete addTorrentDialog;
}

void Controller::removeTorrent(const libtorrent::torrent_handle & torrentHandle) {

    // Remove from session
    session.remove_torrent(torrentHandle);
}

void Controller::pauseTorrent(libtorrent::torrent_handle & torrentHandle) {

    // Turn off auto managing
    torrentHandle.auto_managed(false);

    // Pause
    torrentHandle.pause(libtorrent::torrent_handle::graceful_pause);

    // We save resume data when the pause alert is issued by libtorrent
}

void Controller::startTorrent(libtorrent::torrent_handle & torrentHandle) {

    /*        // Turn on auto managing
        torrentHandle.auto_managed(true);

        // Start
        torrentHandle.resume();
        */
}

libtorrent::torrent_handle Controller::getTorrentHandleFromInfoHash(const libtorrent::sha1_hash & info_hash) {
    return session.find_torrent(info_hash);
}

// comment out later
libtorrent::session & Controller::getSession() {
    return session;
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
            qCDebug(CATEGORY) << "no valid info_hash set.";
            return;
        }
    }

    // Create save_path if it does not exist
    if(!(QDir()).mkpath(params.save_path.c_str())) {

        qCCritical(CATEGORY) << "Could not create save_path: " << params.save_path.c_str();
        return;
    }

    // Load resume data if it exists
    //params.resume_data.clear(); // <-- should be empty
    loadResumeDataForTorrent(params.save_path.c_str(), resumeFileNameForTorrent(params.info_hash), params.resume_data);

    // Set parameters
    //params.storage_mode = (storage_mode_t)allocation_mode; //  disabled_storage_constructor;
    //params.flags |= add_torrent_params::flag_paused; //  |= add_torrent_params::flag_seed_mode;
    //params.flags &= ~add_torrent_params::flag_duplicate_is_error;
    //params.flags |= add_torrent_params::flag_auto_managed; // |= add_torrent_params::flag_share_mode;
    //params.userdata = (void*)strdup(torrent.c_str());

	// Add to libtorrent session
    /*
    libtorrent::add_torrent_params d;
    d.save_path = std::string("C:\\");
    d.url = std::string("magnet:?xt=urn:btih:781ad3adbd9b81b64e4c530712ae9199b1dfbae5&dn=Now+You+See+Me+%282013%29+1080p+EXTENDED+BrRip+x264+-+YIFY&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3");
    session.async_add_torrent(d);
    */
    session.async_add_torrent(params);

    // Add to controller
    // - DO WE EVEN NEED TO KEEP TRACK OF THIS? -
    addTorrentParameters.push_back(params);
}

void Controller::saveStateToFile(const char * file) {

	// Save present state of session in entry
	libtorrent::entry libtorrentSessionSettingsEntry;
	session.save_state(libtorrentSessionSettingsEntry);

	// Create state object
	ControllerState controllerState(libtorrentSessionSettingsEntry, 
									portRange, 
									addTorrentParameters, 
									dhtRouters);

	// Save to file
	controllerState.saveToFile(file);
}

void Controller::begin_close() {

    // Check if we can actually save resume data at this time
    if(sourceForLastResumeDataCall != NONE) {
        qCCritical(CATEGORY) << "ERROR: Cannot save resume data at this time due to outstanding resume_data calls, try again later.";
        return;
    } else  if(numberOfOutstandingResumeDataCalls != 0) {
        qCCritical(CATEGORY) << "ERROR: Invalid state found, numberOfOutstandingResumeDataCalls != 0, despite sourceForLastResumeDataCall == NONE. Resume data cannot be saved.";
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
        qCDebug(CATEGORY) << "Attempting to generate resume data for " << numberOutStanding << " torrents.";
}

void Controller::finalize_close() {

    qCDebug(CATEGORY) << "finalize_close() run.";

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
