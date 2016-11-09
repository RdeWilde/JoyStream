#include <app_kit/SavedTorrents.hpp>
#include <app_kit/SavedTorrentParameters.hpp>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace joystream {
namespace appkit {

SavedTorrents::SavedTorrents()
{

}

SavedTorrents::SavedTorrents(const core::Node* node) {

    for(const auto &t : node->torrents()) {
        _torrents[t.second->infoHash()] = SavedTorrentParameters(t.second);
    }
}

SavedTorrents::SavedTorrents(const QJsonValue& value) {
    if(!value.isArray())
        throw std::runtime_error("expected json array of torrent states");

    for(const QJsonValue &torrentState : value.toArray()) {
        SavedTorrentParameters ts(torrentState);
        auto ti = ts.metaData();
        if(ti) {
            if(_torrents.find(ti->info_hash()) != _torrents.end())
                throw std::runtime_error("duplicate torrent info hash");

            _torrents[ti->info_hash()] = ts;
        }
    }
}

QJsonValue SavedTorrents::toJson() const {

    QJsonArray torrents;

    for(const auto torrent : _torrents) {
        torrents << torrent.second.toJson();
    }

    return torrents;
}

std::map<libtorrent::sha1_hash, SavedTorrentParameters> SavedTorrents::torrents() const {
    return _torrents;
}

}
}
