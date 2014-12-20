#ifndef TORRENT_PLUGIN_STATUS_ALERT_HPP
#define TORRENT_PLUGIN_STATUS_ALERT_HPP

#include "TorrentPluginAlert.hpp"
#include "AlertTypes.hpp"

class TorrentPluginStatusAlert : public TorrentPluginAlert {

public:

    // Public member required for alert_cast
    const static int alert_type = TORRENT_PLUGIN_STATUS_ALERT_ID;


    //const static int static_category = <bitmask of alert::category_t flags>;

    // Constructor
    TorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash
                             ,int numberOfPeers
                             ,int numberOfPeersWithExtension
                             ,bool pluginOn
                             ,int tokensReceived
                             ,int tokensSent);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<alert> clone() const;

    // Getters
    int numberOfPeers() const;
    int numberOfPeersWithExtension() const;
    bool pluginOn() const;
    int tokensReceived() const;
    int tokensSent() const;

private:

    // Number of peers torrent plugin has been connected with (not all peers on libtorrent::torrent peer list?)
    int _numberOfPeers;

    // Among numberOfPeers_, how many did a correct BEP10 handshake indicating they supported BEP43
    int _numberOfPeersWithExtension;

    // Management mode of torrent plugin
    bool _pluginOn;

    // Number of tokens received during this session
    int _tokensReceived;

    // Numbre of tokens sent during this session
    int _tokensSent;
};

#endif // TORRENT_PLUGIN_STATUS_ALERT_HPP
