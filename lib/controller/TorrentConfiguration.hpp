#ifndef TORRENT_CONFIGURATION_HPP
#define TORRENT_CONFIGURATION_HPP

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/entry.hpp> // because you cant forward declare typedefs (libtorrent::entry::dictionary_type)
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/add_torrent_params.hpp>

#include <QtGlobal> // quint64

//class TorrentPluginConfiguration;

#include "extension/TorrentPlugin.hpp" // TorrentPlugin::Configuration

class TorrentConfiguration {

public:

    // Default constructor
    //TorrentConfiguration();

    // Constructor from members
    TorrentConfiguration(const libtorrent::sha1_hash & infoHash,
                         const std::string & name,
                         const std::string & savePath,
                         const std::vector<char> & resumeData,
                         quint64 flags,
                         libtorrent::torrent_info * torrentInfo,
                         const TorrentPlugin::Configuration * configuration);

    // Constructor from dictionary
    TorrentConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    // Destructors
    ~TorrentConfiguration();

    /*
    // Assignment operator, needed for adding type by value to map
    TorrentConfiguration & operator=(const TorrentConfiguration & rhs);
    */

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
     * "torrentPluginConfiguration" -> entry::dictionary_type object representing _torrentPluginConfiguration as encoded by TorrentPluginConfiguration::toDictionaryEntry().
     */
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

    // Turn into add_torrent_params
    // will allocate new memory for resume_data pointer, which is owned by caller
    libtorrent::add_torrent_params toAddTorrentParams() const;

    // Getters
    const libtorrent::sha1_hash & getInfoHash() const;
    const std::string & getName() const;
    const std::string & getSavePath() const;
    const std::vector<char> & getResumeData() const;
    quint64 getFlags() const;
    const libtorrent::torrent_info * getTorrentInfo() const;

    //const TorrentPluginConfiguration * getTorrentPluginConfiguration() const;
    //void setTorrentPluginConfiguration(const TorrentPluginConfiguration & torrentPluginConfiguration);

protected:

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

    // Metadata in torrent file
    // We need pointer since we cannot copy torrent_info
    libtorrent::torrent_info * _torrentInfo;

    // Torrent plugin configuration
    const TorrentPlugin::Configuration * _configuration;
};

#endif // TORRENT_CONFIGURATION_HPP
