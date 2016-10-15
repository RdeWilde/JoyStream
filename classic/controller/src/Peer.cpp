/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include <controller/Peer.hpp>
#include <controller/Torrent.hpp>
#include <gui/gui.hpp>
#include <core/core.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <protocol_statemachine/protocol_statemachine.hpp>

namespace joystream {
namespace classic {
namespace controller {

Peer::Peer(core::Peer * peer,
           Torrent * torrent,
           gui::PeerTableModel * classicPeerTableModel,
           gui::BuyerTableModel * buyerTableModel,
           gui::ObserverTableModel * observerTableModel,
           gui::SellerTableModel * sellerTableModel,
           gui::ConnectionTableModel * sellersTableModel,
           gui::ConnectionTableModel * buyersTableModel)
    : _endPoint(peer->endPoint())
    , _peer(peer)
    , _torrent(torrent)
    , _peerPlugin(nullptr)
    , _connection(nullptr)
    , _balance(0)
    , _modeAnnounced(protocol_statemachine::ModeAnnounced::none)
    , _classicTableModel(classicPeerTableModel)
    , _classicTableRowModel(_classicTableModel->add(_endPoint))
    , _peerDialogModels(_endPoint,
                        buyerTableModel,
                        observerTableModel,
                        sellerTableModel)
    , _sessionDialogModels(_endPoint,
                           sellersTableModel,
                           buyersTableModel) {

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

    if(peerPlugin == nullptr)
        throw std::runtime_error("Cannot set peer plugin: is a null pointer");

    if(_peerPlugin != nullptr)
        throw std::runtime_error("Cannot set peer plugin: is already set");
    else
        _peerPlugin = peerPlugin;

    // PeerBitSwaprBEPSupportStatus
    QObject::connect(peerPlugin,
                     &core::PeerPlugin::peerBitSwaprBEPSupportStatusChanged,
                     this,
                     &Peer::setBEPSupport);

    setBEPSupport(peerPlugin->peerBitSwaprBEPSupportStatus());
}

void Peer::setConnection(core::Connection * connection) {

    if(connection == nullptr)
        throw std::runtime_error("Cannot set connection: is a null pointer");

    if(_connection != nullptr)
        throw std::runtime_error("Cannot set connection: is already set");
    else
        _connection = connection;

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

    // core::Payor signals

    auto payor = machine->payor();
    QObject::connect(payor, &core::Payor::priceChanged, this, &Peer::setPrice);
    QObject::connect(payor, &core::Payor::numberOfPaymentsMadeChanged, this, &Peer::setNumberOfPayments);
    QObject::connect(payor, &core::Payor::fundsChanged, this, &Peer::setFunds);
    QObject::connect(payor, &core::Payor::settlementFeeChanged, this, &Peer::setSettlementFee);
    QObject::connect(payor, &core::Payor::refundLockTimeChanged, this, &Peer::setRefundLockTime);
    QObject::connect(payor, &core::Payor::anchorChanged, this, &Peer::setAnchorChanged);

    // core::Peer signals

    auto payee = machine->payee();
    QObject::connect(payee, &core::Payee::priceChanged, this, &Peer::setPrice);
    QObject::connect(payee, &core::Payee::numberOfPaymentsMadeChanged, this, &Peer::setNumberOfPayments);
    QObject::connect(payee, &core::Payee::fundsChanged, this, &Peer::setFunds);
    QObject::connect(payee, &core::Payee::settlementFeeChanged, this, &Peer::setSettlementFee);
    QObject::connect(payee, &core::Payee::lockTimeChanged, this, &Peer::setRefundLockTime);
    QObject::connect(payee, &core::Payee::anchorChanged, this, &Peer::setAnchorChanged);
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

    protocol_statemachine::ModeAnnounced newModeAnnounced = announcedModeAndTerms.modeAnnounced();

    // Update peer counts if they have changed
    if(newModeAnnounced != _modeAnnounced) {

        // Decrement count for current mode
        updatePeerCountOnTorrent(_modeAnnounced, -1);

        // Increment count for new mode
        updatePeerCountOnTorrent(newModeAnnounced, +1);
    }

    // Peer dialog handling
    switch(newModeAnnounced) {

        case protocol_statemachine::ModeAnnounced::none: _peerDialogModels.removeFromAnnouncedModeTables(); break;
        case protocol_statemachine::ModeAnnounced::observe: _peerDialogModels.showInObserverTable(); break;
        case protocol_statemachine::ModeAnnounced::sell: _peerDialogModels.showInSellerTable(announcedModeAndTerms.sellModeTerms()); break;
        case protocol_statemachine::ModeAnnounced::buy: _peerDialogModels.showInBuyerTable(announcedModeAndTerms.buyModeTerms()); break;

        default:
            assert(false);
    }

    _modeAnnounced = newModeAnnounced;
}

void Peer::setInnerStateIndex(const core::CBStateMachine::InnerStateIndex & index) {
    _sessionDialogModels.setInnerStateIndex(index);
}

void Peer::setPrice(quint64 price) {
    _sessionDialogModels.setPrice(price);
}

void Peer::setNumberOfPayments(quint64 num) {
    _sessionDialogModels.setNumberOfPayments(num);

    _torrent->setBalance(num * price?);
}

void Peer::setFunds(quint64 funds) {
    _sessionDialogModels.setFunds(funds);
}

void Peer::setSettlementFee(quint64 fee) {
    _sessionDialogModels.setSettlementFee(fee);
}

void Peer::setRefundLockTime(quint32 refundLockTime) {
    _sessionDialogModels.setRefundLockTime(refundLockTime);
}

void Peer::setAnchorChanged(const Coin::typesafeOutPoint & anchor) {
    _sessionDialogModels.setAnchorChanged(anchor);
}

void Peer::updatePeerCountOnTorrent(protocol_statemachine::ModeAnnounced modeAnnounced, int change) {

    switch(modeAnnounced) {

        case protocol_statemachine::ModeAnnounced::none: _torrent->setNumberOfUnannounced(_torrent->numberOfUnannounced() + change); break;
        case protocol_statemachine::ModeAnnounced::sell: _torrent->setNumberOfSellers(_torrent->numberOfSellers() + change); break;
        case protocol_statemachine::ModeAnnounced::buy: _torrent->setNumberOfBuyers(_torrent->numberOfBuyers() + change); break;
        case protocol_statemachine::ModeAnnounced::observe: _torrent->setNumberOfObservers(_torrent->numberOfObservers() + change); break;
        default:
            assert(false);
    }
}

/**
void Peer::updateBalanceOnTorrent(qint64 change) {
    _torrent->setBalance(_torrent->balance() + change);
}
*/

}
}
}
