/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_TORRENT_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_TORRENT_HPP

#include <gui/MainWindow/TorrentTableRowModel.hpp>

#include <libtorrent/socket.hpp>
#include <libtorrent/torrent_status.hpp>
#include <QObject>
#include <boost/optional.hpp>
#include <cstdint>
#include <memory>

class QStandardItem;

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
            gui::TorrentTableRowModel * rowModel);

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

    void setName(const std::string & name);

    void setSize();

    void setState(libtorrent::torrent_status::state_t state, float progress);

    void setUploadSpeed(int speed);

    void setDownloadSpeed(int speed);

    void setPaused(bool);

    /// view events

public:

    /// Peers Dialog

    void showPeersDialog();

    void hidePeersDialog();

    bool peersDialogDisplayed();

    /// Torrent tree view row

    void setTorrentTreeViewRow(gui::TorrentTableRowModel & row);
    void unsetTorrentTreeViewRow();

    bool peerTorrentTreeViewRow() const noexcept;


private:

    core::Torrent * _torrent;

    gui::TorrentTableRowModel * _rowModel;

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

