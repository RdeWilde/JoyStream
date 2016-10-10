/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include <controller/Peer.hpp>
#include <gui/gui.hpp>
#include <core/core.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <protocol_statemachine/protocol_statemachine.hpp>

namespace joystream {
namespace classic {
namespace controller {

Peer::Peer(core::Peer * peer,
           gui::PeerTableModel * classicPeerTableModel,
           gui::BuyerTableModel * buyerTableModel,
           gui::ObserverTableModel * observerTableModel,
           gui::SellerTableModel * sellerTableModel)
    : _endPoint(peer->endPoint())
    , _peer(peer)
    , _peerPlugin(nullptr)
    , _connection(nullptr)
    , _classicTableModel(classicPeerTableModel)
    , _classicTableRowModel(_classicTableModel->add(_endPoint))
    , _peerDialogModels(_endPoint,
                        buyerTableModel,
                        observerTableModel,
                        sellerTableModel) {

    // Host (always set): no signal
    setHost(peer->endPoint());

    // Client name
    QObject::connect(peer,
                     &core::Peer::clientChanged,
                     this,
                     &Peer::setClientName);

    setClientName(peer->client());
}

Peer::~Peer() {

    // Drop from classic model
    _classicTableModel->remove(_classicTableRowModel->row());
}

void Peer::setPeerPlugin(core::PeerPlugin * peerPlugin) {

    // PeerBitSwaprBEPSupportStatus
    QObject::connect(peerPlugin,
                     &core::PeerPlugin::peerBitSwaprBEPSupportStatusChanged,
                     this,
                     &Peer::setBEPSupport);

    setBEPSupport(peerPlugin->peerBitSwaprBEPSupportStatus());
}

void Peer::setConnection(core::Connection * connection) {

    /// CBStateMachine signals
    core::CBStateMachine * machine = connection->machine();

    // AnnouncedModeAndTermsFromPeers
    QObject::connect(machine,
                     &core::CBStateMachine::announcedModeAndTermsFromPeerChanged,
                     this,
                     &Peer::setAnnouncedModeAndTermsFromPeer);

    setAnnouncedModeAndTermsFromPeer(machine->announcedModeAndTermsFromPeer());

    // InnerStateTypeIndex
    QObject::connect(machine,
                     &core::CBStateMachine::innerStateTypeIndexChanged,
                     this,
                     &Peer::setInnerStateIndex);

    setInnerStateIndex(machine->innerStateTypeIndex());
}

void Peer::setHost(const boost::asio::ip::tcp::endpoint &endPoint) {
     _classicTableRowModel->setHost(endPoint);
}

void Peer::setClientName(const std::string &client) {
    _classicTableRowModel->setClientName(client);
}

void Peer::setBEPSupport(const extension::BEPSupportStatus & status) {
    _classicTableRowModel->setBEPSupport(status);
}

void Peer::setAnnouncedModeAndTermsFromPeer(const protocol_statemachine::AnnouncedModeAndTerms & announcedModeAndTerms) {

    // Peer dialog handling

    switch(announcedModeAndTerms.modeAnnounced()) {

        case protocol_statemachine::ModeAnnounced::none: _peerDialogModels.removeFromAnnouncedModeTables(); break;
        case protocol_statemachine::ModeAnnounced::observe: _peerDialogModels.showInObserverTable(); break;
        case protocol_statemachine::ModeAnnounced::sell: _peerDialogModels.showInSellerTable(announcedModeAndTerms.sellModeTerms()); break;
        case protocol_statemachine::ModeAnnounced::buy: _peerDialogModels.showInBuyerTable(announcedModeAndTerms.buyModeTerms()); break;

        default:
            assert(false);
    }

}

void Peer::setInnerStateIndex(const core::CBStateMachine::InnerStateIndex &) {
    // update stuff in session dialog views?
    // gui::BuyingModeSessionWidget
}

}
}
}
