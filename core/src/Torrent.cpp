/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 17 2016
 */

#include <core/Torrent.hpp>
#include <core/Peer.hpp>
#include <core/TorrentPlugin.hpp>
#include <core/Exception.hpp>
#include <core/detail/detail.hpp>

Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)
Q_DECLARE_METATYPE(std::vector<char>)
Q_DECLARE_METATYPE(libtorrent::torrent_status::state_t)
Q_DECLARE_METATYPE(libtorrent::torrent_status)

namespace joystream {
namespace core {

void Torrent::registerMetaTypes() {

    Peer::registerMetaTypes();
    TorrentPlugin::registerMetaTypes();
    qRegisterMetaType<libtorrent::tcp::endpoint>();
    qRegisterMetaType<std::vector<char>>();
    qRegisterMetaType<libtorrent::torrent_status::state_t>();
}

Torrent::Torrent(const libtorrent::torrent_handle & handle,
                 const std::vector<char> & resumeData,
                 const boost::shared_ptr<extension::Plugin> & plugin)
    : _infoHash(handle.info_hash())
    , _handle(handle)
    , _resumeData(resumeData)
    , _plugin(plugin) {
}

Torrent::~Torrent() {

    for(std::map<libtorrent::tcp::endpoint, std::unique_ptr<Peer>>::const_iterator it = _peers.cbegin();it != _peers.cend();)
        removePeer(it++);

    removeTorrentPlugin();
}

void Torrent::pause(bool graceful, const TorrentPaused & handler) {
    _plugin->submit(extension::request::PauseTorrent(_handle.info_hash(), graceful, handler));
}

void Torrent::resume(const TorrentResumed & handler) {
    _plugin->submit(extension::request::ResumeTorrent(_handle.info_hash(), handler));
}

void Torrent::generateResumeData() {
    _handle.save_resume_data();
}

void Torrent::postPeerStatusUpdates() noexcept {

    // Get peer_info for peer, which unfortunately requires
    // getting it for all peers
    std::vector<libtorrent::peer_info> v;

    try {
        _handle.get_peer_info(v);
    } catch (const libtorrent::libtorrent_exception &) {
        // Handle was invalidated, drop torrent,
    }

    // Update peer statuses on torrent

    // For each connection with a status
    for(libtorrent::peer_info p: v) {

        auto it = _peers.find(p.ip);

        // if peer is present, then update
        if(it != _peers.cend())
            it->second->update(p);
    }
}

libtorrent::sha1_hash Torrent::infoHash() const noexcept {
    return _infoHash;
}

std::map<libtorrent::tcp::endpoint, Peer *> Torrent::peers() const noexcept {
    return detail::getRawMap<libtorrent::tcp::endpoint, Peer>(_peers);
}

bool Torrent::torrentPluginSet() const noexcept {
    return _torrentPlugin.get() != nullptr;
}

TorrentPlugin * Torrent::torrentPlugin() const {

    if(torrentPluginSet())
        return _torrentPlugin.get();
    else
        throw exception::HandleNotSet();
}

void Torrent::addPeer(const libtorrent::peer_info & info) {

    assert(_peers.count(info.ip) == 0);

    auto p = Peer::create(info,
                          boost::optional<extension::status::PeerPlugin>());

    _peers.insert(std::make_pair(info.ip, std::unique_ptr<Peer>(p)));

    emit peerAdded(p);
}

void Torrent::removePeer(const libtorrent::tcp::endpoint & ip) {

    auto it = _peers.find(ip);
    assert(it != _peers.end());

    removePeer(it);
}

void Torrent::removePeer(std::map<libtorrent::tcp::endpoint, std::unique_ptr<Peer>>::const_iterator it) {

    libtorrent::tcp::endpoint endPoint = it->first;

    _peers.erase(it);

    emit peerRemoved(endPoint);
}

void Torrent::addTorrentPlugin(const extension::status::TorrentPlugin & status) {

    assert(_torrentPlugin.get() == nullptr);

    TorrentPlugin * plugin = TorrentPlugin::create(status, _plugin);

    _torrentPlugin.reset(plugin);

    emit torrentPluginAdded(plugin);
}

void Torrent::removeTorrentPlugin() {

    assert(_torrentPlugin.get() != nullptr);

    _torrentPlugin.release();

    emit removeTorrentPlugin();
}

void Torrent::setResumeDataGenerationResult(const std::vector<char> & resumeData) {

    _resumeData = resumeData;

    emit resumeDataGenerationCompleted(resumeData);
}

/*
void Torrent::addStream(Stream * stream) {
    _streams.insert(stream);
}

void Torrent::removeStream(Stream * stream) {
    _streams.remove(stream);
}

void Torrent::pieceRead(const boost::shared_array<char> & buffer,
                                    int pieceIndex,
                                    int size) {

    // Iterate streams and notify them
    for(QSet<Stream *>::iterator i = _streams.begin(),
        end = _streams.end();
        i != end;i++)
        (*i)->pieceRead(buffer, pieceIndex, size);
}

void Torrent::pieceFinished(int piece) {

    // Iterate streams and notify them
    for(QSet<Stream *>::iterator i = _streams.begin(),
        end = _streams.end();
        i != end;i++)
        (*i)->pieceDownloaded(piece);
}
*/

}
}
