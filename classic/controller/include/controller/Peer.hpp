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
namespace gui {
    class PeerTreeViewRow;
}
namespace controller {

class Peer : public QObject {

    Q_OBJECT

public:

    Peer(core::Peer * peer,
         gui::PeerTreeViewRow * peerTreeViewRow);

    ~Peer();

    /**
     * @brief Creates a peer tree view row
     * @return
     */
    gui::PeerTreeViewRow * create() const noexcept;

    gui::PeerTreeViewRow * peerTreeViewRow() const noexcept;

    /**
     * @brief Sets peer tree view row, frees any previously set instance
     * @param peerTreeViewRow row to be set
     */
    void setPeerTreeViewRow(gui::PeerTreeViewRow * peerTreeViewRow);

    void dropPeerTreeViewRow();

    bool peerTreeViewRowSet() const noexcept;

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

    std::unique_ptr<gui::PeerTreeViewRow> _peerTreeViewRow;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_PEER_HPP
