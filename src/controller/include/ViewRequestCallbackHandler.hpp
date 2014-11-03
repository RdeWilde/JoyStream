
#ifndef VIEW_REQUEST_CALLBACK_HANDLER_HPP
#define VIEW_REQUEST_CALLBACK_HANDLER_HPP

// Headers required for add_torrent_alert
#include <libtorrent/entry.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/alert_types.hpp>

// Likely going to be killed

class ViewRequestCallbackHandler {

private:

	//virtual void thisMethodExistsToMakeClassAbstract() = 0;

public:
	
	/*
	* Virtual call back routines which should be implemented
	* by class inheriting from this one.
	*/

	virtual void addTorrentCallback(const libtorrent::add_torrent_alert * p);

};

#endif
