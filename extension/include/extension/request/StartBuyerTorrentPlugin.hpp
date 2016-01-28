/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef START_BUYER_TORRENT_PLUGIN_HPP
#define START_BUYER_TORRENT_PLUGIN_HPP

#include <core/extension/Request/PluginRequest.hpp>
#include <core/extension/BuyerTorrentPlugin.hpp>
#include <common/UnspentP2PKHOutput.hpp>

#include <libtorrent/sha1_hash.hpp>

class StartBuyerTorrentPlugin : public PluginRequest
{
public:

    // Constructor
    StartBuyerTorrentPlugin();

    // Constructor from members
    StartBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration configuration, const Coin::UnspentP2PKHOutput & utxo);

    // Returns request type
    virtual PluginRequestType getPluginRequestType() const;

    // Getters
    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash & infoHash);

    BuyerTorrentPlugin::Configuration configuration() const;
    void setConfiguration(const BuyerTorrentPlugin::Configuration & configuration);

    Coin::UnspentP2PKHOutput utxo() const;
    void setUtxo(const Coin::UnspentP2PKHOutput & utxo);

private:

    // Torrent info_hash
    libtorrent::sha1_hash _infoHash;

    // Buyer torrent plugin configuration
    BuyerTorrentPlugin::Configuration _configuration;

    // Utxo funds buyer torrent plugin
    Coin::UnspentP2PKHOutput _utxo;
};

#endif // START_BUYER_TORRENT_PLUGIN_HPP
