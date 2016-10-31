#ifndef JOYSTREAM_APPKIT_NODESTATE_HPP
#define JOYSTREAM_APPKIT_NODESTATE_HPP

#include <core/Node.hpp>
#include <app_kit/TorrentState.hpp>
#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace appkit {

class NodeState
{
public:
    NodeState();

    // Construct node state from an instance of Node
    NodeState(const core::Node *);

    // Construct node state from json value
    NodeState(const QJsonValue&);

    QJsonValue toJson() const;

private:
    std::map<libtorrent::sha1_hash, TorrentState> _torrents;
};

}
}
#endif // JOYSTREAM_APPKIT_NODESTATE_HPP
