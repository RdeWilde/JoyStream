#include "PeerPluginId.hpp"
#include "PeerPlugin.hpp"


/*
PeerPluginId::PeerPluginId(const PeerPlugin * peerPlugin)
    : PeerPluginId(peerPlugin->getEndPoint(), peerPlugin->getInfoHash()) // delegated ctr did not work
{ }
*/

// PeerPluginId::PeerPluginId(const boost::asio::ip::tcp::endpoint & endPoint, const libtorrent::sha1_hash & info_hash)

PeerPluginId::PeerPluginId(const PeerPlugin * peerPlugin)
    : endPoint_(peerPlugin->getEndPoint())
    , info_hash_(peerPlugin->getInfoHash())
{
    // Create back inserter to write to string
    std::back_insert_iterator<std::string> id_inserter(id_);

    // Write [4 bytes IP address][2 byte unsigned int port]
    libtorrent::detail::write_endpoint(endPoint_, id_inserter);

    // Write bytes of sha1_hash
    for(libtorrent::sha1_hash::iterator i = info_hash_.begin(),
        end(info_hash_.end());i != end;i++)
        libtorrent::detail::write_int8(*i, id_inserter); // unsigned char to int 8
}

const std::string & PeerPluginId::getId() const {
    return id_;
}

bool PeerPluginId::operator<(PeerPluginId other) const {
    return this->id_.compare(other.getId());
}

