#include "TorrentPluginStatus.hpp"

TorrentPluginStatus::TorrentPluginStatus(libtorrent::sha1_hash & info_hash, int numberOfPeers, int numberOfPeersWithExtension, bool pluginOn, int tokensReceived, int tokensSent)
    : info_hash_(info_hash)
    , numberOfPeers_(numberOfPeers)
    , numberOfPeersWithExtension_(numberOfPeersWithExtension)
    , pluginOn_(pluginOn)
    , tokensReceived_(tokensReceived)
    , tokensSent_(tokensSent)
{
}

TorrentPluginStatus::TorrentPluginStatus()
    : info_hash_(libtorrent::sha1_hash())
    , numberOfPeers_(0)
    , numberOfPeersWithExtension_(0)
    , pluginOn_(false)
    , tokensReceived_(0)
    , tokensSent_(0)
{
}
