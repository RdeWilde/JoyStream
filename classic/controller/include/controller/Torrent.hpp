/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_TORRENT_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_TORRENT_HPP

#include <libtorrent/socket.hpp>
#include <libtorrent/torrent_status.hpp>

#include <QObject>

#include <cstdint>

namespace joystream {
namespace core {
    class Torrent;
    class TorrentPlugin;
    class Session;
    class Connection;
}
namespace protocol_session {
    enum class SessionMode;
}
namespace classic {
namespace gui {
    class TorrentTreeViewRow;
}
namespace controller {

class ApplicationController;

class Torrent : public QObject {

public:

    Torrent(ApplicationController * applicationController,
            gui::TorrentTreeViewRow * row,
            core::Torrent * torrent);

public slots:

    /// core events

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

private:

    ApplicationController * _applicationController;

    gui::TorrentTreeViewRow * _row;

    core::Torrent * _torrent;

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

