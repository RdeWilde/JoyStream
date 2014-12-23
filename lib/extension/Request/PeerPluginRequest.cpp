#include "PeerPluginRequest.hpp"

PeerPluginRequest::PeerPluginRequest(const libtorrent::sha1_hash & info_hash, const libtorrent::tcp::endpoint & endpoint)
    : _info_hash(info_hash)
    , _endpoint(endpoint) {
}
