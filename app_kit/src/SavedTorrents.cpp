/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

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

SavedTorrents::SavedTorrents(const std::map<libtorrent::sha1_hash, core::Torrent*> &torrents) {

    for(const auto &t : torrents) {
        _torrents[t.first] = SavedTorrentParameters(t.second);
    }
}

SavedTorrents::SavedTorrents(const QJsonValue& value) {
    if(!value.isArray())
        throw std::runtime_error("expected json array of saved torrents");

    for(const QJsonValue &torrentState : value.toArray()) {
        SavedTorrentParameters ts(torrentState);

        if(_torrents.find(ts.infoHash()) != _torrents.end())
            throw std::runtime_error("duplicate torrent info hash");

        _torrents[ts.infoHash()] = ts;
    }
}

SavedTorrents::SavedTorrentsMap::size_type SavedTorrents::size() const {
    return _torrents.size();
}

void SavedTorrents::insert(const SavedTorrentParameters &params) {
    _torrents[params.infoHash()] = params;
}

QJsonValue SavedTorrents::toJson() const {

    QJsonArray torrents;

    for(const auto torrent : _torrents) {
        torrents << torrent.second.toJson();
    }

    return torrents;
}

SavedTorrents::SavedTorrentsMap SavedTorrents::torrents() const {
    return _torrents;
}

}
}
