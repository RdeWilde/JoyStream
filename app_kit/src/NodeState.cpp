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
        _torrents[t.second->infoHash()] = TorrentState(t.second);
    }
}

NodeState::NodeState(const QJsonValue& value) {
    if(!value.isArray())
        throw std::runtime_error("expected json array of torrent states");

    for(const QJsonValue &torrentState : value.toArray()) {
        TorrentState ts(torrentState);
        auto ti = ts.metaData();
        if(ti) {
            if(_torrents.find(ti->info_hash()) != _torrents.end())
                throw std::runtime_error("duplicate torrent info hash");

            _torrents[ti->info_hash()] = ts;
        }
    }
}

QJsonValue NodeState::toJson() const {

    QJsonArray torrents;

    for(const auto torrent : _torrents) {
        torrents << torrent.second.toJson();
    }

    return torrents;
}
}
}
