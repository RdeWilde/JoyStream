/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 17 2016
 */

#include <core/Torrent.hpp>
#include <core/TorrentPlugin.hpp>
#include <core/Exception.hpp>

namespace joystream {
namespace core {

Torrent::Torrent(const libtorrent::torrent_handle & handle,
                 const libtorrent::torrent_status & status,
                 const std::vector<char> & resumeData,
                 int uploadLimit,
                 int downloadLimit,
                 const boost::shared_ptr<extension::Plugin> & plugin)
    : _plugin(plugin)
    , _handle(handle)
    , _status(status)
    , _resumeData(resumeData)
    , _uploadLimit(uploadLimit)
    , _downloadLimit(downloadLimit) {
}

void Torrent::paused(bool graceful, const TorrentPaused & handler) {
    _plugin->submit(extension::request::PauseTorrent(infoHash(), graceful, handler));
}

void Torrent::resumed(const TorrentResumed & handler) {
    _plugin->submit(extension::request::ResumeTorrent(infoHash(), handler));
}

void Torrent::generateResumeData() {
    _handle.save_resume_data();
}

libtorrent::sha1_hash Torrent::infoHash() const noexcept {
    return _status.info_hash;
}

std::map<libtorrent::tcp::endpoint, std::shared_ptr<Peer>> Torrent::peers() const noexcept {
    return _peers;
}

std::shared_ptr<TorrentPlugin> Torrent::torrentPlugin() const noexcept {
    return _torrentPlugin;
}

libtorrent::torrent_status::state_t Torrent::state() const noexcept {
    return _status.state;
}

std::string Torrent::savePath() const noexcept {
    return _status.save_path;
}

std::string Torrent::name() const noexcept {
    return _status.name;
}

std::vector<char> Torrent::resumeData() const noexcept {
    return _resumeData;
}

boost::weak_ptr<const libtorrent::torrent_info> Torrent::metaData() const noexcept {
    return _status.torrent_file;
}

float Torrent::progress() const noexcept {
    return _status.progress;
}

int Torrent::downloadRate() const noexcept {
    return _status.download_rate;
}

int Torrent::uploadRate() const noexcept {
    return _status.upload_rate;
}

bool Torrent::isPaused() const noexcept {
    return _status.paused;
}

int Torrent::uploadLimit() const noexcept {
    return _uploadLimit;
}

int Torrent::downloadLimit() const noexcept {
    return _downloadLimit;
}

void Torrent::addPeer(const libtorrent::peer_info & info) {

    assert(_peers.count(info.ip) == 0);

    std::shared_ptr<Peer> plugin(new Peer(info));

    _peers.insert(std::make_pair(info.ip, plugin));

    emit addPeer(info);
}

void Torrent::removePeer(const libtorrent::tcp::endpoint & ip) {

    // Make sure peer exists, and then drop it
    auto it = _peers.find(ip);
    assert(it != _peers.cend());

    _peers.erase(it);

    emit removePeer(ip);
}

void Torrent::addTorrentPlugin(const extension::status::TorrentPlugin & status) {

    assert(!_torrentPlugin);

    _torrentPlugin = std::shared_ptr<TorrentPlugin>(new TorrentPlugin(status, _plugin));

    emit torrentPluginAdded(_torrentPlugin);
}

void Torrent::removeTorrentPlugin() {

    assert(_torrentPlugin);

    _torrentPlugin = nullptr;

    emit removeTorrentPlugin();
}

void Torrent::updateStatus(const libtorrent::torrent_status & status) {

    assert(_status.info_hash == status.info_hash);

    if(_status.state != status.state)
        emit stateChanged(status.state);

    if(_status.progress != status.progress)
        emit progressChanged(status.progress);

    if(_status.total_download != status.total_download)
        emit downloadRateChanged(status.total_download);

    if(_status.total_upload != status.total_upload)
        emit uploadRateChanged(status.total_upload);

    if(_status.paused != status.paused)
        emit pausedChanged(status.paused);

    _status = status;
}

void Torrent::updatePeerStatuses(const std::vector<libtorrent::peer_info> & v) {

    // We create a tempoary endpoint->peer_info map from v vector,
    // and use it for endpoint based lookups when checking for missing peers
    std::map<libtorrent::tcp::endpoint, libtorrent::peer_info> peerToStatus;

    // for each connection with a status
    for(libtorrent::peer_info p: v) {

        auto it = _peers.find(p.ip);

        // if peer is present, then update
        if(it != _peers.cend())
            it->second->update(p);
        else // otherwise add
            addPeer(p);

        // add to mapping
        peerToStatus.insert(std::make_pair(p.ip, p));
    }

    // for each exisiting peer
    for(auto p: _peers) {

        // if there is no status for it, then remove
        if(peerToStatus.count(p.first) == 0)
            removePeer(p.first);
    }

}

void Torrent::updateTorrentPluginStatus(const extension::status::TorrentPlugin & status) {

    assert(status.infoHash == status.infoHash);

    _torrentPlugin->update(status);
}

void Torrent::updateUploadLimit(int uploadLimit) {

    if(_uploadLimit != uploadLimit)
        emit uploadLimitChanged(uploadLimit);

    _uploadLimit = uploadLimit;
}

void Torrent::updateDownloadLimit(int downloadLimit) {

    if(_downloadLimit != downloadLimit)
        emit downloadLimitChanged(downloadLimit);

    _downloadLimit = downloadLimit;
}

void Torrent::updatePaused(bool paused) {

    if(_status.paused != paused) {

        emit pausedChanged(paused);

        _status.paused = paused;
    }
}

void Torrent::setResumeDataGenerationResult(const std::vector<char> & resumeData) {

    _resumeData = resumeData;

    emit resumeDataGenerationCompleted(resumeData);
}

void Torrent::setMetadata(const boost::shared_ptr<const libtorrent::torrent_info> & torrent_info) {

    // If there already is valid metadata, then we are done
    if(_status.torrent_file.lock())
        return;

    _status.torrent_file = torrent_info;

    emit metadataReady();
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
