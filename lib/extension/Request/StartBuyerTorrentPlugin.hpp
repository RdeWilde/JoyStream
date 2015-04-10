#ifndef START_BUYER_TORRENT_PLUGIN_HPP
#define START_BUYER_TORRENT_PLUGIN_HPP

#include "PluginRequest.hpp"
#include <libtorrent/sha1_hash.hpp>

#include "extension/BuyerTorrentPlugin.hpp"
#include "extension/BitCoin/UnspentP2PKHOutput.hpp"

class StartBuyerTorrentPlugin : public PluginRequest
{
public:

    // Constructor
    StartBuyerTorrentPlugin();

    // Constructor from members
    StartBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration configuration, const UnspentP2PKHOutput & utxo);

    // Returns request type
    virtual PluginRequestType getPluginRequestType() const;

    // Getters
    libtorrent::sha1_hash infoHash() const;
    void setInfoHash(const libtorrent::sha1_hash & infoHash);

    BuyerTorrentPlugin::Configuration configuration() const;
    void setConfiguration(const BuyerTorrentPlugin::Configuration & configuration);

    UnspentP2PKHOutput utxo() const;
    void setUtxo(const UnspentP2PKHOutput & utxo);

private:

    // Torrent info_hash
    libtorrent::sha1_hash _infoHash;

    // Buyer torrent plugin configuration
    BuyerTorrentPlugin::Configuration _configuration;

    // Utxo funds buyer torrent plugin
    UnspentP2PKHOutput _utxo;
};

#endif // START_BUYER_TORRENT_PLUGIN_HPP
