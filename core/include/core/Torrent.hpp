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

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    Torrent(const libtorrent::torrent_handle & handle,
            const std::vector<char> & resumeData,
            const boost::shared_ptr<extension::Plugin> &);

    ~Torrent();

    /**
     * @brief All connections are ended, can only be called when Node is started.
     * @param graceful whether regular bittorrent, i.e. non-plugin, pieces in inbound transit will be completed before closing a connection.
     * @param handler callback handler when pause oeration has been completed
     */
    void pause(bool graceful, const TorrentPaused & handler);

    /**
     * @brief All connections from a previous pause are resumed, can only be called when Node is started.
     * @param handler callback handler when resume operation has been completed
     */
    void resume(const TorrentResumed & handler);

    /**
     * @brief Starts asynchronous process of generating resume data, can be called at any time.
     * When process is done, the resumeDataGenerated signal will be emitted,
     */
    void generateResumeData();

    /**
     * @brief Request (blocking) updates on status of all known peers, result
     * is returned as emittion of signal core::Peer::statusUpdated
     * on all corresponding objects.
     */
    void postPeerStatusUpdates() noexcept;

    /// Getters

    libtorrent::torrent_handle handle() const noexcept;

    std::vector<char> resumeData() const noexcept;

    libtorrent::sha1_hash infoHash() const noexcept;

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

signals:

    void peerAdded(Peer *);

    void peerRemoved(const libtorrent::tcp::endpoint &);

    void statusUpdated(const libtorrent::torrent_status & status);

    void torrentPluginAdded(TorrentPlugin *);

    void torrentPluginRemoved();

    void resumeDataGenerationCompleted(const std::vector<char> &);

    void metadataReceived(const boost::shared_ptr<const libtorrent::torrent_info> & torrent_info);

    void metadataFailed();

    void stateChanged(libtorrent::torrent_status::state_t state, float progress);

    void pausedChanged(bool);

private:

    friend struct detail::NodeImpl;

    /// Updating routines

    void addPeer(const libtorrent::peer_info &);

    void removePeer(const libtorrent::tcp::endpoint &);

    void removePeer(std::map<libtorrent::tcp::endpoint, std::unique_ptr<Peer>>::const_iterator it);

    void addTorrentPlugin(const extension::status::TorrentPlugin &);

    void removeTorrentPlugin();

    void setResumeDataGenerationResult(const std::vector<char> & resumeData);

    /// Members


    libtorrent::sha1_hash _infoHash;

    // Handle for torrent
    libtorrent::torrent_handle _handle;

    // Most up to fast resume data for torrent
    std::vector<char> _resumeData;

    // Plugin reference
    boost::shared_ptr<extension::Plugin> _plugin;

    // Peers
    std::map<libtorrent::tcp::endpoint, std::unique_ptr<Peer>> _peers;

    // TorrentPlugin
    std::unique_ptr<TorrentPlugin> _torrentPlugin;

};

}
}

#endif // JOYSTREAM_CORE_TORRENT_HPP
