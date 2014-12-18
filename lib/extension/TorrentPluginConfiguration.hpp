#ifndef TORRENT_PLUGIN_CONFIGURATION_HPP
#define TORRENT_PLUGIN_CONFIGURATION_HPP

#include "PluginMode.hpp"

#include <libtorrent/entry.hpp> // because you cant forward declare typedefs (libtorrent::entry::dictionary_type)
#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/socket.hpp> // tcp::endpoint

#include <QtGlobal> // quint8,...

class PeerPluginConfiguration;

/**
 * Was factored out of TorrentConfiguration because Plugin
 * does not need to know about all parts of torrent configuration
 * only the plugin spesific ones.
 */

class TorrentPluginConfiguration {

public:

    // Default constructor, for default constructor in TorrentConfiguration
    TorrentPluginConfiguration();

    // Constructor from members
    TorrentPluginConfiguration(PluginMode pluginMode, bool enableBanningSets);

    // Constructor from dictionary
    TorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    /*
     * DISABLED
    // Adds a peer plugin configuration
    void insertPeerPluginConfiguration(const PeerPluginConfiguration * peerPluginConfiguration);

    // Beginning iterator of _peerPluginConfigurations
    std::map<libtorrent::tcp::endpoint, PeerPluginConfiguration *>::const_iterator getBeginPeerPluginConfigurationsIterator() const;

    // End iterator of _peerPluginConfigurations
    std::map<libtorrent::tcp::endpoint, PeerPluginConfiguration *>::const_iterator getEndPeerPluginConfigurationsIterator() const;
    */

    /**
     * Write configuration into dictionary
     * ===============================================================
     *
     * Torrent plugin configuration as it persists across sessions on disk
     * encoded as entry::dictionary_type with the following keys:
     *
     * IMPLEMENT LATER
     *
     */
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

    // Getters & Setters
    PluginMode getPluginMode() const;

    bool getEnableBanningSets() const;

protected:

    // Mode of plugin
    PluginMode _pluginMode;

    // Use the two sets below when accepting new peers in new_connect
    bool _enableBanningSets;

    // Configurations for all active peer plugins
    //std::map<libtorrent::tcp::endpoint, PeerPluginConfiguration *> _peerPluginConfigurations;
};

#endif // TORRENT_PLUGIN_CONFIGURATION_HPP
