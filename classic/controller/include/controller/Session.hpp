/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_SESSION_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_SESSION_HPP

#include <controller/Buying.hpp>
#include <controller/Selling.hpp>

#include <libtorrent/socket.hpp>

#include <boost/variant.hpp>

#include <QObject>

namespace joystream {
namespace protocol_session {
    enum class SessionMode;
    enum class SessionState;
}
namespace core {
    class Session;
    class Connection;
}
namespace classic {
namespace gui {
    class PeersDialog;
    class TorrentTableRowModel;
}
namespace controller {

class Connection;

class Observing {

};

class Session : public QObject {

    Q_OBJECT

public:

    typedef boost::variant<Observing, Buying, Selling> SubState;

    Session(core::Session * session);

    ~Session();

    SubState & subState() noexcept;

    void setTorrentTreeViewRow(gui::TorrentTableRowModel * row);
    void unsetTorrentTreeViewRow();

    void setPeersDialog(gui::PeersDialog * peersDialog);
    void unsetPeersDialog();

public slots:

    void setMode(protocol_session::SessionMode);

    void setState(protocol_session::SessionState);

    void addConnection(const core::Connection * c);

    void removeConnection(const libtorrent::tcp::endpoint & endPoint);

private:

    core::Session * _session;

    std::map<libtorrent::tcp::endpoint, std::unique_ptr<Connection>> _peerPlugins;

    SubState _subState;

    gui::TorrentTableRowModel * _torrentTreeViewRow;

    gui::PeersDialog * _peersDialog;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_SESSION_HPP
