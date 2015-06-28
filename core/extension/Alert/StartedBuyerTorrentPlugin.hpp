/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

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
    StartedBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash,
                              const BuyerTorrentPlugin::Configuration & configuration,
                              const UnspentP2PKHOutput & utxo,
                              const BuyerTorrentPlugin::Status & status);

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

    UnspentP2PKHOutput utxo() const;
    void setUtxo(const UnspentP2PKHOutput & utxo);

    BuyerTorrentPlugin::Status status() const;
    void setStatus(const BuyerTorrentPlugin::Status & status);

private:

    // Info hash of torrent started
    libtorrent::sha1_hash _infoHash;

    // Configuration under which plugin was started
    BuyerTorrentPlugin::Configuration _configuration;

    // Funding utxo
    UnspentP2PKHOutput _utxo;

    // Starting status
    BuyerTorrentPlugin::Status _status;

};

#endif // STARTED_BUYER_TORRENT_PLUGIN_HPP
