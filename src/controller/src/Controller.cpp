
#include "controller/include/Controller.hpp"
#include "Config.hpp"
#include "view/include/addtorrentdialog.h"
#include "controller/include/Exceptions/ListenOnException.hpp"

#include <libtorrent/alert_types.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>

#include <QObject>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QtGlobal>
#include <QDir>
#include <QFile>
#include <QByteArray>

#ifndef Q_MOC_RUN
#include <boost/bind.hpp>
#endif Q_MOC_RUN

Controller::Controller(const ControllerState & state)
    : session(libtorrent::fingerprint("BR", BITSWAPR_VERSION_MAJOR, BITSWAPR_VERSION_MINOR, 0, 0) , libtorrent::session::add_default_plugins + libtorrent::alert::debug_notification + libtorrent::alert::stats_notification)
    , view(this) {

    // Register types for signal and slots
    qRegisterMetaType<libtorrent::sha1_hash>();
    qRegisterMetaType<std::string>();
    qRegisterMetaType<libtorrent::error_code>();
    qRegisterMetaType<std::vector<libtorrent::torrent_status>>();
    qRegisterMetaType<libtorrent::torrent_status>();

    // Register type for QMetaObject::invokeMethod
    qRegisterMetaType<const libtorrent::alert*>();

	// Set session settings - these acrobatics with going back and forth seem to indicate that I may have done it incorrectly
	std::vector<char> buffer;
	libtorrent::bencode(std::back_inserter(buffer), state.getLibtorrentSessionSettingsEntry());
	libtorrent::lazy_entry settingsLazyEntry;
	libtorrent::error_code lazyBdecodeEc;
	libtorrent::lazy_bdecode(&buffer[0], &buffer[0] + buffer.size(), settingsLazyEntry, lazyBdecodeEc);
	session.load_state(settingsLazyEntry);

	// Add DHT routing nodes
	const std::vector<std::pair<std::string, int>> & r = state.getDhtRouters();

	for(std::vector<std::pair<std::string, int>>::const_iterator i = r.begin()
		, end(r.end()); i != end; ++i) {

		// Add router to session
		session.add_dht_router(*i);

		// Add to controller
		//std::pair<std::string, int> router = *i;
		dhtRouters.push_back(*i);
	}

	// Add some sort of check that we actually have some dht routers?

	// Start DHT node
	session.start_dht();

	// Add torrents to session and to controller
	const std::vector<libtorrent::add_torrent_params> & p = state.getTorrentParameters();
	
	for(std::vector<libtorrent::add_torrent_params>::const_iterator i = p.begin()
		, end(p.end()); i != end; ++i) {

		// Add to session
		session.async_add_torrent(*i);

		// Add to controller
		addTorrentParameters.push_back(*i);
	}

    // Set libtorrent to call processAlert when alert is created
    session.set_alert_dispatch(boost::bind(&Controller::libtorrent_alert_dispatcher_callback, this, _1));

	// Start listening
	boost::system::error_code listenOnErrorCode;
	session.listen_on(state.getPortRange(), listenOnErrorCode);

	// throw
	if(listenOnErrorCode)
		throw ListenOnException(listenOnErrorCode);

    // Start timer which calls session.post_torrent_updates at regular intervals
    statusUpdateTimer.setInterval(POST_TORRENT_UPDATES_DELAY);
    QObject::connect(&statusUpdateTimer, SIGNAL(timeout()), this, SLOT(callPostTorrentUpdates()));
    statusUpdateTimer.start();

    // Show view
    view.show();
}

void Controller::callPostTorrentUpdates() {
    session.post_torrent_updates();
}

/*
void Controller::connecToViewSlots(MainWindow * view) {

    // Setup view, connect controller signals with view slots
    QObject::connect(this, SIGNAL(addTorrent(const libtorrent::sha1_hash &, const std::string &, int)),
                      view, SLOT(addTorrent(const libtorrent::sha1_hash &, const std::string &, int)));

    QObject::connect(this, SIGNAL(addTorrentFailed(const std::string &, const libtorrent::sha1_hash &, const libtorrent::error_code &)),
                      view, SLOT(addTorrentFailed(const std::string &, const libtorrent::sha1_hash &, const libtorrent::error_code &)));

    QObject::connect(this, SIGNAL(updateTorrentStatus(const std::vector<libtorrent::torrent_status> &)),
                      view, SLOT(updateTorrentStatus(const std::vector<libtorrent::torrent_status> &)));

    QObject::connect(this, SIGNAL(updateTorrentStatus(const libtorrent::torrent_status &)),
                      view, SLOT(updateTorrentStatus(const libtorrent::torrent_status &)));

    QObject::connect(this, SIGNAL(removeTorrent(const libtorrent::sha1_hash &)),
                      view, SLOT(removeTorrent(const libtorrent::sha1_hash &)));
}
*/

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

    // In each case, tell bitswapr thread to run the given method
	if (libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a))
	{
		std::cout << "metadata_received_alert" << std::endl;

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
    else if (libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a))
        processAddTorrentAlert(p);
    else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a)) {
		std::cout << "torrent_finished_alert" << std::endl;

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
	else if (libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a))
	{
		std::cout << "save_resume_data_alert" << std::endl;

		/*
		--num_outstanding_resume_data;
		torrent_handle h = p->handle;
		TORRENT_ASSERT(p->resume_data);
		if (p->resume_data)
		{
			std::vector<char> out;
			bencode(std::back_inserter(out), *p->resume_data);
			torrent_status st = h.status(torrent_handle::query_save_path);
			save_file(combine_path(st.save_path, combine_path(".resume", to_hex(st.info_hash.to_string()) + ".resume")), out);
			if (h.is_valid()
				&& non_files.find(h) == non_files.end()
				&& std::find_if(files.begin(), files.end()
					, boost::bind(&handles_t::value_type::second, _1) == h) == files.end())
				ses.remove_torrent(h);
		}
		*/
	}
	else if (libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a))
	{
		std::cout << "save_resume_data_failed_alert" << std::endl;

		/*
		--num_outstanding_resume_data;
		torrent_handle h = p->handle;
		if (h.is_valid()
			&& non_files.find(h) == non_files.end()
			&& std::find_if(files.begin(), files.end()
				, boost::bind(&handles_t::value_type::second, _1) == h) == files.end())
			ses.remove_torrent(h);
			*/
	}
	else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a))
	{
		std::cout << "torrent_paused_alert" << std::endl;
		/*
		// write resume data for the finished torrent
		// the alert handler for save_resume_data_alert
		// will save it to disk
		torrent_handle h = p->handle;
		h.save_resume_data();
		++num_outstanding_resume_data;
		*/
	}
	else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a))
        processStatusUpdateAlert(p);

    /*
    if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a))
        processTorrentRemovedAlert(p);
    */

    // Delete alert
    //delete p;
}

/*
void Controller::processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p) {

    // Docs say p->handle may be invalid at this time.

    // Find torrent params in addTorrentParameters
    for(std::vector<libtorrent::add_torrent_params>::iterator i = addTorrentParameters.begin(),
        end(addTorrentParameters.end()); i != end;i++) {

        libtorrent::add_torrent_params & params = (*i);

        // Check for match
        if(params.info_hash == p->info_hash) {

            // Remove torrent
            addTorrentParameters.erase(i);

            // Notify view to remove torrent
            view.removeTorrent(p->info_hash);

            // Done
            break;
        }

    }

    std::cerr << "Got a torrent_removed_alert alert, but no match exists in addTorrentParameters." << std::endl;
}
*/

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
    if (p->error)
        view.addTorrentFailed(name, p->params.info_hash, p->error);
    else {

        // Save handle
        //torrentHandles.push_back(p->handle);

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

void Controller::addTorrentFromTorrentFile(const QString & torrentFile) {

    // Check that torrent file exists
    if(!QFile::exists(torrentFile)) {
        std::cerr << "Torrent file " << torrentFile.toStdString().c_str() << " does not exist." << std::endl;
        return;
    }

    // Setup parameters
    libtorrent::add_torrent_params params;

    // Load torrent file
    boost::intrusive_ptr<libtorrent::torrent_info> torrentInfo;
    libtorrent::error_code ec;
    torrentInfo = new libtorrent::torrent_info(torrentFile.toStdString().c_str(), ec);
    if(ec) {
        std::cout << "Invalid torrent file: " << ec.message().c_str() << std::endl;
        return;
    }

    params.ti = torrentInfo;

    // Show window for adding torrent
    AddTorrentDialog * addTorrentDialog = new AddTorrentDialog(this, params);
    addTorrentDialog->exec();

    // Delete window resources
    // is this a good idea
    //delete addTorrentDialog;
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
        std::cout << "Malformed magnet link: " << ec.message().c_str() << std::endl;
        return;
    }

    // Show window for adding torrent
    AddTorrentDialog * addTorrentDialog = new AddTorrentDialog(this, params);
    addTorrentDialog->exec();

    // Delete window resources
    // is this a good idea
    //delete addTorrentDialog;
}

void Controller::removeTorrent(const libtorrent::torrent_handle torrentHandle) {

    //std::cerr << "view was able to call controller" << std::endl;
    session.remove_torrent(torrentHandle);
}

libtorrent::torrent_handle Controller::getTorrentHandleFromInfoHash(const libtorrent::sha1_hash & info_hash) {
    return session.find_torrent(info_hash);
}

libtorrent::session & Controller::getSession() {
    return session;
}

void Controller::addTorrent(libtorrent::add_torrent_params & params) {

    QString savePath(params.save_path.c_str());

    // Create save_path if it does not exist
    QDir savePathDirectory;
    bool failed = !savePathDirectory.mkpath(savePath);

    if(failed) {
        std::cerr << "Could not create save_path: " << savePath.toStdString().c_str() << std::endl;
        return;
    }

    // Load resume data if it exists
    QString resumeFileName = (libtorrent::to_hex(params.info_hash.to_string()) + ".resume").c_str();

    if(QDir(savePath).exists(resumeFileName)) {

        // Vector for resume data
        std::vector<char> resume_data;

        // Open file
        QFile file(resumeFileName);

        if(!file.open(QIODevice::ReadOnly)) {
            std::cerr << "Could not open : " << resumeFileName.toStdString().c_str() << std::endl;
            return;
        }

        // Read entire file
        QByteArray fullFile = file.readAll();

        // Populate resume_data vector
        for(QByteArray::iterator i = fullFile.begin(),
            end(fullFile.end()); i != end; i++)
            resume_data.push_back(*i);

        // set parameter
        params.resume_data = resume_data;
    }

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

void Controller::close() {

    // Stop libtorrent

    // Save libtorrent state, resume data etc.

    // Save state to file

    // Stop all event further event processing

    // stop timer
    //statusUpdateTimer

    // Done

}
