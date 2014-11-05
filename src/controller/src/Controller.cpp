
#include "controller/include/Controller.hpp"
#include "Config.hpp"
#include "controller/include/Exceptions/ListenOnException.hpp"
#include "controller/include/Exceptions/MissingInfoHashViewRequestException.hpp"

#include <libtorrent/alert_types.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/bencode.hpp>

#include <QObject>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QtGlobal>

Controller::Controller(const ControllerState & state)
    : session(libtorrent::fingerprint("BR", BITSWAPR_VERSION_MAJOR, BITSWAPR_VERSION_MINOR, 0, 0) , libtorrent::session::add_default_plugins + libtorrent::alert::debug_notification + libtorrent::alert::stats_notification)
    , view(this){

    // Connect controller signals with view slots
    QObject::connect(this, SIGNAL(addTorrent(const libtorrent::sha1_hash &, const std::string &, int)),
                      &view, SLOT(addTorrent(const libtorrent::sha1_hash &, const std::string &, int)));

    QObject::connect(this, SIGNAL(addTorrentFailed(const std::string &, const libtorrent::sha1_hash &, const libtorrent::error_code &)),
                      &view, SLOT(addTorrentFailed(const std::string &, const libtorrent::sha1_hash &, const libtorrent::error_code &)));

    QObject::connect(this, SIGNAL(updateTorrentStatus(const std::vector<libtorrent::torrent_status> &)),
                      &view, SLOT(updateTorrentStatus(const std::vector<libtorrent::torrent_status> &)));

    QObject::connect(this, SIGNAL(updateTorrentStatus(const libtorrent::torrent_status &)),
                      &view, SLOT(updateTorrentStatus(const libtorrent::torrent_status &)));

    QObject::connect(this, SIGNAL(removeTorrent(const libtorrent::sha1_hash &)),
                      &view, SLOT(removeTorrent(const libtorrent::sha1_hash &)));

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

	// Start listening
	boost::system::error_code listenOnErrorCode;
	session.listen_on(state.getPortRange(), listenOnErrorCode);

	// throw
	if(listenOnErrorCode)
		throw ListenOnException(listenOnErrorCode);
}

void Controller::begin() {

    // Show window
    view.show();

    // Start servicing session in new thread
    /*
     * ADD CODE HERE TO PREVENT STARTING THREAD MULTIPLE TIMES, SINCE
     * CONTROLLER IS NOT REINTRANT.
     */

    // Start thread which runs session looop:
    // http://antonym.org/2009/05/threading-with-boost---part-i-creating-threads.html
    //sessionLoopThread = boost::thread(&Controller::sessionLoop, this);
    this->start();
}

void Controller::run() {

    // Start timer which coordinates how often various session methods are called
    QElapsedTimer timer;

    // Set up time stamps
    qint64 lastPopAlerts = Q_INT64_C(0),
           lastPostTorrentUpdates = Q_INT64_C(0);

	// Allocate alerts queue
	std::deque<libtorrent::alert*> alerts;

    forever {

        // Get fresh libtorrent alerts if time is right
        qint64 elapsed = timer.elapsed();
        if(elapsed - lastPopAlerts > CONTROLLER_POP_ALERTS_DELAY) {

            //std::cerr << " calling pop_alerts " << elapsed << std::endl;

            lastPopAlerts = elapsed;
            session.pop_alerts(&alerts);
        }
		
		// Iterate alerts
		for (std::deque<libtorrent::alert* >::iterator i = alerts.begin()
			, end(alerts.end()); i != end; i++) {

			// Process this alert
			processAlert(*i);

			// Clear memory used by alert
			delete *i;
		}

        // Clear alerts queue
        alerts.clear();

        // Tell session to give us an update on the status of torrents, if time is right
        elapsed = timer.elapsed();
        if(elapsed - lastPostTorrentUpdates > CONTROLLER_POST_TORRENT_UPDATES_DELAY) {

            //std::cerr << " calling post_torrent_updates " << elapsed << std::endl;

            lastPostTorrentUpdates = elapsed;
            session.post_torrent_updates();
        }

        // Sleep 100ms, take this away later
        sleep(100);
	}

}

void Controller::processAlert(libtorrent::alert const * a) {
	
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
	else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a))
	{
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

}

void Controller::processAddTorrentAlert(libtorrent::add_torrent_alert const * p) {

    // Name of torrent
    std::string name = p->params.ti->name();

    // Check if there was an error
    if (p->error)
        emit addTorrentFailed(name, p->params.info_hash, p->error);
    else {

        // Save handle
        torrentHandles.push_back(p->handle);

        /*
		h.set_max_connections(max_connections_per_torrent);
		h.set_max_uploads(-1);
		h.set_upload_limit(torrent_upload_limit);
		h.set_download_limit(torrent_download_limit);
		h.use_interface(outgoing_interface.c_str());
        */

        // Add torrent to view
        emit addTorrent(p->handle.info_hash(), name, (p->params.ti)->total_size());
	}
}

void Controller::processStatusUpdateAlert(libtorrent::state_update_alert const * p) {
    emit updateTorrentStatus(p->status);
}

void Controller::submitAddTorrentViewRequest(const libtorrent::add_torrent_params & params) {

	// Add to libtorrent session
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
