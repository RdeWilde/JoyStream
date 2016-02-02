/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef EXTENSION_STARTED_BUYER_TORRENT_PLUGIN_HPP
#define EXTENSION_STARTED_BUYER_TORRENT_PLUGIN_HPP

#include <extension/alert/AlertTypes.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <extension/BuyerTorrentPluginConfiguration.hpp>
#include <extension/BuyerTorrentPluginStatus.hpp>

#include <libtorrent/alert.hpp>
#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace extension {
namespace alert {


    class StartedBuyerTorrentPlugin : public libtorrent::alert {

    public:

        // Public member required for alert_cast
        const static int alert_type = STARTED_BUYER_TORRENT_PLUGIN_ID;

        // Default constructor
        StartedBuyerTorrentPlugin();

        // Constructor from members
        StartedBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash,
                                  const BuyerTorrentPluginConfiguration & configuration,
                                  const Coin::UnspentP2PKHOutput & utxo,
                                  const BuyerTorrentPluginStatus & status);

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

        BuyerTorrentPluginConfiguration configuration() const;
        void setConfiguration(const BuyerTorrentPluginConfiguration & configuration);

        Coin::UnspentP2PKHOutput utxo() const;
        void setUtxo(const Coin::UnspentP2PKHOutput & utxo);

        BuyerTorrentPluginStatus status() const;
        void setStatus(const BuyerTorrentPluginStatus & status);

    private:

        // Info hash of torrent started
        libtorrent::sha1_hash _infoHash;

        // Configuration under which plugin was started
        BuyerTorrentPluginConfiguration _configuration;

        // Funding utxo
        Coin::UnspentP2PKHOutput _utxo;

        // Starting status
        BuyerTorrentPluginStatus _status;

    };

}
}
}

#endif // EXTENSION_STARTED_BUYER_TORRENT_PLUGIN_HPP
