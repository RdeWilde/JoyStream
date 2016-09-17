/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 23 2016
 */

#include <gui/PeersDialog.hpp>
#include <gui/Language.hpp>
#include "ui_PeersDialog.h"
#include <core/core.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <extension/extension.hpp>
#include <common/BitcoinRepresentation.hpp>

#include <boost/variant/get.hpp>

namespace joystream {
namespace classic {
namespace gui {

// Move out later
QString toString(const libtorrent::tcp::endpoint & endPoint) {

    std::string endPointString = libtorrent::print_endpoint(endPoint);
    return QString::fromStdString(endPointString);
}

/// NormalPeerRow

PeerTreeViewRow * PeerTreeViewRow::create(QStandardItemModel * model,
                                      const std::shared_ptr<core::Peer> & peer) {

    QStandardItem * host = new QStandardItem(),
                  * clientName = new QStandardItem(),
                  * BEPSupportStatus = new QStandardItem();

    // NB: Claims item memory
    model->appendRow(QList<QStandardItem *>() << host << clientName << BEPSupportStatus);

    PeerTreeViewRow * row = new PeerTreeViewRow(host, clientName, BEPSupportStatus);

    row->setHost(peer->endPoint());
    row->setClientName(peer->client());

    QObject::connect(peer.get(), &core::Peer::clientChanged,
                     row, &PeerTreeViewRow::setClientName);

    return row;
}

PeerTreeViewRow::PeerTreeViewRow(QStandardItem * host,
                             QStandardItem * clientName,
                             QStandardItem * BEPSupportStatus)
    : _hostItem(host)
    , _clientNameItem(clientName)
    , _BEPSupportStatusItem(BEPSupportStatus) {
}

void PeerTreeViewRow::set(const std::shared_ptr<core::PeerPlugin> & peerPlugin) {

    setBEPSupport(peerPlugin->peerBitSwaprBEPSupportStatus());

    QObject::connect(peerPlugin.get(), &core::PeerPlugin::peerBEP10SupportStatusChanged,
                     this, &PeerTreeViewRow::setBEPSupport);
}

QStandardItem * PeerTreeViewRow::hostItem() const noexcept {
    return _hostItem;
}

QStandardItem * PeerTreeViewRow::clientNameItem() const noexcept {
    return _clientNameItem;
}

QStandardItem * PeerTreeViewRow::BEPSupportStatusItem() const noexcept {
    return _BEPSupportStatusItem;
}

void PeerTreeViewRow::setHost(const libtorrent::tcp::endpoint & endPoint) {
    _hostItem->setText(toString(endPoint));
}

void PeerTreeViewRow::setClientName(const std::string & clientName) {
    _clientNameItem->setText(QString::fromStdString(clientName));
}

void PeerTreeViewRow::setBEPSupport(const extension::BEPSupportStatus & status) {
    _BEPSupportStatusItem->setText(Language::toString(status));
}

int PeerTreeViewRow::row() const noexcept {
    return _hostItem->row();
}

/// BuyerConnectionRow

BuyerConnectionRow * BuyerConnectionRow::create(QStandardItemModel * model,
                                                const libtorrent::tcp::endpoint & endPoint,
                                                const protocol_wire::BuyerTerms & terms,
                                                const BitcoinDisplaySettings * settings) {

    QStandardItem * hostItem = new QStandardItem(),
                  * maxPriceItem = new QStandardItem(),
                  * maxLockItem = new QStandardItem(),
                  * minNumberOfSellersItem = new QStandardItem(),
                  * maxContractFeePerKbItem = new QStandardItem();

    // NB: Claims item memory
    model->appendRow(QList<QStandardItem *>() << hostItem
                                              << maxPriceItem
                                              << maxLockItem
                                              << minNumberOfSellersItem
                                              << maxContractFeePerKbItem);

    BuyerConnectionRow * row = new BuyerConnectionRow(hostItem,
                                                      maxPriceItem,
                                                      maxLockItem,
                                                      minNumberOfSellersItem,
                                                      maxContractFeePerKbItem,
                                                      settings);

    row->setHost(endPoint);
    row->setTerms(terms);

    return row;
}

BuyerConnectionRow::BuyerConnectionRow(QStandardItem * hostItem,
                                       QStandardItem * maxPriceItem,
                                       QStandardItem * maxLockItem,
                                       QStandardItem * minNumberOfSellersItem,
                                       QStandardItem * maxContractFeePerKbItem,
                                       const BitcoinDisplaySettings * settings)
    : _hostItem(hostItem)
    , _maxPriceItem(maxPriceItem)
    , _maxLockItem(maxLockItem)
    , _minNumberOfSellersItem(minNumberOfSellersItem)
    , _maxContractFeePerKbItem(maxContractFeePerKbItem)
    , _settings(settings) {
}

void BuyerConnectionRow::setHost(const libtorrent::tcp::endpoint & endPoint) {
    _hostItem->setText(toString(endPoint));
}

void BuyerConnectionRow::setTerms(const protocol_wire::BuyerTerms & terms) {

    _maxPriceItem->setText(BitcoinRepresentation(terms.maxPrice()).toString(_settings));
    _maxLockItem->setText(QString::number(terms.maxLock()));
    _minNumberOfSellersItem->setText(QString::number(terms.minNumberOfSellers()));
    _maxContractFeePerKbItem->setText(BitcoinRepresentation(terms.maxContractFeePerKb()).toString(_settings));
}

int BuyerConnectionRow::row() const noexcept {
    return _hostItem->row();
}

/// SellerConnectionRow

SellerConnectionRow * SellerConnectionRow::create(QStandardItemModel * model,
                                                  const libtorrent::tcp::endpoint & endPoint,
                                                  const protocol_wire::SellerTerms & terms,
                                                  const BitcoinDisplaySettings * settings) {

    QStandardItem * hostItem = new QStandardItem(),
                  * minPrice = new QStandardItem(),
                  * minLock = new QStandardItem(),
                  * maxSellers = new QStandardItem(),
                  * minContractFeePerKb = new QStandardItem(),
                  * settlementFee = new QStandardItem();

    // NB: Claims item memory
    model->appendRow(QList<QStandardItem *>() << hostItem
                                              << minPrice
                                              << minLock
                                              << maxSellers
                                              << minContractFeePerKb
                                              << settlementFee);

    SellerConnectionRow * row = new SellerConnectionRow(hostItem,
                                                        minPrice,
                                                        minLock,
                                                        maxSellers,
                                                        minContractFeePerKb,
                                                        settlementFee,
                                                        settings);

    row->setHost(endPoint);
    row->setTerms(terms);

    return row;
}

SellerConnectionRow::SellerConnectionRow(QStandardItem * hostItem,
                                         QStandardItem * minPriceItem,
                                         QStandardItem * minLockItem,
                                         QStandardItem * maxSellersItem,
                                         QStandardItem * minContractFeePerKbItem,
                                         QStandardItem * settlementFeeItem,
                                         const BitcoinDisplaySettings * settings)
    : _hostItem(hostItem)
    , _minPriceItem(minPriceItem)
    , _minLockItem(minLockItem)
    , _maxSellersItem(maxSellersItem)
    , _minContractFeePerKbItem(minContractFeePerKbItem)
    , _settlementFeeItem(settlementFeeItem)
    , _settings(settings) {

}

void SellerConnectionRow::setHost(const libtorrent::tcp::endpoint & endPoint) {
    _hostItem->setText(toString(endPoint));
}

void SellerConnectionRow::setTerms(const protocol_wire::SellerTerms & terms) {

    _minPriceItem->setText(BitcoinRepresentation(terms.minPrice()).toString(_settings));
    _minLockItem->setText(QString::number(terms.minLock()));
    _maxSellersItem->setText(QString::number(terms.maxSellers()));
    _minContractFeePerKbItem->setText(BitcoinRepresentation(terms.minContractFeePerKb()).toString(_settings));
    _settlementFeeItem->setText(BitcoinRepresentation(terms.settlementFee()).toString(_settings));
}

int SellerConnectionRow::row() const noexcept {
    return _hostItem->row();
}

/// ObserverConnectionRow

ObserverConnectionRow * ObserverConnectionRow::create(QStandardItemModel * model,
                                                      const libtorrent::tcp::endpoint & endPoint,
                                                      const BitcoinDisplaySettings * settings) {

    QStandardItem * hostItem = new QStandardItem();

    // NB: Claims item memory
    model->appendRow(QList<QStandardItem *>() << hostItem);

    ObserverConnectionRow * row = new ObserverConnectionRow(hostItem, settings);

    row->setHost(endPoint);

    return row;
}

ObserverConnectionRow::ObserverConnectionRow(QStandardItem * hostItem,
                                             const BitcoinDisplaySettings * settings)
    : _hostItem(hostItem)
    , _settings(settings){
}

void ObserverConnectionRow::setHost(const libtorrent::tcp::endpoint & endPoint) {
    _hostItem->setText(toString(endPoint));
}

int ObserverConnectionRow::row() const noexcept {
    return _hostItem->row();
}

/// PeerRows

PeerRows * PeerRows::create(QStandardItemModel * anyPeerTableModel,
                            QStandardItemModel * buyerConnectionTableModel,
                            QStandardItemModel * sellerConnectionTableModel,
                            QStandardItemModel * observerConnectionTableModel,
                            PeerTreeViewRow * normalPeerRow,
                            const libtorrent::tcp::endpoint & endPoint,
                            const BitcoinDisplaySettings * settings) {

    boost::optional<ConnectionRow> row; // no connection

    return new PeerRows(anyPeerTableModel,
                        buyerConnectionTableModel,
                        sellerConnectionTableModel,
                        observerConnectionTableModel,
                        normalPeerRow,
                        row,
                        endPoint,
                        settings);
}

PeerRows::PeerRows(QStandardItemModel * anyPeerTableModel,
                   QStandardItemModel * buyerConnectionTableModel,
                   QStandardItemModel * sellerConnectionTableModel,
                   QStandardItemModel * observerConnectionTableModel,
                   PeerTreeViewRow * normalPeerRow,
                   boost::optional<ConnectionRow> & connectionRow,
                   const libtorrent::tcp::endpoint & endPoint,
                   const BitcoinDisplaySettings * settings)
    : _anyPeerTableModel(anyPeerTableModel)
    , _buyerConnectionTableModel(buyerConnectionTableModel)
    , _sellerConnectionTableModel(sellerConnectionTableModel)
    , _observerConnectionTableModel(observerConnectionTableModel)
    , _normalPeerRow(normalPeerRow)
    , _connectionRow(connectionRow)
    , _endPoint(endPoint)
    , _settings(settings) {
}

PeerRows::~PeerRows() {

    dropConnectionRow();

    // normalPeerRow: Remove from row
    _anyPeerTableModel->removeRow(_normalPeerRow->row());

    // normalPeerRow is automatically cleared through parental relationship
    delete _normalPeerRow;
}

void PeerRows::set(const std::shared_ptr<core::Connection> & connection) {

    auto machine = connection->machine();

    setAnouncedModesAndterms(machine->announcedModeAndTermsFromPeer());

    QObject::connect(machine.get(), &core::CBStateMachine::announcedModeAndTermsFromPeerChanged,
                     this, &PeerRows::setAnouncedModesAndterms);

}

PeerTreeViewRow * PeerRows::normalPeerRow() const noexcept {
    return _normalPeerRow;
}

PeerRows::ConnectionRow PeerRows::createConnectionRow(const libtorrent::tcp::endpoint & endPoint, const protocol_statemachine::AnnouncedModeAndTerms & announed) {

    protocol_statemachine::ModeAnnounced mode = announed.modeAnnounced();

    switch(mode) {

        case protocol_statemachine::ModeAnnounced::buy: return BuyerConnectionRow::create(_buyerConnectionTableModel, endPoint, announed.buyModeTerms(), _settings);
        case protocol_statemachine::ModeAnnounced::sell: return SellerConnectionRow::create(_sellerConnectionTableModel, endPoint, announed.sellModeTerms(), _settings);
        case protocol_statemachine::ModeAnnounced::observe: return ObserverConnectionRow::create(_observerConnectionTableModel, endPoint, _settings);
        case protocol_statemachine::ModeAnnounced::none: return PeerRows::ConnectionRow();
        default:
            assert(false);
    }
}

void PeerRows::dropConnectionRow() {

    if(_connectionRow.is_initialized()) {

        ConnectionRow row = _connectionRow.get();

        if(BuyerConnectionRow * & b = boost::get<BuyerConnectionRow *>(row)) {

            _buyerConnectionTableModel->removeRow(b->row());

            delete b;

        } else if(SellerConnectionRow * & s = boost::get<SellerConnectionRow *>(row)) {

            _sellerConnectionTableModel->removeRow(s->row());

            delete s;

        } else if(ObserverConnectionRow * & o = boost::get<ObserverConnectionRow *>(row)) {

            _observerConnectionTableModel->removeRow(o->row());

            delete o;
        }

        // Empty out row
        _connectionRow = ConnectionRow();
    }
}

void PeerRows::setAnouncedModesAndterms(const protocol_statemachine::AnnouncedModeAndTerms & announed) {

    // If a connection row is already set,
    if(_connectionRow.is_initialized()) {

        ConnectionRow row = _connectionRow.get();

        // replace it if a new type of mode has been announced, or just possibly update terms if its the same
        if(BuyerConnectionRow * & b = boost::get<BuyerConnectionRow *>(row)) {

            if(announed.modeAnnounced() != protocol_statemachine::ModeAnnounced::buy) {

                dropConnectionRow();

                _connectionRow = createConnectionRow(_endPoint, announed);

            } else
                b->setTerms(announed.buyModeTerms());

        } else if(SellerConnectionRow * & s = boost::get<SellerConnectionRow *>(row)) {

            if(announed.modeAnnounced() != protocol_statemachine::ModeAnnounced::sell) {

                dropConnectionRow();

                _connectionRow = createConnectionRow(_endPoint, announed);

            } else
                s->setTerms(announed.sellModeTerms());

        } else if(boost::get<ObserverConnectionRow *>(row)) {

            if(announed.modeAnnounced() != protocol_statemachine::ModeAnnounced::observe) {

                dropConnectionRow();

                _connectionRow = createConnectionRow(_endPoint, announed);

            } // nothing to update

        } else
            assert(false);

    } else  // When not set, create the new correct one
        _connectionRow = PeerRows::createConnectionRow(_endPoint, announed);
}

/// PeersDialog

PeersDialog::PeersDialog(QWidget * parent,
                         const BitcoinDisplaySettings * settings,
                         const std::shared_ptr<core::Torrent> torrent)
    : QDialog(parent)
    , _settings(settings)
    , _torrent(torrent){

    ui->setupUi(this);

    /// all

    _anyPeerTableModel.setHorizontalHeaderLabels(QStringList() << tr("Host") << tr("Client") << tr("Extension"));

    ui->allPeersTableView->setColumnWidth(0, 100);
    ui->allPeersTableView->setColumnWidth(1, 100);
    ui->allPeersTableView->setColumnWidth(2, 100);
    ui->allPeersTableView->setModel(&_anyPeerTableModel);

    /// buyers

    _buyerConnectionTableModel.setHorizontalHeaderLabels(QStringList() << tr("Host") << tr("Price") << tr("Lock Time") << tr("#Sellers") << tr("Contract fee"));

    ui->buyersPeersTableView->setColumnWidth(0, 100);
    ui->buyersPeersTableView->setColumnWidth(1, 100);
    ui->buyersPeersTableView->setColumnWidth(2, 100);
    ui->buyersPeersTableView->setColumnWidth(3, 100);
    ui->buyersPeersTableView->setColumnWidth(4, 100);
    ui->buyersPeersTableView->setModel(&_buyerConnectionTableModel);

    /// sellers

    _sellerConnectionTableModel.setHorizontalHeaderLabels(QStringList() << tr("Host") << tr("Price") << tr("Lock Time") << tr("Max #Sellers") << tr("Contract fee") << tr("Settlement fee"));

    ui->sellersPeersTableView->setColumnWidth(0, 100);
    ui->sellersPeersTableView->setColumnWidth(1, 100);
    ui->sellersPeersTableView->setColumnWidth(2, 100);
    ui->sellersPeersTableView->setColumnWidth(3, 100);
    ui->sellersPeersTableView->setColumnWidth(4, 100);
    ui->sellersPeersTableView->setColumnWidth(5, 100);
    ui->sellersPeersTableView->setModel(&_sellerConnectionTableModel);

    /// observers

    _observerConnectionTableModel.setHorizontalHeaderLabels(QStringList() << tr("Host"));
    ui->observersPeersTableView->setModel(&_observerConnectionTableModel);

    /// Populate models

    // Add all peers
    for(auto mapping : torrent->peers())
        addPeer(mapping.second);

    // Add all peer plugins
    std::shared_ptr<core::TorrentPlugin> plugin = torrent->torrentPlugin();

    if(plugin.get() != NULL) {
        for(auto mapping : plugin->peers())
            addPeerPlugin(mapping.second);
    }

    // Add all connections
    std::shared_ptr<core::Session> session = plugin->session();
    for(auto mapping : session->connections())
        addConnection(mapping.second);

    /// Connect

    QObject::connect(torrent.get(), &core::Torrent::peerAdded,
                     this, &PeersDialog::addPeer);

    QObject::connect(torrent.get(), &core::Torrent::peerRemoved,
                     this, &PeersDialog::removePeer);

    QObject::connect(plugin.get(), &core::TorrentPlugin::peerPluginAdded,
                     this, &PeersDialog::addPeerPlugin);

    QObject::connect(plugin.get(), &core::TorrentPlugin::peerPluginRemoved,
                     this, &PeersDialog::removePeerPlugin);

    QObject::connect(session.get(), &core::Session::connectionAdded,
                     this, &PeersDialog::addConnection);

    QObject::connect(session.get(), &core::Session::connectionRemoved,
                     this, &PeersDialog::removeConnection);
}

PeersDialog::~PeersDialog()
{
    delete ui;

    // Clear all PeerRows
    for(auto mapping : _endPointToPeerRows)
        delete mapping.second;
}

void PeersDialog::addPeer(const std::weak_ptr<core::Peer> & p) {

    if(std::shared_ptr<core::Peer> peer = p.lock()) {

        auto endPoint = peer->endPoint();

        assert(_endPointToPeerRows.count(endPoint) == 0);

        PeerTreeViewRow * normalPeerRow = PeerTreeViewRow::create(&_anyPeerTableModel, peer);

        PeerRows * newPeerRow = PeerRows::create(&_anyPeerTableModel,
                                                 &_buyerConnectionTableModel,
                                                 &_sellerConnectionTableModel,
                                                 &_observerConnectionTableModel,
                                                 normalPeerRow,
                                                 endPoint,
                                                 _settings);

        // If present, add corresponding peer and connection
        // NB: we make no assumption that we did not miss
        if(std::shared_ptr<core::Torrent> torrent = _torrent.lock()) {

            auto torrentPlugin = torrent->torrentPlugin();

            // peer plugins: we can´t trust that addPeer event occurs prior to addPeerPlugin
            {
                auto peerPlugins = torrentPlugin->peers();
                auto it = peerPlugins.find(endPoint);

                if(it != peerPlugins.cend())
                    normalPeerRow->set(it->second);
            }

            // connections:: we can´t trust that addPeer event occurs prior to addConnection
            {
                auto connections = torrentPlugin->session()->connections();
                auto it = connections.find(endPoint);

                if(it != connections.cend())
                    newPeerRow->set(it->second);
            }

        }

        _endPointToPeerRows.insert(std::make_pair(endPoint, newPeerRow));
    }

}

void PeersDialog::removePeer(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _endPointToPeerRows.find(endPoint);

    if(it != _endPointToPeerRows.cend()) {

        // Remove from map
        _endPointToPeerRows.erase(it);

        // Delete peer
        delete it->second;
    }

}

void PeersDialog::addPeerPlugin(const std::weak_ptr<core::PeerPlugin> & p) {

    if(std::shared_ptr<core::PeerPlugin> peerPlugin = p.lock()) {

        auto it = _endPointToPeerRows.find(peerPlugin->endPoint());

        // If peer present, then setup plugin, otherwise we ignore event
        if(it != _endPointToPeerRows.cend())
            it->second->normalPeerRow()->set(peerPlugin);
    }

}

void PeersDialog::removePeerPlugin(const libtorrent::tcp::endpoint &) {
    // ignore
}

void PeersDialog::addConnection(const std::weak_ptr<core::Connection> & c) {

    if(std::shared_ptr<core::Connection> connection = c.lock()) {

        auto it = _endPointToPeerRows.find(connection->connectionId());

        // If peer present, then setup connection, otherwise we ignore event
        if(it != _endPointToPeerRows.cend())
            it->second->set(connection);
    }

}

void PeersDialog::removeConnection(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _endPointToPeerRows.find(endPoint);

    // If peer present, then try to drop connection row
    if(it != _endPointToPeerRows.cend())
        it->second->dropConnectionRow();
}

}
}
}
