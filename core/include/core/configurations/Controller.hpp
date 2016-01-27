/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2016
 */

#ifndef JOYSTREAM_CORE_CONFIGURATIONS_CONTROLLER_HPP
#define JOYSTREAM_CORE_CONFIGURATIONS_CONTROLLER_HPP

#include <core/configurations/Torrent.hpp>

#include <QVector>

namespace libtorrent {
    class sha1_hash;
}

namespace joystream {
namespace core {
namespace configurations {

    /**
     * @brief Persistant state of Controller.
     */
    class Controller {

    public:

        // Constructor using default parameter settings
        Controller();

        // Destructor
        ~Controller();

        // Constructor using members
        Controller(const libtorrent::entry & libtorrentSessionSettingsEntry,
                      const std::pair<int, int> & portRange,
                      const std::vector<std::pair<std::string, int> > & dhtRouters,
                      const QVector<Torrent> & torrents);

        // Constructor using dictionary entry
        Controller(const libtorrent::entry::dictionary_type & dictionaryEntry);

        // Constructor using file
        Controller(const char * fileName);

        /**
         * Saves to dictionary entry
         * ===============================================================
         *
         * Controller parameters as they persist across sessions on disk
         * encoded as entry::dictionary_type with the following keys:
         *
         * "libtorrentSettings" -> entry::dictionary_type object from session.save_state().
         *
         * "portRange" -> entry::list_type object with two positive integers used as start (first) and end (second)
         * of port range for running client.
         *
         * "dhtRouters" -> entry::list_type object with entry::list_type objects with two elements, each encoding a dht router by the host (first)
         * and port (second).
         *
         * "torrentConfigurations" -> entry::list_type object, with list item objects being of type entry::dictionary_type and
         * representing state of corresponding torrent as dictated by encoding used in TorrentConfiguration::toDictionaryEntry().
         */
        void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry);

        // Saves to file
        void saveToFile(const char * file);

        /*
        // Inserts torrent configuration.
        // This object then takes ownership of file, and deletes in constructor
        void insertTorrentController(TorrentConfiguration * torrentConfiguration);
        */

        // Getters & Setters
        libtorrent::entry getLibtorrentSessionSettingsEntry() const;
        void setLibtorrentSessionSettingsEntry(const libtorrent::entry & libtorrentSessionSettingsEntry);

        std::pair<int, int> getPortRange() const;

        std::vector<std::pair<std::string, int> > getDhtRouters() const;

        //std::vector<TorrentConfiguration *>::const_iterator getBeginTorrentConfigurationsIterator() const;
        //std::vector<TorrentConfiguration *>::const_iterator getEndTorrentConfigurationsIterator() const;


        /*
        QMap<libtorrent::sha1_hash, QPair<Torrent, BuyerTorrentPlugin::Configuration> > buyers() const;
        void setBuyers(const QMap<libtorrent::sha1_hash, QPair<Torrent, BuyerTorrentPlugin::Configuration> > & buyers);

        QMap<libtorrent::sha1_hash, QPair<Torrent, SellerTorrentPlugin::Configuration> > sellers() const;
        void setSellers(const QMap<libtorrent::sha1_hash, QPair<Torrent, SellerTorrentPlugin::Configuration> > & sellers);
        */

        //TorrentPlugin::Configuration *getTorrentPluginController() const;
        //void setTorrentPluginController(TorrentPlugin::Configuration *value);

        QVector<Torrent> torrents() const;
        void setTorrents(const QVector<Torrent> & torrents);

        libtorrent::sha1_hash infoHash() const;
        void setInfoHash(const libtorrent::sha1_hash &infoHash);

        std::string name() const;
        void setName(const std::string &name);

        std::string savePath() const;
        void setSavePath(const std::string &savePath);

        std::vector<char> resumeData() const;
        void setResumeData(const std::vector<char> &resumeData);

        quint64 flags() const;
        void setFlags(const quint64 &flags);

        libtorrent::torrent_info *torrentInfo() const;
        void setTorrentInfo(libtorrent::torrent_info *torrentInfo);

        //const TorrentPlugin::Configuration *torrentPluginController() const;
        //void setTorrentPluginController(const TorrentPlugin::Configuration *torrentPluginConfiguration);

    private:

        /*
         * Holds all settings of session, that includes
         * session_settings, dht_settings, dht_state,
         * proxy_settings, i2p_proxy, pe_settings, feed
         * and extension settings.
         *
         * It would be cleaner to save class representations rather
         * than using entry type, however that gets messy and/or
         * requires lots of extra work for various reasons.
         */

        //// SWITCH TO THIS LATER //////////////////
        // libtorrent::settings_pack settings;
        ///////////////////////////////////////////

        libtorrent::entry _libtorrentSessionSettingsEntry;

        ///////////////////////////////////////////
        // libtorrent::dht_settings dht;
        ///////////////////////////////////////////

        // Listening port range: DO WE EVEN NEED THIS? IT MAY BE PART OF DHT_SETTINGS ENTRY?
        std::pair<int, int> _portRange;

        // Dht routers
        std::vector<std::pair<std::string, int> > _dhtRouters;

        // Torrent configurations
        QVector<Torrent> _torrents;
    };

}
}
}

#endif // JOYSTREAM_CORE_CONFIGURATIONS_CONTROLLER_HPP

