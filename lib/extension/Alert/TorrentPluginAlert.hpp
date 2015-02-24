#ifndef TORRENT_PLUGIN_ALERT_HPP
#define TORRENT_PLUGIN_ALERT_HPP

#include <libtorrent/alert.hpp>
#include <libtorrent/peer.hpp> // sha1_hash

// Abstract type for all torrent plugin alerts
class TorrentPluginAlert : public libtorrent::alert {

public:

    // Default constructor
    TorrentPluginAlert();

    // Constructor
    TorrentPluginAlert(const libtorrent::sha1_hash & infoHash);

    // Virtual routines from libtorrent::alert
    virtual int type() const = 0;
    virtual char const* what() const = 0;
    virtual std::string message() const = 0;
    virtual int category() const = 0;
    virtual std::auto_ptr<alert> clone() const = 0;

    // Getters
    libtorrent::sha1_hash infoHash() const;

protected:

    libtorrent::sha1_hash _infoHash;
};

#endif // TORRENT_PLUGIN_ALERT_HPP
