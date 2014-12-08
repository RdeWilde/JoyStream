#include "PeerPluginId.hpp"

PeerPluginId::PeerPluginId() {
}

PeerPluginId::PeerPluginId(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endPoint)
    : info_hash_(info_hash)
    , endPoint_(endPoint)
{
}
