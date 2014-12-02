#ifndef PEER_PLUGIN_ID_HPP
#define PEER_PLUGIN_ID_HPP

#include <libtorrent/socket_io.hpp> // detail::write_endpoint, detail::write_uint8

#include <string>

class PeerPlugin;

/**
 * THIS CLASS WAS LIKELY A USELESS MISTAKE, REMOVE LATER?
 */

class PeerPluginId
{
public:

    // Constructor
    PeerPluginId(const PeerPlugin * peerPlugin);

    //PeerPluginId(const boost::asio::ip::tcp::endpoint & endPoint, const libtorrent::sha1_hash & info_hash);

    const std::string & getId() const;

    bool operator<(PeerPluginId other) const;

private:

    // IP:PORT:info_hash
    std::string id_;

    boost::asio::ip::tcp::endpoint endPoint_;

    libtorrent::sha1_hash info_hash_;
};

#endif // PEER_PLUGIN_ID_HPP
