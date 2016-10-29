#ifndef JOYSTREAM_APPKIT_NODESTATE_HPP
#define JOYSTREAM_APPKIT_NODESTATE_HPP

#include <core/Node.hpp>
#include <app_kit/TorrentState.hpp>

namespace joystream {
namespace appkit {

class NodeState : public std::vector<TorrentState>
{
public:
    NodeState();

    // Construct node state from an instance of Node
    NodeState(const core::Node *);

    // Construct node state from json value
    NodeState(const QJsonValue&);

    QJsonValue toJson() const;

};

}
}
#endif // JOYSTREAM_APPKIT_NODESTATE_HPP
