#ifndef CONTROLLER_CONFIGURATION_HPP
#define CONTROLLER_CONFIGURATION_HPP

#include <libtorrent/entry.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>

class TorrentConfiguration;

class ControllerConfiguration {

public:

    // Constructor using default parameter settings
    ControllerConfiguration();

    // Destructor
    ~ControllerConfiguration();

    // Constructor using fields
    ControllerConfiguration(const libtorrent::entry & libtorrentSessionSettingsEntry,
                    const std::pair<int, int> & portRange,
                    const std::map<libtorrent::sha1_hash, TorrentConfiguration *> torrentParameters,
                    const std::vector<std::pair<std::string, int>> & dhtRouters);

    // Constructor using dictionary entry
    ControllerConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

	// Constructor using state file
    ControllerConfiguration(const char * fileName);

    /**
     * Saves to dictionary entry
     * ===============================================================
     *
     * Controller parameters as they persist across sessions on disk
     * encoded as entry::dictionary_type with the following keys:
     *
     * "libtorrentSettings" -> entry::dictionary_type object from session.save_state().
     *
     * "portRange" -> entry::list_type object with two positive integers used as start (first) and end (second)
     * of port range for running bitswapr.
     *
     * "dhtRouters" -> entry::list_type object with entry::list_type objects with two elements, each encoding a dht router by the host (first)
     * and port (second).
     *
     * "persistentTorrentStates" -> entry::dictionary_type object, with key representing torrent info hash and value being
     * entry::dictionary_type object representing state of corresponding torrent as dictated by encoding used in PersistentTorrentState::toDictionaryEntry().
     */
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry);

	// Saves to file
	void saveToFile(const char * file);

    // Getters
    libtorrent::entry & getLibtorrentSessionSettingsEntry();
    std::pair<int, int> & getPortRange();
    std::map<libtorrent::sha1_hash, TorrentConfiguration *> & getTorrentConfigurations();
    std::vector<std::pair<std::string, int>> & getDhtRouters();

    // Setters
    void setLibtorrentSessionSettingsEntry(const libtorrent::entry & libtorrentSessionSettingsEntry);

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
    libtorrent::entry _libtorrentSessionSettingsEntry;

    // Listening port range: DO WE EVEN NEED THIS? IT MAY BE PART OF DHT_SETTINGS ENTRY?
    std::pair<int, int> _portRange;

    // Dht routers
    std::vector<std::pair<std::string, int>> _dhtRouters;

    // Torrent states
    std::map<libtorrent::sha1_hash, TorrentConfiguration *> _torrentConfigurations;
};

#endif // CONTROLLER_CONFIGURATION_HPP
