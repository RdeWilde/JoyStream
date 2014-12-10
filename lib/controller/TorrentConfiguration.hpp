#ifndef TORRENT_CONFIGURATION_HPP
#define TORRENT_CONFIGURATION_HPP

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/entry.hpp>

#include <QtGlobal> // quint64

#include <vector>

class TorrentConfiguration {

public:

    // Constructor from fields
    TorrentConfiguration(const libtorrent::sha1_hash & info_hash, const std::string & name, const std::string & save_path, const std::vector<char> & resume_data, quint64 flags);

    // Constructor from dictionary
    TorrentConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    // Assignment operator needed for adding type by value to map
    //TorrentConfiguration & operator=(const TorrentConfiguration & rhs);

    /**
     * Write state into dictionary
     * ===============================================================
     *
     * Torrent parameters as they persist across sessions on disk
     * encoded as entry::dictionary_type with the following keys:
     *
     * "info_hash" -> entry::string_type object representing info hash of torrent.
     *
     * "name" -> entry::string_type object representing name of torrent.
     *
     * "save_path" -> entry::string_type object representing save_path parameter.
     *
     * "resume_data" -> entry::string_type object representing resume data.
     *
     * "flags" -> entry::integer_type object representing flag parameter.
     */
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

    // Getters
    const libtorrent::sha1_hash & getInfoHash() const;
    const std::string & getName() const;
    const std::string & getSavePath() const;
    std::vector<char> & getResumeData();
    const std::vector<char> & getConstResumeData() const;
    quint64 getFlags() const;

protected:

    // Info hash of torrent
    libtorrent::sha1_hash info_hash_;

    // Name of torrent
    std::string name_;

    // Save path
    std::string save_path_;

    // Resume data
    std::vector<char> resume_data_;

    // Flags
    quint64 flags_;
};

#endif // TORRENT_CONFIGURATION_HPP
