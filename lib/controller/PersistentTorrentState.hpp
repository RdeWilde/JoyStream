#ifndef PERSISTENT_TORRENT_STATE_HPP
#define PERSISTENT_TORRENT_STATE_HPP

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/entry.hpp>

#include <QtGlobal> // quint64

#include <vector>

/*
 * State of the controller as it persists across sessions on disk
 * ===============================================================
 *
 * Is encoded as entry::dictionary_type with the following keys:
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

class PersistentTorrentState
{
public:

    // Default constructor
    PersistentTorrentState();

    // Constructor from fields
    PersistentTorrentState(const libtorrent::sha1_hash & info_hash, const std::string & name, const std::string & save_path, const std::vector<char> & resume_data, quint64 flags);

    // Constructor from dictionary
    PersistentTorrentState(const libtorrent::entry::dictionary_type & dictionaryEntry);

    // Assignment operator needed for adding type by value to map
    PersistentTorrentState & operator=(const PersistentTorrentState & rhs);

    // Write state into dictionary
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

    // Getters
    const libtorrent::sha1_hash & getInfoHash() const;
    const std::string & getName() const;
    const std::string & getSavePath() const;
    std::vector<char> & getResumeData();
    const std::vector<char> & PersistentTorrentState::getConstResumeData() const;
    quint64 getFlags() const;

private:

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

#endif
