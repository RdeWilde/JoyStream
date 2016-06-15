/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2016
 */

#ifndef JOYSTREAM_CORE_CONFIGURATION_TORRENT_HPP
#define JOYSTREAM_CORE_CONFIGURATION_TORRENT_HPP

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/entry.hpp> // libtorrent::entry::dictionary_type
#include <libtorrent/torrent_info.hpp>
#include <boost/intrusive_ptr.hpp>

namespace libtorrent {
    struct TORRENT_EXPORT add_torrent_params;
}

namespace joystream {
namespace core {

    class TorrentConfiguration {

    public:

        // Default constructor
        TorrentConfiguration();

        // Constructor from members
        TorrentConfiguration(const libtorrent::sha1_hash & infoHash,
                     const std::string & name,
                     const std::string & savePath,
                     const std::vector<char> & resumeData,
                     boost::uint64_t flags,
                     const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile,
                     const std::string url);

        // Constructor from dictionary
        TorrentConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

        // Destructors
        ~TorrentConfiguration();

        static TorrentConfiguration fromTorrentFile(const std::string & file);
        static TorrentConfiguration fromMagnetLink(const std::string & magnetLink);

        /**
         * Write state into dictionary
         * ===============================================================
         *
         * Torrent parameters as they persist across sessions on disk
         * encoded as entry::dictionary_type with the following keys:
         *
         * "infoHash" -> entry::string_type object representing info hash of torrent.
         *
         * "name" -> entry::string_type object representing name of torrent.
         *
         * "savePath" -> entry::string_type object representing save_path parameter.
         *
         * "resumeData" -> entry::string_type object representing resume data.
         *
         * "flags" -> entry::integer_type object representing flag parameter.
         *
         * "torrentInfo" -> not implemented
         *
         ///* "torrentPluginConfiguration" -> entry::dictionary_type object representing _torrentPluginConfiguration as encoded by TorrentPluginConfiguration::toDictionaryEntry().
         */
        void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

        // Turn into add_torrent_params
        // will allocate new memory for resume_data pointer, which is owned by caller
        libtorrent::add_torrent_params toAddTorrentParams() const;

        // Getters

        //const TorrentPlugin::Configuration * torrentPluginConfiguration() const;
        //void setTorrentPluginConfiguration(const TorrentPlugin::Configuration *torrentPluginConfiguration);

        boost::intrusive_ptr<libtorrent::torrent_info> torrentFile() const;
        //libtorrent::torrent_info torrentInfo() const;
        //void setTorrentInfo(const libtorrent::torrent_info & torrentInfo);

        boost::uint64_t flags() const;
        void setFlags(boost::uint64_t flags);

        std::vector<char> resumeData() const;
        void setResumeData(const std::vector<char> & resumeData);

        std::string savePath() const;
        void setSavePath(const std::string & savePath);

        std::string name() const;
        void setName(const std::string & name);

        libtorrent::sha1_hash infoHash() const;
        void setInfoHash(const libtorrent::sha1_hash & infoHash);

    private:

        // Info hash of torrent
        libtorrent::sha1_hash _infoHash;

        // Name of torrent
        std::string _name; // how should this be used?

        // Save path
        std::string _savePath;

        // Resume data
        std::vector<char> _resumeData;

        // Flags
        boost::uint64_t _flags;

        // Magnet Link Uri
        std::string _magnetLink;

        // Metadata in torrent file
        // We need pointer since its not copy assignable,
        // hence does not support being set in d
        //
        // Why cant this be const again??
        boost::intrusive_ptr<libtorrent::torrent_info> _torrentFile;

        //libtorrent::torrent_info _torrentInfo;

        // Have to use pointer to get polymorphism. :/
        //const TorrentPlugin::Configuration * _torrentPluginConfiguration;
    };

}
}

#endif // JOYSTREAM_CORE_CONFIGURATIONS_TORRENT_HPP
