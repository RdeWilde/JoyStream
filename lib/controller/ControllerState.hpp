#ifndef CONTROLLER_STATE_HPP
#define CONTROLLER_STATE_HPP

#include <libtorrent/entry.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>

class ControllerState {

private:
	
	/*
	* Holds all settings of session, that includes
	* session_settings, dht_settings, dht_state,
	* proxy_settings, i2p_proxy, pe_settings, feed
	* and extension settings.
	*
	* It would be cleaner to save class representations rather 
	* than using entry type, however that gets messy and/or
	* requires lots of extra work for various reasons.
	*/
	libtorrent::entry libtorrentSessionSettingsEntry;

	// Listening port range: DO WE EVEN NEED THIS? IT MAY BE PART OF DHT_SETTINGS ENTRY?
	std::pair<int, int> portRange;

	// Parameters for each torrent, includes fast resume data and user data
	std::vector<libtorrent::add_torrent_params> addTorrentParameters;

	// Dht routers
	std::vector<std::pair<std::string, int>> dhtRouters;

	// Routines for en/de-coding object as dictionary entry
	/**
	* This object is encoded as entry::dictionary_type by adding
	* the following keys:
	*
	* "libtorrentSettings" -> entry::dictionary_type from session.save_state().
	*
	* "portRange" -> entry::list_type with two positive integers used as start (first) and end (second) 
	* of port range for running bitswapr.
	*
	* "addTorrentParameters" -> entry::list_type with entry::dictionary_type objects, each encoding libtorrent::add_torrent_params
	* for a torrent to be loaded into client when started.
	*
	* "dhtRouters" -> entry::list_type with entry::list_type objects with two elements, each encoding a dht router by the host (first)
	* and port (second).
	*/
    static libtorrent::entry::dictionary_type addTorrentParamsToDictionaryEntry(const libtorrent::add_torrent_params & addTorrentParams);
	static void dictionaryEntryToAddTorrentParams(libtorrent::add_torrent_params & addTorrentParams, const libtorrent::entry::dictionary_type & dictionaryEntry);

	void loadFromDictionaryEntry(const libtorrent::entry::dictionary_type & bitSwaprStateDictionaryEntry);
public:

	// Default name of parameter file name
    //static const std::string DEFAULT_FILE_NAME;
	
	// Constructor 
	ControllerState(const libtorrent::entry & libtorrentSessionSettingsEntry_,
					const std::pair<int, int> & portRange_,
					const std::vector<libtorrent::add_torrent_params> & addTorrentParameters_,
					const std::vector<std::pair<std::string, int>> & dhtRouters_);

	// Constructor using default parameter settings
	ControllerState();

	// Constructor using state
	ControllerState(const libtorrent::entry::dictionary_type & bitSwaprStateDictionaryEntry);

	// Constructor using state file
	ControllerState(const char * fileName);

	// Saves to dictionary entry
	void saveToDictionaryEntry(libtorrent::entry::dictionary_type & bitSwaprStateDictionaryEntry);

	// Saves to file
	void saveToFile(const char * file);

	// Get routines
    libtorrent::entry getLibtorrentSessionSettingsEntry() const;
    std::pair<int, int> getPortRange() const;
    std::vector<libtorrent::add_torrent_params> getTorrentParameters() const;
    std::vector<std::pair<std::string, int>> getDhtRouters() const;
};

#endif
