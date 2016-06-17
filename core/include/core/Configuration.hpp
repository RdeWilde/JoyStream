/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 15 2016
 */

#ifndef JOYSTREAM_CORE_CONFIGURATION_HPP
#define JOYSTREAM_CORE_CONFIGURATION_HPP

#include <extension/extension.hpp>

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_info.hpp>

class QJsonObject;
class QString;
class QJsonValue;

namespace libtorrent {
    struct TORRENT_EXPORT add_torrent_params;
}

namespace joystream {
namespace core {
namespace configuration {

template <class T>
QJsonValue safeRead<T>(const QJsonObject & json, const QString & key);

struct BuyingPlugin {

    BuyingPlugin():{}

    BuyingPlugin() {}

    protocol_session::BuyingPolicy policy;
    protocol_wire::BuyerTerms terms;
};

struct SellingPlugin {

    SellingPlugin():{}

    SellingPlugin()
        : {}

    protocol_session::SellingPolicy policy;
    protocol_wire::SellerTerms terms;
};

class TorrentPlugin {

public:

    TorrentPlugin();

    TorrentPlugin inBuyMode(const protocol_session::BuyingPolicy & policy,
                            const protocol_wire::BuyerTerms & terms);

    TorrentPlugin inSellMode(const protocol_session::SellingPolicy & policy,
                             const protocol_wire::SellerTerms & terms);

    /**
    * Schema for JSON encoding of configuration
    {
        "mode" : <string: one among "buying", "selling", "not-set", "observing">
        "buying": <object: >,
        "selling":<object: >
        "constraint": "mode" == "buying" <==> "buying" is the only other key,
                      "mode" == "selling" <==> "selling" is the only other key
    }
    */

    // Encode/Decode configuration from JSON using schema above
    QJsonObject toJSON() const;
    static Torrent fromJSON(const QJsonObject &);

private:

    protocol_session::SessionMode _mode;

    // _mode == SessionMode::observing, not_set
    // no

    // _mode == SessionMode::buying
    BuyingPlugin buying;

    // _mode == SessionMode::selling
    SellingPlugin selling;
};

// Persistant representation of a torrent
class Torrent {

public:

    Torrent();

    Torrent(const libtorrent::add_torrent_params &,
            const TorrentPlugin &);

    /**
    // Load torrent information based on magnet URI
    static libtorrent::torrent_info fromMagnetUri(const std::string &);

    // Load torrent information from torrent file
    static libtorrent::torrent_info fromTorrentFile(const std::string &);
    */

    /**
    * Schema for JSON encoding of configuration
    {
        "params": <string: encoding libtorrent::add_torrent_params>
    }
    */

    // Encode/Decode configuration from JSON using schema above
    QJsonObject toJSON() const;
    static Torrent fromJSON(const QJsonObject &);

    // Adding parameters
    libtorrent::add_torrent_params params() const;

private:

    // Basic information for adding torrent to libtorrent session
    libtorrent::add_torrent_params _params;

    // Other stuff about plugin...
    TorrentPlugin _plugin;

    /**
    * Schema for JSON (partial) encoding of libtorrent::add_torrent_params
    {
        "ti":           <string:Bencoded torrent file, null>
        "name":         <string: Human readable torrent name>
        "save_path":    <string: Local path where data is stored to/loaded from",
        "resume_data":  <string: libtorrent fast-resume data>,
        "flags":        <int: libtorrent::add_torrent_params flags>
    }
    */

    // Encode/Decode add_torrent_params in JSON schema above
    static QJsonObject toJSON(const libtorrent::add_torrent_params &);
    static libtorrent::add_torrent_params fromJSON(const QJsonObject &);

    // Decode/Encode torrent_info to/from bencoding
    static QString bencode(const libtorrent::torrent_info &);
    static libtorrent::torrent_info deBencode(const QString &);
};

// Persistant representation of a node
class Node {

public:

    Node();

    Node(const libtorrent::settings_pack & settingPack,
               const libtorrent::dht_settings & dhtSettings,
               const std::pair<int, int> & portRange,
               const std::vector<std::pair<std::string, int> > & dhtRouters);

    /**
    * Schema for JSON encoding of configuration
    {
          "settings_pack": <string: bencoding as dictate by libtorrent::save_settings_to_dict>,
          "dht_settings": <...>, <--- dropped?
          "torrents": [..., <JSON encoding of configuration::Torrent>, ...]
    }
    */

    // Decode from JSON using schema above
    static Node fromJSON(const QJsonObject &);

    // Encode to JSON using schema above
    QJsonObject toJSON();

    // Loads configuration from file, encoded in JSON using schema above
    static Node fromFile(const std::string &);

    // Saves to file
    void toFile(const std::string &);

private:

    // Session settings
    libtorrent::settings_pack _settingsPack;

    // DHT settings
    libtorrent::dht_settings _dhtSettings;

    // Dht routers
    //std::vector<std::pair<std::string, int> > _dhtRouters;


    // Decode/Encode libtorrent::settings_pack to/from bencoding
    static QString toJSON(const libtorrent::settings_pack & settings);
    libtorrent::settings_pack fromJSON(const QString &);

    // Decode/Encode libtorrent::dht_settings to/from bencoding
    static QString toJSON(const libtorrent::dht_settings & settings);
    libtorrent::dht_settings fromJSON(const QString &);
};

}
}
}

#endif // JOYSTREAM_CORE_CONFIGURATION_HPP
