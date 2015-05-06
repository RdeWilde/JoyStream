#ifndef SELLER_PEER_PLUGIN_STARTED_ALERT_HPP
#define SELLER_PEER_PLUGIN_STARTED_ALERT_HPP

#include "TorrentPluginAlert.hpp"
#include "AlertTypes.hpp"
#include "extension/SellerTorrentPlugin.hpp"

class SellerPeerPluginStartedAlert : public TorrentPluginAlert
{
public:

    // Public member required for alert_cast
    const static int alert_type = SELLER_PEER_PLUGIN_STARTED_ALERT_ID;

    // Constructor
    SellerPeerPluginStartedAlert(const libtorrent::sha1_hash & infoHash, const SellerTorrentPlugin::Configuration & configuration);

    // Copy constructor
    SellerPeerPluginStartedAlert(const SellerPeerPluginStartedAlert & alert);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<libtorrent::alert> clone() const;

    // Getters and setters
    SellerTorrentPlugin::Configuration configuration() const;
    void setConfiguration(const SellerTorrentPlugin::Configuration & configuration);

private:

    // Configuration under which peer was added
    SellerTorrentPlugin::Configuration _configuration;
};

#endif // SELLER_PEER_PLUGIN_STARTED_ALERT_HPP
