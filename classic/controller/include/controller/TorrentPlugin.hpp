/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_TORRENTPLUGIN_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_TORRENTPLUGIN_HPP

#include <controller/Session.hpp>

#include <libtorrent/socket.hpp>

#include <QObject>

#include <map>
#include <memory>

namespace joystream {
namespace core {
    class TorrentPlugin;
    class PeerPlugin;
}
namespace classic {
namespace gui {
    class PeersDialog;
    class TorrentTreeViewRow;
}
namespace controller {

class PeerPlugin;

class TorrentPlugin : public QObject {

    Q_OBJECT

public:

    TorrentPlugin(core::TorrentPlugin * torrentPlugin);

    ~TorrentPlugin();

    Session & session() noexcept;

public slots:

    void addPeerPlugin(core::PeerPlugin *);

    void removePeerPlugin(const libtorrent::tcp::endpoint &);

private:

    core::TorrentPlugin * _torrentPlugin;

    std::map<libtorrent::tcp::endpoint, std::unique_ptr<PeerPlugin>> _peerPlugins;

    Session _session;

    gui::TorrentTreeViewRow * _torrentTreeViewRow;

    gui::PeersDialog * _peersDialog;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_TORRENTPLUGIN_HPP
