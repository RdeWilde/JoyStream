#ifndef JOYSTREAM_APPKIT_NODESTATE_HPP
#define JOYSTREAM_APPKIT_NODESTATE_HPP

#include <core/Node.hpp>
#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace appkit {

class SavedTorrentParameters;

class SavedTorrents
{
public:
    SavedTorrents();

    // Construct node state from an instance of Node
    SavedTorrents(const core::Node *);

    // Construct node state from json value
    SavedTorrents(const QJsonValue&);

    QJsonValue toJson() const;

    std::map<libtorrent::sha1_hash, SavedTorrentParameters> torrents() const;

private:
    std::map<libtorrent::sha1_hash, SavedTorrentParameters> _torrents;
};

}
}
#endif // JOYSTREAM_APPKIT_NODESTATE_HPP
