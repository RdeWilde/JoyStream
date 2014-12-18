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

    // Constructor using members
    ControllerConfiguration(const libtorrent::entry & libtorrentSessionSettingsEntry,
                    const std::pair<int, int> & portRange,
                    const std::vector<TorrentConfiguration *> & torrentConfigurations,
                    const std::vector<std::pair<std::string, int>> & dhtRouters);

    // Constructor using dictionary entry
    ControllerConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    // Constructor using file
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
     * "torrentConfigurations" -> entry::list_type object, with list item objects being of type entry::dictionary_type and
     * representing state of corresponding torrent as dictated by encoding used in TorrentConfiguration::toDictionaryEntry().
     */
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry);

	// Saves to file
	void saveToFile(const char * file);

    // Inserts torrent configuration.
    // This object then takes ownership of file, and deletes in constructor
    bool insertTorrentConfiguration(const TorrentConfiguration * torrentConfiguration);

    // Getters & Setters
    libtorrent::entry getLibtorrentSessionSettingsEntry() const;
    void setLibtorrentSessionSettingsEntry(const libtorrent::entry & libtorrentSessionSettingsEntry);

    std::pair<int, int> getPortRange() const;

    std::vector<std::pair<std::string, int>> getDhtRouters() const;

    std::vector<TorrentConfiguration *>::const_iterator getBeginTorrentConfigurationsIterator() const;
    std::vector<TorrentConfiguration *>::const_iterator getEndTorrentConfigurationsIterator() const;

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
    std::vector<TorrentConfiguration *> _torrentConfigurations;
};

#endif // CONTROLLER_CONFIGURATION_HPP
