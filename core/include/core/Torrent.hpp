/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 17 2016
 */

#ifndef JOYSTREAM_CORE_TORRENT_HPP
#define JOYSTREAM_CORE_TORRENT_HPP

#include <extension/extension.hpp>
#include <core/Node.hpp>
#include <core/Peer.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <QObject>
#include <memory>

namespace joystream {
namespace core {

class Peer;
class TorrentPlugin;

class Torrent : public QObject {

    Q_OBJECT

private:

    Torrent(const libtorrent::torrent_status &,
            int uploadLimit,
            int downloadLimit,
            const boost::shared_ptr<extension::Plugin> &);

public:

    /// Actions

    /**
     * @brief All connections are ended
     * @param graceful whether regular bittorrent, i.e. non-plugin, pieces in inbound transit will be completed before closing a connection.
     * @param handler callback handler when pause oeration has been completed
     */
    void paused(bool graceful, const TorrentPaused & handler);

    /**
     * @brief All connections from a previous pause are resumed
     * @param handler callback handler when resume operation has been completed
     */
    void resumed(const TorrentResumed & handler);

    /// Getters

    libtorrent::sha1_hash infoHash() const noexcept;

    std::map<libtorrent::tcp::endpoint, std::shared_ptr<Peer>> peers() const noexcept;

    std::shared_ptr<TorrentPlugin> torrentPlugin() const noexcept;

    libtorrent::torrent_status::state_t state() const noexcept;

    std::string savePath() const noexcept;

    std::string name() const noexcept;

    boost::weak_ptr<const libtorrent::torrent_info> metaData() const noexcept;

    float progress() const noexcept;

    int downloadRate() const noexcept;

    int uploadRate() const noexcept;

    bool paused() const noexcept;

    int uploadLimit() const noexcept;

    int downloadLimit() const noexcept;

signals:

    void peerAdded(const std::shared_ptr<Peer> &);

    void peerRemoved(const libtorrent::tcp::endpoint &);

    void torrentPluginAdded(const std::shared_ptr<TorrentPlugin> &);

    void torrentPluginRemoved();

    // Triggered when torrent is added witout metadata, but later
    // receives it from peers
    void metadataReady();

    void stateChanged(libtorrent::torrent_status::state_t);

    void progressChanged(float);

    void downloadRateChanged(int);

    void uploadRateChanged(int);

    void pausedChanged(bool);

    void uploadLimitChanged(int);

    void downloadLimitChanged(int);

private:

    friend class Node;

    /// Updating routines

    void addPeer(const libtorrent::peer_info &);

    void removePeer(const libtorrent::tcp::endpoint &);

    void addTorrentPlugin(const extension::status::TorrentPlugin &);

    void removeTorrentPlugin();

    void updateStatus(const libtorrent::torrent_status &);

    void updatePeerStatuses(const std::vector<libtorrent::peer_info> &);

    void updateUploadLimit(int);

    void updateDownloadLimit(int);

    void paused();

    void resumed();

    void setMetadata(const boost::shared_ptr<const libtorrent::torrent_info> &);

    /// Members

    // Plugin reference
    boost::shared_ptr<extension::Plugin> _plugin;

    // Most recent libtorrent status
    libtorrent::torrent_status _status;

    // Total (bytes/second across libtorrent+plugin) upload/download limit.
    // If not set, then unlimited.
    int _uploadLimit, _downloadLimit;

    // Peers
    std::map<libtorrent::tcp::endpoint, std::shared_ptr<Peer>> _peers;

    // TorrentPlugin
    std::shared_ptr<TorrentPlugin> _torrentPlugin;

    // All streams for this torrent.
    // Not quite sure if multiple separate streams for one torrent
    // is necessary, if not, then remove this QSet later.
    //QSet<Stream *> _streams;
};

}
}

#endif // JOYSTREAM_CORE_TORRENT_HPP
