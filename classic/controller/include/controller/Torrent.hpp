/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_TORRENT_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_TORRENT_HPP

#include <gui/MainWindow/TorrentTreeViewRow.hpp>

#include <libtorrent/socket.hpp>
#include <libtorrent/torrent_status.hpp>
#include <QObject>
#include <boost/optional.hpp>
#include <cstdint>
#include <memory>

namespace joystream {
namespace core {
    class Torrent;
    class Peer;
    class TorrentPlugin;
    class Session;
    class Connection;
}
namespace protocol_session {
    enum class SessionMode;
}
namespace classic {
namespace gui {
    class PeersDialog;
}
namespace controller {

class ApplicationController;
class Peer;

class Torrent : public QObject {

    Q_OBJECT

public:

    Torrent(core::Torrent * torrent,
            ApplicationController * applicationController,
            gui::TorrentTreeViewRow * torrentTreeViewRow);

    ~Torrent();

public slots:

    /// core events

    void addPeer(core::Peer * peer);

    void removePeer(const libtorrent::tcp::endpoint & endPoint);


    void pluginModeChanged(protocol_session::SessionMode mode);

    void connectionAdded(const core::Connection *);

    void connectionRemoved(const libtorrent::tcp::endpoint &);

    void setTorrentPluginPresent(core::TorrentPlugin * model);

    void setTorrentPluginAbsent();

    void updateBalance();

    //

    void setState(libtorrent::torrent_status::state_t state, float progress);

    void setPaused(bool);

    void setDownloadSpeed(int speed);

    void setUploadSpeed(int speed);

    /// view events

public:

    /// Peers Dialog

    void showPeersDialog();

    void hidePeersDialog();

    bool peersDialogDisplayed();

    /// Torrent tree view row

    gui::TorrentTreeViewRow * torrentTreeViewRow() const noexcept;

    /**
     * @brief Sets peer tree view row, frees any previously set instance
     * @param peerTreeViewRow row to be set
     */
    void setTorrentTreeViewRow(gui::TorrentTreeViewRow * torrentTreeViewRow);

    void dropTorrentTreeViewRow();

    bool peerTorrentTreeViewRow() const noexcept;


private:

    core::Torrent * _torrent;

    ApplicationController * _applicationController;

    // Could be by value, since all torrents are represented with
    // a row in the main window, however this may change, so we may
    // as well make this as general as possible
    std::unique_ptr<gui::TorrentTreeViewRow> _torrentTreeViewRow;

    std::map<libtorrent::tcp::endpoint, std::unique_ptr<Peer>> _peers;

    std::unique_ptr<gui::PeersDialog> _peersDialog;

    // Updates the peer counts
    void updateConnectionCounts(const core::Session * session);

    // Utility routine for getting counts on number of different peer types
    static void numberOf(const core::Session * session,
                         unsigned int & numberOfBuyers,
                         unsigned int & numberOfSellers,
                         unsigned int & numberOfObservers,
                         unsigned int & numberOfUnAnnounced);

    // Get balance
    int64_t getBalance(const core::Session * session);

    // Utility routine for figuring out the balance in the corresponding mode
    static int64_t getBalanceInBuyingMode(const core::Session * session);
    static int64_t getBalanceInSellingMode(const core::Session * session);

    core::Session * getSession() const;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_TORRENT_HPP

