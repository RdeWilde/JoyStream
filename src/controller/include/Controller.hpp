#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller/include/ControllerState.hpp"
#include "controller/include/ViewRequestCallbackHandler.hpp"
#include "view/include/mainwindow.h"

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

class Controller {

private:
	
	/*
	* Underlying libtorrent session,
	* it is made public through get routine getSession()
	* so view thread can control session
	* since this is mostly thread safe
	* MAY need to be moved later.
	*/
	libtorrent::session session;

	// Listening port range
	std::pair<int, int> portRange;

	// Parameters for all torrents added
	std::vector<libtorrent::add_torrent_params> addTorrentParameters;

	// Dht routers
	std::vector<std::pair<std::string, int>> dhtRouters;

	// Handles callbacks of requests made by view
    //ViewRequestCallbackHandler * const handler_;

    // Main window
    MainWindow * view_;

	// Routine for processing a single libtorrent alert
	void processAlert(const libtorrent::alert * a);

	// Routine for processig libtorrent alerts
	void processAddTorrentAlert(libtorrent::add_torrent_alert const * p);

public:

	// Constructor starting session with given state
    Controller(const ControllerState & state, MainWindow * view);
	
	/*
	* Service loop for libtorrent alerts, calling this
	* causes infinite looping, closing client requires
	* issuing a command.
	*/
	void sessionLoop();

	// Saves to file
	void saveStateToFile(const char * file);

	// Returns session, for control by view
	libtorrent::session getSession();

	// Routines for submitting requests from view
	void submitAddTorrentViewRequest(const libtorrent::add_torrent_params & params);
};

#endif
