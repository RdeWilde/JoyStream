#ifndef START_OBSERVER_TORRENT_PLUGIN_HPP
#define START_OBSERVER_TORRENT_PLUGIN_HPP

#include "PluginRequest.hpp"
#include <libtorrent/sha1_hash.hpp>

class StartObserverTorrentPlugin : public PluginRequest
{
public:

    // Constructor
    StartObserverTorrentPlugin();

    // Constructor from members
    StartObserverTorrentPlugin(const libtorrent::sha1_hash & infoHash);

    // Returns request type
    virtual PluginRequestType getPluginRequestType() const;

    // Getters
    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash & infoHash);

private:

    // Torrent info_hash
    libtorrent::sha1_hash _infoHash;

    // Observer configuration
};

#endif // START_OBSERVER_TORRENT_PLUGIN_HPP
