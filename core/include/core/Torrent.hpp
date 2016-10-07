/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 17 2016
 */

#ifndef JOYSTREAM_CORE_TORRENT_HPP
#define JOYSTREAM_CORE_TORRENT_HPP

#include <extension/extension.hpp>
#include <libtorrent/peer_info.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <QObject>
#include <memory>
#include <chrono>

namespace joystream {
namespace core {
namespace detail {
    struct NodeImpl;
}

class Peer;
class TorrentPlugin;

/**
 * @brief Torrent handle
 * @note Detect expiry of handles listening to the Node::removeTorrent
 */
class Torrent : public QObject {

    Q_OBJECT

public:

    typedef extension::request::SubroutineHandler TorrentPaused;
    typedef extension::request::SubroutineHandler TorrentResumed;
    typedef std::chrono::system_clock::time_point Timestamp;

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    Torrent(const libtorrent::torrent_handle & handle,
            const libtorrent::torrent_status & status,
            const std::vector<char> & resumeData,
            int uploadLimit,
            int downloadLimit,
            const boost::shared_ptr<extension::Plugin> &);

    ~Torrent();

    /// Actions

    /**
     * @brief All connections are ended, can only be called when Node is started.
     * @param graceful whether regular bittorrent, i.e. non-plugin, pieces in inbound transit will be completed before closing a connection.
     * @param handler callback handler when pause oeration has been completed
     */
    void paused(bool graceful, const TorrentPaused & handler);

    /**
     * @brief All connections from a previous pause are resumed, can only be called when Node is started.
     * @param handler callback handler when resume operation has been completed
     */
    void resumed(const TorrentResumed & handler);

    /**
     * @brief Starts asynchronous process of generating resume data, can be called at any time.
     * When process is done, the resumeDataGenerated signal will be emitted,
     */
    void generateResumeData();

    /// Getters

    libtorrent::sha1_hash infoHash() const noexcept;

    /**
     * @brief Returns hash of the _JS appended infohash for faster peer discovery
     * @return sha1_hash of _JS appended infohash
     */
    libtorrent::sha1_hash secondaryInfoHash() const noexcept;

    /**
     * @brief Returns map of Peer handles, and endpoint is used as key.
     * @return Map of Peer handles with endpoint as key
     */
    std::map<libtorrent::tcp::endpoint, Peer *> peers() const noexcept;

    /**
     * @brief Torrent plugin handle existence ten
     * @return whether torrent plugin handle is present
     */
    bool torrentPluginSet() const noexcept;

    /**
     * @brief Returns torrent plugin handle on for this torrent, if present.
     * @throws HandleNotSet if torrent plugin not set
     * @return Torrent plugin handle
     */
    TorrentPlugin * torrentPlugin() const;

    /**
     * @brief Returns map of announced JoyStream peers with the timestamp of the getPeersReply alert.
     * @return Map of peers with format <endpoint, timestamp>
     */
    std::map<libtorrent::tcp::endpoint, Timestamp> announcedJSPeersAtTimestamp() const noexcept;

    /**
     * @brief Adds a JS peer with this torrent to list of JS peers. Note that we might have not
     * connected and validated the peers.
     */
    void addJSPeerAtTimestamp(libtorrent::tcp::endpoint peer, Timestamp timestamp) noexcept;

    /**
     * @brief Loops through all announced JS peers and removes them if they they aren't updated for a while.
     */
    void invalidateOldJSPeers() noexcept;

    libtorrent::torrent_status::state_t state() const noexcept;

    std::string savePath() const noexcept;

    std::string name() const noexcept;

    std::vector<char> resumeData() const noexcept;

    boost::weak_ptr<const libtorrent::torrent_info> metaData() const noexcept;

    float progress() const noexcept;

    int downloadRate() const noexcept;

    int uploadRate() const noexcept;

    bool isPaused() const noexcept;

    int uploadLimit() const noexcept;

    int downloadLimit() const noexcept;

    libtorrent::torrent_handle handle() const;

signals:

    void peerAdded(Peer *);

    void peerRemoved(const libtorrent::tcp::endpoint &);

    void torrentPluginAdded(TorrentPlugin *);

    void torrentPluginRemoved();

    void resumeDataGenerationCompleted(const std::vector<char> &);

    // Triggered when torrent is added witout metadata, but later
    // receives it from peers
    void metadataReady();

    void stateChanged(libtorrent::torrent_status::state_t state, float progress);

    void downloadRateChanged(int);

    void uploadRateChanged(int);

    void pausedChanged(bool);

    void uploadLimitChanged(int);

    void downloadLimitChanged(int);

private:

    friend struct detail::NodeImpl;

    /// Updating routines

    void addPeer(const libtorrent::peer_info &);

    void removePeer(const libtorrent::tcp::endpoint &);

    void removePeer(std::map<libtorrent::tcp::endpoint, std::unique_ptr<Peer>>::const_iterator it);

    void addTorrentPlugin(const extension::status::TorrentPlugin &);

    void removeTorrentPlugin();

    void updateStatus(const libtorrent::torrent_status &);

    void updatePeerStatuses(const std::vector<libtorrent::peer_info> &);

    void updateTorrentPluginStatus(const extension::status::TorrentPlugin &);

    void updateUploadLimit(int);

    void updateDownloadLimit(int);

    void updatePaused(bool);

    void setResumeDataGenerationResult(const std::vector<char> &);

    void setMetadata(const boost::shared_ptr<const libtorrent::torrent_info> &);

    /// Members

    // Plugin reference
    boost::shared_ptr<extension::Plugin> _plugin;

    // Handle for torrent
    libtorrent::torrent_handle _handle;

    // Most recent libtorrent status
    libtorrent::torrent_status _status;

    // Most up to fast resume data for torrent
    std::vector<char> _resumeData;

    // Total (bytes/second across libtorrent+plugin) upload/download limit.
    // If not set, then unlimited.
    int _uploadLimit, _downloadLimit;

    // Peers
    std::map<libtorrent::tcp::endpoint, std::unique_ptr<Peer>> _peers;

    // TorrentPlugin
    std::unique_ptr<TorrentPlugin> _torrentPlugin;

    std::map<libtorrent::tcp::endpoint, Timestamp> _announcedJSPeersAtTimestamp;

    // All streams for this torrent.
    // Not quite sure if multiple separate streams for one torrent
    // is necessary, if not, then remove this QSet later.
    //QSet<Stream *> _streams;
};

}
}

#endif // JOYSTREAM_CORE_TORRENT_HPP
