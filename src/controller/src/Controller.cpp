
#include "controller/include/Controller.hpp"
#include "controller/include/Config.hpp"
#include "controller/include/Exceptions/ListenOnException.hpp"
#include "controller/include/Exceptions/MissingInfoHashViewRequestException.hpp"

#include <libtorrent/alert_types.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/bencode.hpp>

#include <QMessageBox>

Controller::Controller(const ControllerState & state)
    : session(libtorrent::fingerprint("BR", BITSWAPR_VERSION_MAJOR, BITSWAPR_VERSION_MINOR, 0, 0) , libtorrent::session::add_default_plugins + libtorrent::alert::debug_notification + libtorrent::alert::stats_notification)
    , view(this){

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

void Controller::start() {

    // Show window
    view.show();

    // Start servicing session in new thread
    /*
     * ADD CODE HERE TO PREVENT STARTING THREAD MULTIPLE TIMES, SINCE
     * CONTROLLER IS NOT REINTRANT.
     */

    // Start thread which runs session looop:
    // http://antonym.org/2009/05/threading-with-boost---part-i-creating-threads.html
    sessionLoopThread = boost::thread(&Controller::sessionLoop, this);
}

void Controller::sessionLoop() {

	// Allocate alerts queue
	std::deque<libtorrent::alert*> alerts;

    int counter = 0;

	while(true) {

        //std::cerr << "looping: " << counter++ << std::endl ;

		// Get fresh libtorrent alerts
		session.pop_alerts(&alerts);
		
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

        // Sleep 100ms
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
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
	{
		std::cout << "state_update_alert" << std::endl;

		/*
		bool need_filter_update = false;
		for (std::vector<torrent_status>::iterator i = p->status.begin();
			i != p->status.end(); ++i)
		{
			boost::unordered_set<torrent_status>::iterator j = all_handles.find(*i);
			// don't add new entries here, that's done in the handler
			// for add_torrent_alert
			if (j == all_handles.end()) continue;
			if (j->state != i->state
				|| j->paused != i->paused
				|| j->auto_managed != i->auto_managed)
				need_filter_update = true;
			((torrent_status&)*j) = *i;
		}
		if (need_filter_update)
			update_filtered_torrents(all_handles, filtered_handles, counters);

		return true;
		*/
	}

}

void Controller::processAddTorrentAlert(libtorrent::add_torrent_alert const * p) {

	/*
	std::string filename;
	if (p->params.userdata)
	{
		filename = (char*)p->params.userdata;
		free(p->params.userdata);
	}

	if (p->error)
	{
		fprintf(stderr, "failed to add torrent: %s %s\n", filename.c_str(), p->error.message().c_str());
	}
	else
	{
		torrent_handle h = p->handle;

		if (!filename.empty())
			files.insert(std::pair<const std::string, torrent_handle>(filename, h));
		else
			non_files.insert(h);

		h.set_max_connections(max_connections_per_torrent);
		h.set_max_uploads(-1);
		h.set_upload_limit(torrent_upload_limit);
		h.set_download_limit(torrent_download_limit);
		h.use_interface(outgoing_interface.c_str());
#ifndef TORRENT_DISABLE_RESOLVE_COUNTRIES
		h.resolve_countries(true);
#endif

		// if we have a peer specified, connect to it
		if (!peer.empty())
		{
			char* port = (char*) strrchr((char*)peer.c_str(), ':');
			if (port > 0)
			{
				*port++ = 0;
				char const* ip = peer.c_str();
				int peer_port = atoi(port);
				error_code ec;
				if (peer_port > 0)
					h.connect_peer(tcp::endpoint(address::from_string(ip, ec), peer_port));
			}
		}

		boost::unordered_set<torrent_status>::iterator j
			= all_handles.insert(h.status()).first;
		if (show_torrent(*j, torrent_filter, counters))
		{
			filtered_handles.push_back(&*j);
			need_resort = true;
		}
	}
	*/

}

void Controller::submitAddTorrentViewRequest(const libtorrent::add_torrent_params & params) {

	/* 
	* Check that info_hash is valid,
	* by either directly from add_torrent_params, 
	* or torrent_info field of latter
	
	libtorrent::sha1_hash info_hash;

	if(!params.info_hash.is_all_zeros())
		info_hash = params.info_hash;
	else if(params.ti.get()->info_hash().is_all_zeros())
		info_hash = params.ti.get()->info_hash();
	else
		throw MissingInfoHashViewRequestException(params);
	*/

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
