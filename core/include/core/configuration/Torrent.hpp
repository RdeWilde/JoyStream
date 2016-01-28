/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2016
 */

#ifndef JOYSTREAM_CORE_CONFIGURATION_TORRENT_HPP
#define JOYSTREAM_CORE_CONFIGURATION_TORRENT_HPP

#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace core {
namespace configuration {

    /**
     * @brief Persistant state of Torrent.
     */
    class Torrent {

    public:

        // Default constructor
        Torrent();

        // Constructor from members
        Torrent(const libtorrent::sha1_hash & infoHash,
                     const std::string & name,
                     const std::string & savePath,
                     const std::vector<char> & resumeData,
                     quint64 flags,
                     //const libtorrent::torrent_info & torrentInfo
                     const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile,
                     const std::string url);
                     //const TorrentPlugin::Configuration * torrentPluginConfiguration);

        // Constructor from dictionary
        Torrent(const libtorrent::entry::dictionary_type & dictionaryEntry);

        // Destructors
        ~Torrent();

        static Torrent fromTorrentFile(const QString & resource);
        static Torrent fromMagnetLink(const QString & resource);

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

        quint64 flags() const;
        void setFlags(quint64 flags);

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
        quint64 _flags;

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
}

#endif // JOYSTREAM_CORE_CONFIGURATIONS_TORRENT_HPP

