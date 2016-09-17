/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_PEER_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_PEER_HPP

#include <gui/PeersDialog/PeerTreeViewRow.hpp>

#include <libtorrent/socket.hpp>
#include <QObject>
#include <boost/cstdint.hpp>
#include <boost/optional.hpp>
#include <memory>

namespace joystream {
namespace core {
    class Peer;
}
namespace extension {
    enum class BEPSupportStatus;
}
namespace classic {
namespace controller {

class Peer : public QObject {

    Q_OBJECT

public:

    Peer(core::Peer * peer,
         const boost::optional<gui::PeerTreeViewRow> & peerTreeViewRow);

    boost::optional<gui::PeerTreeViewRow> peerTreeViewRow() const;
    void setPeerTreeViewRow(const gui::PeerTreeViewRow & row);

public slots:

    /// The slots below will forward the state chagne to the
    /// relevant gui type, if gui is present, otherwise it will
    /// ignore the signal.

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    void setClientName(std::string & client);

    void setBEPSupport(const extension::BEPSupportStatus & status);

private:

    // Not being used for anything, since
    // type has no operations, but may be of use in the future.
    core::Peer * _peer;

    boost::optional<gui::PeerTreeViewRow> _peerTreeViewRow;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_PEER_HPP
