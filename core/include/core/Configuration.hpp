/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 15 2016
 */

#ifndef JOYSTREAM_CORE_CONFIGURATION_HPP
#define JOYSTREAM_CORE_CONFIGURATION_HPP

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

// Persistant representation of a torrent
class Torrent {

    Torrent();

    Torrent(const libtorrent::add_torrent_params &);

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

private:

    // Basic information for adding torrent to libtorrent session
    libtorrent::add_torrent_params _params;

    // Other stuff about plugin...

    /**
    * Schema for JSON (partial) encoding of libtorrent::add_torrent_params
    {
        "ti":  <string:Bencoded torrent file, null>
        "name":         <string: Human readable torrent name>
        "save_path":     <string: Local path where data is stored to/loaded from",
        "resume_data":   <string: libtorrent fast-resume data>,
        "flags":        <int: libtorrent::add_torrent_params flags>
    }
    */

    // Encode/Decode add_torrent_params in JSON schema above
    static QJsonObject toJSON(const libtorrent::add_torrent_params &);
    static libtorrent::add_torrent_params fromJSON(const QJsonObject &);

    // Decode/Encode torrent_info to/from bencoding
    static QString toBencoding(const libtorrent::torrent_info &);
    static libtorrent::torrent_info fromBencoding(const QString &);
};

// Persistant representation of a controller
class Controller {

public:

    Controller();

    Controller(const libtorrent::entry & libtorrentSessionSettingsEntry,
              const std::pair<int, int> & portRange,
              const std::vector<std::pair<std::string, int> > & dhtRouters,
              const std::vector<Torrent> & torrents);

    /**
    * Schema for JSON encoding of configuration
    {
          "libtorrentSettings" : <..>

          "dhtRouters" : entry::list_type object with entry::list_type objects with two elements, each encoding a dht router by the host (first)
          and port (second).

          "torrents" : [...,<JSON encoding of configuration::Torrent>]
    }
    */

    // Decode from JSON using schema above
    static Controller fromJSON(const QJsonObject &);

    // Encode to JSON using schema above
    QJsonObject toJSON();

    // Loads configuration from file, encoded in JSON using schema above
    static Controller fromFile(const std::string &);

    // Saves to file
    void toFile(const std::string &);

private:

    // Session settings
    libtorrent::settings_pack settings;

    // libtorrent::dht_settings dht;

    // Dht routers
    //std::vector<std::pair<std::string, int> > _dhtRouters;

    // Torrent configurations
    std::vector<Torrent> _torrents;
};


}
}
}

#endif // JOYSTREAM_CORE_CONFIGURATION_HPP
