#ifndef STARTED_BUYER_TORRENT_PLUGIN_HPP
#define STARTED_BUYER_TORRENT_PLUGIN_HPP

#include "AlertTypes.hpp"

#include "extension/BuyerTorrentPlugin.hpp"

#include <libtorrent/alert.hpp>
#include <libtorrent/sha1_hash.hpp>

class StartedBuyerTorrentPlugin : public libtorrent::alert
{
public:

    // Public member required for alert_cast
    const static int alert_type = STARTED_BUYER_TORRENT_PLUGIN_ID;

    // Default constructor
    StartedBuyerTorrentPlugin();

    // Constructor from members
    StartedBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration);

    // Copy constructor
    StartedBuyerTorrentPlugin(const StartedBuyerTorrentPlugin & alert);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<libtorrent::alert> clone() const;

    // Getters and setters
    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash & infoHash);

    BuyerTorrentPlugin::Configuration configuration() const;
    void setConfiguration(const BuyerTorrentPlugin::Configuration & configuration);

private:

    // Info hash of torrent started
    libtorrent::sha1_hash _infoHash;

    // Configuration under which plugin was started
    BuyerTorrentPlugin::Configuration _configuration;

};

#endif // STARTED_BUYER_TORRENT_PLUGIN_HPP
