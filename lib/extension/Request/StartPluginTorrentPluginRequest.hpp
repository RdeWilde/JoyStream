#ifndef START_PLUGIN_TORRENT_PLUGIN_REQUEST_HPP
#define START_PLUGIN_TORRENT_PLUGIN_REQUEST_HPP

#include "TorrentPluginRequest.hpp"
#include "extension/TorrentPluginConfiguration.hpp"

/**
 * @brief The StartPluginTorrentPluginRequest class
 *
 * MAKE TO DISTINCT TYPES LATER, START_WITH_EXISTING_CONFIGURATION,
 * WHICH BASICALLY SAYS USE WHAT YOU HAVE. AND A START WITH CONFIGURITAON
 * WHICH HAS A CONFIGURATION AS A MEMBER BY VALUE
 */
class StartPluginTorrentPluginRequest : public TorrentPluginRequest
{

public:
    StartPluginTorrentPluginRequest(const libtorrent::sha1_hash & info_hash, TorrentPluginConfiguration * torrentPluginConfiguration);

    // Returns request type
    virtual TorrentPluginRequestType getTorrentPluginRequestType() const;

    // Getters
    const TorrentPluginConfiguration * getTorrentPluginConfiguration() const;

private:

    // Configuration
    // We do not own this, recipient of configuration must delete.
    TorrentPluginConfiguration * _torrentPluginConfiguration;
};

#endif // START_PLUGIN_TORRENT_PLUGIN_REQUEST_HPP
