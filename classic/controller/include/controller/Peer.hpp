/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_PEER_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_PEER_HPP

#include <core/core.hpp> // InnerStateIndex
#include <controller/detail.hpp>
#include <libtorrent/socket.hpp>
#include <QObject>

namespace joystream {
namespace core {
    class Peer;
    class PeerPlugin;
    class Connection;
}
namespace extension {
    enum class BEPSupportStatus;
}
namespace protocol_statemachine {
    class AnnouncedModeAndTerms;
}
namespace classic {
namespace controller {

class Peer : public QObject {

    Q_OBJECT

public:

    Peer(core::Peer * peer,
         gui::PeerTableModel * classicPeerTableModel,
         gui::BuyerTableModel * buyerTableModel,
         gui::ObserverTableModel * observerTableModel,
         gui::SellerTableModel * sellerTableModel);

    ~Peer();

    void setPeerPlugin(core::PeerPlugin * peerPlugin);

    void setConnection(core::Connection * connection);

public slots:

    /// core::Peer signal handlers

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    void setClientName(const std::string & client);

    /// core::PeerPlugin signal handlers

    void setBEPSupport(const extension::BEPSupportStatus & status);

    /// core::CBStateMachine signal handlers

    void setAnnouncedModeAndTermsFromPeer(const protocol_statemachine::AnnouncedModeAndTerms & announcedModeAndTerms);

    void setInnerStateIndex(const core::CBStateMachine::InnerStateIndex & index);

    /// core::Payor signal handlers


    /// core::Payee signal handlers


private:

    libtorrent::tcp::endpoint _endPoint;

    /// Core references

    core::Peer * _peer;

    core::PeerPlugin * _peerPlugin;

    core::Connection * _connection;

    /// View model references

    // Model for classic view of this peer
    gui::PeerTableModel * _classicTableModel;

    gui::PeerTableRowModel * _classicTableRowModel;

    detail::ModeAnnouncedTableModelManager _peerDialogModels;

    detail::SessionDialogModels _sessionDialogModels;

};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_PEER_HPP
