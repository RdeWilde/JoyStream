/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_PEER_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_PEER_HPP

#include <libtorrent/socket.hpp>
#include <QObject>

namespace joystream {
namespace core {
    class Peer;
}
namespace extension {
    enum class BEPSupportStatus;
}
namespace classic {
namespace gui {
    class PeerTableRowModel;
}
namespace controller {

class Peer : public QObject {

    Q_OBJECT

public:

    Peer(core::Peer * peer,
         gui::PeerTableRowModel * peerTreeViewRow);

    ~Peer();

public slots:

    /// The slots below will forward the state chagne to the
    /// relevant gui type, if gui is present, otherwise it will
    /// ignore the signal.

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    void setClientName(const std::string & client);

    void setBEPSupport(const extension::BEPSupportStatus & status);

private:

    // Not being used for anything, since
    // core::Peer has no operations, but may be of use in the future.
    core::Peer * _peer;

    gui::PeerTableRowModel * _row;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_PEER_HPP
