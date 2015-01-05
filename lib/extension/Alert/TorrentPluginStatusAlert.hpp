#ifndef TORRENT_PLUGIN_STATUS_ALERT_HPP
#define TORRENT_PLUGIN_STATUS_ALERT_HPP

#include "TorrentPluginAlert.hpp"
#include "AlertTypes.hpp"
#include "../PluginMode.hpp"

class TorrentPluginStatusAlert : public TorrentPluginAlert {

public:

    // Public member required for alert_cast
    const static int alert_type = TORRENT_PLUGIN_STATUS_ALERT_ID;

    // Constructor based on members
    TorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash
                             ,int numberOfPeers
                             ,int numberOfPeersWithExtension
                             ,bool pluginStarted
                             ,int tokensReceived
                             ,int tokensSent
                             ,PluginMode mode);

    // Constructor based on reference
    TorrentPluginStatusAlert(const TorrentPluginStatusAlert & torrentPluginStatusAlert);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<alert> clone() const;

    // Getters
    int numberOfPeers() const;
    int numberOfPeersWithExtension() const;
    bool pluginStarted() const;
    int tokensReceived() const;
    int tokensSent() const;
    PluginMode mode() const;

private:

    // Number of peers torrent plugin has been connected with (not all peers on libtorrent::torrent peer list?)
    int _numberOfPeers;

    // Among numberOfPeers_, how many did a correct BEP10 handshake indicating they supported BEP43
    int _numberOfPeersWithExtension;

    // Management mode of torrent plugin
    bool _pluginStarted;

    // Number of tokens received during this session
    int _tokensReceived;

    // Numbre of tokens sent during this session
    int _tokensSent;

    // Mode of plugin
    PluginMode _pluginMode;
};

#endif // TORRENT_PLUGIN_STATUS_ALERT_HPP
