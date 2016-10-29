#include <app_kit/NodeState.hpp>
#include <app_kit/TorrentState.hpp>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace joystream {
namespace appkit {

NodeState::NodeState()
{

}

NodeState::NodeState(const core::Node* node) {

    for(const auto &t : node->torrents()) {
        this->push_back(TorrentState(t.second));
    }
}

NodeState::NodeState(const QJsonValue& value) {

    for(const QJsonValue &torrentState : value.toArray()) {
        this->push_back(TorrentState(torrentState));
    }
}

QJsonValue NodeState::toJson() const {

    QJsonArray torrents;

    for(const TorrentState & torrentState: *this) {
        torrents << torrentState.toJson();
    }

    return torrents;
}
}
}
