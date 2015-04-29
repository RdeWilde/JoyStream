#ifndef BUYER_TORRENT_PLUGIN_STATUS_ALERT_HPP
#define BUYER_TORRENT_PLUGIN_STATUS_ALERT_HPP

#include "TorrentPluginAlert.hpp"
#include "AlertTypes.hpp"
#include "extension/BuyerTorrentPlugin.hpp"

class BuyerTorrentPluginStatusAlert : public TorrentPluginAlert
{
public:

    // Public member required for alert_cast
    const static int alert_type = BUYER_TORRENT_PLUGIN_STATUS_ALERT_ID;

    // Constructor from members
    BuyerTorrentPluginStatusAlert(const libtorrent::sha1_hash & infoHash, BuyerTorrentPlugin::Status status);

    // Copy constructor
    BuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert & alert);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<alert> clone() const;

    // Getters and Setters
    BuyerTorrentPlugin::Status status() const;
    void setStatus(const BuyerTorrentPlugin::Status & status);

private:

    // Plugin status
    BuyerTorrentPlugin::Status _status;
};

#endif // BUYER_TORRENT_PLUGIN_STATUS_ALERT_HPP
