/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 15 2016
 */

#ifndef JOYSTREAM_CORE_CONFIGURATION_HPP
#define JOYSTREAM_CORE_CONFIGURATION_HPP

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/entry.hpp> // libtorrent::entry::dictionary_type
#include <libtorrent/torrent_info.hpp>
#include <boost/intrusive_ptr.hpp>

class QJsonObject;

namespace libtorrent {
    struct TORRENT_EXPORT add_torrent_params;
}

namespace joystream {
namespace core {
namespace configuration {

    // Persistant representation of a torrent
    struct Torrent {

    public:

        Torrent();

        Torrent(const libtorrent::torrent_info &,
                const std::string & name,
                const std::string & savePath,
                const std::vector<char> & resumeData,
                boost::uint64_t flags);

        // Load torrent information based on magnet URI
        static libtorrent::torrent_info fromMagnetUri(const std::string &);

        // Load torrent information from torrent file
        static libtorrent::torrent_info fromTorrentFile(const std::string &);

        /**
         * Schema for JSON encoding of configuration
        {
            "torrentInfo": {
                            "description": "classic info_hash of torrent",
                            "type" : "string",
                            "encoding" : "hex"
            },
            "name" : {},
            "savePath" : {},
            "resumeData" : {},
            "flags" : {}
        }
         */

        // Decode from JSON using schema above
        static Torrent fromJSON(const QJsonObject &);

        // Encode to JSON using schema above
        QJsonObject toJSON() const;

        // Turn into add_torrent_params
        libtorrent::add_torrent_params toParams() const;

        //// Member variables

        // Information about torrent
        libtorrent::torrent_info _torrentInfo;

        // Name of torrent
        std::string _name;

        // Save path
        std::string _savePath;

        // Resume data
        std::vector<char> _resumeData;

        // Flags
        boost::uint64_t _flags;
    };

    // Persistant representation of a controller
    struct Controller {

        Controller();

        Controller(const libtorrent::entry & libtorrentSessionSettingsEntry,
                  const std::pair<int, int> & portRange,
                  const std::vector<std::pair<std::string, int> > & dhtRouters,
                  const std::vector<Torrent> & torrents);

        /**
         * Schema for JSON encoding of configuration
         {
              "libtorrentSettings" : entry::dictionary_type object from session.save_state().

              "portRange" : entry::list_type object with two positive integers used as start (first) and end (second)
              of port range for running client.

              "dhtRouters" : entry::list_type object with entry::list_type objects with two elements, each encoding a dht router by the host (first)
              and port (second).

              "torrentConfigurations" : entry::list_type object, with list item objects being of type entry::dictionary_type and
              representing state of corresponding torrent as dictated by encoding used in TorrentConfiguration::toDictionaryEntry().
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

        //// Member variables

        // Session settings
        libtorrent::settings_pack settings;

        // libtorrent::dht_settings dht;

        // Dht routers
        std::vector<std::pair<std::string, int> > _dhtRouters;

        // Torrent configurations
        std::vector<Torrent> _torrents;
    };


}
}
}

#endif // JOYSTREAM_CORE_CONFIGURATION_HPP
