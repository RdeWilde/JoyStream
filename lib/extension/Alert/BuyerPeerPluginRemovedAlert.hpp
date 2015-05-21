#ifndef BUYER_PEER_PLUGIN_REMOVED_ALERT_HPP
#define BUYER_PEER_PLUGIN_REMOVED_ALERT_HPP

#include "TorrentPluginAlert.hpp"
#include "AlertTypes.hpp"
#include "extension/BuyerPeerPlugin.hpp"

class BuyerPeerPluginRemovedAlert : public TorrentPluginAlert
{
public:

    // Public member required for alert_cast
    const static int alert_type = BUYER_PEER_PLUGIN_REMOVED_ALERT_ID;

    // Constructor from members
    BuyerPeerPluginRemovedAlert(const libtorrent::sha1_hash & infoHash,
                                const libtorrent::tcp::endpoint & endPoint);

    // Copy Constructor
    BuyerPeerPluginRemovedAlert(const BuyerPeerPluginRemovedAlert & alert);

    // Virtual routines from libtrorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<libtorrent::alert> clone() const;

    // Getters and setters
    libtorrent::tcp::endpoint endPoint() const;

private:

    // Endpoint for peer started
    libtorrent::tcp::endpoint _endPoint;

    // SOME SORT OF REASON HERE?
    // ** if nothing meaningful beyond a simple
    // buyer peer plugin status is required, then
    // this alert becomes identical with the added
    // alert, in which case one can just combined them
    // and add some information about if peer is being added
    // or removed

};

#endif // BUYER_PEER_PLUGIN_REMOVED_ALERT_HPP
