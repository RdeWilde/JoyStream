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
 *
 * Is now an object passed arround when starting torrent plugin.
 * THIS HAS TO CHANGE, SHOULD BE BY VALUE/REF. AND THEN
 * IN THE FUTURE ONE HAS TO EDIT THE LIVE CONFIGURATION OF
 * THE PLUGIN IN SOME WAY, AND THIS MAY BE WITHOUT THE CONFIGURATION,
 * SINCE CONFIGURATION IS PERSISTANT DISK STATE, AND THIS IS NOT FULL
 * STATE THAT ONE MAY WANT TO ADMINISTRATE LIVE. FIGURE THIS OUT LATER.
 */

class TorrentPluginConfiguration {

public:

    // Default constructor, for default constructor in TorrentConfiguration
    //TorrentPluginConfiguration();

    // Observe constructor from members
    TorrentPluginConfiguration(bool enableBanningSets);

    // Sell constructor from members
    TorrentPluginConfiguration(bool enableBanningSets, quint32 sellerPrice);

    // Buy constructor from member
    TorrentPluginConfiguration(bool enableBanningSets, quint32 buyerPrice, quint32 fee, qint32 btcVersion, qint16 waitTime, qint8 minPeers);

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

    // Getters
    PluginMode pluginMode() const;
    bool enableBanningSets() const;

//protected:

    // Mode of started plugin
    PluginMode _pluginMode;

    // Use the two sets below when accepting new peers in new_connect
    bool _enableBanningSets;

    /**
     * Mode spesific
     */

    // Sell
    quint32 _sellerPrice;

    // Buy
    quint32 _buyerPrice;
    quint32 _fee;
    qint32 _btcVersion;

    qint16 _waitTime;
    qint8 _minPeers;

    // Configurations for all active peer plugins
    //std::map<libtorrent::tcp::endpoint, PeerPluginConfiguration *> _peerPluginConfigurations;
};

#endif // TORRENT_PLUGIN_CONFIGURATION_HPP
