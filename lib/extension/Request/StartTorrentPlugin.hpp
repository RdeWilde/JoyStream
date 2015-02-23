#ifndef START_TORRENT_PLUGIN_HPP
#define START_TORRENT_PLUGIN_HPP

#include "PluginRequest.hpp"
#include "extension/TorrentPlugin.hpp"

namespace libtorrent {
    class sha1_hash;
}

class StartTorrentPlugin : public PluginRequest {

public:

    // Constructor
    StartTorrentPlugin(const libtorrent::sha1_hash & infoHash, const TorrentPlugin::Configuration * configuration);

    // Returns request type
    virtual PluginRequestType getPluginRequestType() const;

    // Getters
    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash & infoHash);

    const TorrentPlugin::Configuration *configuration() const;
    void setConfiguration(const TorrentPlugin::Configuration *configuration);

private:

    // Torrent info_hash
    libtorrent::sha1_hash _infoHash;

    // Configuration, not owned by request
    const TorrentPlugin::Configuration * _configuration;
};



#endif // START_TORRENT_PLUGIN_HPP
