/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 20 2016
 */

#include <gui/BuyingModeSessionWidget.hpp>
#include <gui/Common.hpp>
#include "ui_BuyingModeSessionWidget.h"
#include <common/BitcoinDisplaySettings.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <core/core.hpp>
#include <protocol_statemachine/protocol_statemachine.hpp>

namespace joystream {
namespace classic {
namespace gui {

BuyingModeSessionWidget * BuyingModeSessionWidget::create(QWidget * parent,
                                                          const BitcoinDisplaySettings * settings,
                                                          const std::shared_ptr<core::Session> & session) {

    if(session->mode() != protocol_session::SessionMode::buying)
        throw std::runtime_error("Session is not in buy mode."); // add type safe later

    // Create widget
    BuyingModeSessionWidget * widget = new BuyingModeSessionWidget(parent, settings);

    // Set fields
    std::shared_ptr<core::Buying> buying = session->buying();

    widget->setTerms(buying->terms());
    widget->setPolicy(buying->policy());
    widget->setState(buying->state());

    if(buying->state() != protocol_session::BuyingState::sending_invitations)
        widget->setContractTx(buying->contractTx());

    // Add Connections
    for(auto mapping : session->connections())
        widget->addConnections(mapping.second);

    // Connect buying mode to widget
    core::Buying * rawBuying = buying.get();

    QObject::connect(rawBuying, &core::Buying::stateChanged,
                     widget, &BuyingModeSessionWidget::setState);

    QObject::connect(rawBuying, &core::Buying::policyChanged,
                     widget, &BuyingModeSessionWidget::setPolicy);

    QObject::connect(rawBuying, &core::Buying::termsChanged,
                     widget, &BuyingModeSessionWidget::setTerms);

    QObject::connect(rawBuying, &core::Buying::contractTxChanged,
                     widget, &BuyingModeSessionWidget::setContractTx);

    // Connect session to widget
    core::Session * rawSession = session.get();

    QObject::connect(rawSession, &core::Session::connectionAdded,
                     widget, &BuyingModeSessionWidget::addConnections);

    QObject::connect(rawSession, &core::Session::connectionRemoved,
                     widget, &BuyingModeSessionWidget::removeConnections);

    return widget;
}

BuyingModeSessionWidget::BuyingModeSessionWidget(QWidget *parent,
                                                 const BitcoinDisplaySettings * settings)
    : QWidget(parent)
    , ui(new Ui::Form())
    , _settings(settings) {

    ui->setupUi(this);
    ui->stateValueLabel->setText("");
    ui->contractTxIdValueLabel->setText("");
    ui->updateTermsPushButton->hide(); // not not implemented

    QObject::connect(ui->updateTermsPushButton, &QPushButton::clicked,
                     this, &BuyingModeSessionWidget::updateTerms);
}

BuyingModeSessionWidget::~BuyingModeSessionWidget() {
    delete ui;
}

void BuyingModeSessionWidget::setTerms(const protocol_wire::BuyerTerms & terms) {

    ui->maxContractFeePrKbValueLabel->setText(BitcoinRepresentation(terms.maxContractFeePerKb()).toString(_settings));

    ui->minNumberOfSellersValueLabel->setText(QString::number(terms.minNumberOfSellers()));

    ui->maxPriceValueLabel->setText(BitcoinRepresentation(terms.maxPrice()).toString(_settings));

    ui->maxRefundDelayValueLabel->setText(QString::number(terms.maxLock()));
}

void BuyingModeSessionWidget::setPolicy(const protocol_session::BuyingPolicy &) {
    // nothing to do really
}

void BuyingModeSessionWidget::setState(const protocol_session::BuyingState & state) {

    ui->stateValueLabel->setText(Common::toString(state));

    // Set update terms button visibility
    if(state != protocol_session::BuyingState::sending_invitations)
        ui->updateTermsPushButton->hide();
    else
        ui->updateTermsPushButton->show();
}

void BuyingModeSessionWidget::setContractTx(const Coin::Transaction & tx) {

    std::string hexEncodedTxId = tx.getHashLittleEndian().getHex();

    ui->contractTxIdValueLabel->setText(QString::fromStdString(hexEncodedTxId));
}

void BuyingModeSessionWidget::addConnections(const std::weak_ptr<core::Connection> & c) {

    if(std::shared_ptr<core::Connection> connection = c.lock()) {

        libtorrent::tcp::endpoint endPoint = connection->connectionId();

        assert(_connections.count(endPoint) == 0);

        auto * widgetConnection = BuyingModeSessionWidget::Connection::create(this, _settings, &_buyers, connection);

        // Add to map
        _connections.insert(std::make_pair(endPoint, widgetConnection));
    }

}

void BuyingModeSessionWidget::removeConnections(const libtorrent::tcp::endpoint & endPoint) {

    // Remove from map
    auto it = _connections.find(endPoint);

    assert(it != _connections.cend());

    _connections.erase(it);

    // Remove from table view model
    int row = it->second->row();

    if(row != -1)
        _buyers.removeRow(row);

    // Delete connection
    delete it->second;
}

void BuyingModeSessionWidget::updateTerms() {
    // ** show the ui for doign this, when click happen **
}

/// BuyingModeSessionWidget::Connection

BuyingModeSessionWidget::Connection * BuyingModeSessionWidget::Connection::create(BuyingModeSessionWidget * parent,
                                                                                  const BitcoinDisplaySettings * settings,
                                                                                  QStandardItemModel * itemModel,
                                                                                  const std::shared_ptr<core::Connection> & c) {

    // Create items
    QStandardItem * hostItem  = new QStandardItem(),
                  * stateItem = new QStandardItem(),
                  * fundsItem = new QStandardItem(),
                  * lockItem = new QStandardItem(),
                  * priceItem = new QStandardItem(),
                  * numberOfPayments = new QStandardItem(),
                  * balance = new QStandardItem();

    // Center content
    hostItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    stateItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    fundsItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    lockItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    priceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    numberOfPayments->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    balance->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Don´t allow edits
    hostItem->setEditable(false);
    stateItem->setEditable(false);
    fundsItem->setEditable(false);
    lockItem->setEditable(false);
    priceItem->setEditable(false);
    numberOfPayments->setEditable(false);
    balance->setEditable(false);

    // Add row to view model, which takes ownership of items
    QList<QStandardItem *> items;

    items << hostItem
          << stateItem
          << fundsItem
          << lockItem
          << priceItem
          << numberOfPayments
          << balance;

    itemModel->appendRow(items);

    // Create connection
    BuyingModeSessionWidget::Connection * connection = new BuyingModeSessionWidget::Connection(parent,
                                                                                               settings,
                                                                                               hostItem,
                                                                                               stateItem,
                                                                                               fundsItem,
                                                                                               lockItem,
                                                                                               priceItem,
                                                                                               numberOfPayments,
                                                                                               balance);

    /// Set inital values
    connection->setHost(c->connectionId());

    auto machine = c->machine();

    connection->setState(machine->innerStateTypeIndex());

    auto payor = machine->payor();

    // <Perhaps we should only set these if the state is suitable>
    connection->setFunds(payor->funds());
    connection->setRefundLockTime(payor->refundLockTime());
    connection->setPrice(payor->price());
    connection->setNumberOfPayments(payor->numberOfPaymentsMade());
    connection->setBalance(payor->price() * payor->numberOfPaymentsMade());
    connection->setAnnouncedModeAndTerms(machine->announcedModeAndTermsFromPeer());

    /// Connect model signals to our slots
    core::CBStateMachine * rawMachine = machine.get();

    QObject::connect(rawMachine, &core::CBStateMachine::innerStateTypeIndexChanged,
                     connection, &Connection::setState);

    QObject::connect(rawMachine, &core::CBStateMachine::announcedModeAndTermsFromPeerChanged,
                     connection, &Connection::setAnnouncedModeAndTerms);

    core::Payor * rawPayor = payor.get();

    QObject::connect(rawPayor, &core::Payor::fundsChanged,
                     connection, &Connection::setFunds);

    QObject::connect(rawPayor, &core::Payor::refundLockTimeChanged,
                     connection, &Connection::setRefundLockTime);

    QObject::connect(rawPayor, &core::Payor::priceChanged,
                     connection, [connection, payor](quint64 price) {
        connection->setPrice(price);
        connection->setBalance(price * payor->numberOfPaymentsMade());
    });

    QObject::connect(rawPayor, &core::Payor::numberOfPaymentsMadeChanged,
                     connection, [connection, payor](quint64 n) {
        connection->setNumberOfPayments(n);
        connection->setBalance(payor->price() * n);
    });

    return connection;
}

BuyingModeSessionWidget::Connection::Connection(BuyingModeSessionWidget * parent,
                                                const BitcoinDisplaySettings * settings,
                                                QStandardItem * hostItem,
                                                QStandardItem * stateItem,
                                                QStandardItem * fundsItem,
                                                QStandardItem * lockItem,
                                                QStandardItem * priceItem,
                                                QStandardItem * numberOfPayments,
                                                QStandardItem * balance)
    : QObject(parent)
    , _settings(settings)
    , _hostItem(hostItem)
    , _stateItem(stateItem)
    , _fundsItem(fundsItem)
    , _lockItem(lockItem)
    , _priceItem(priceItem)
    , _numberOfPayments(numberOfPayments)
    , _balance(balance) {
}

int BuyingModeSessionWidget::Connection::row() const noexcept {
    return _hostItem->row();
}

void BuyingModeSessionWidget::Connection::setHost(const libtorrent::tcp::endpoint & endPoint) {
    std::string hostString = libtorrent::print_endpoint(endPoint);
    _hostItem->setText(QString::fromStdString(hostString));
}

void BuyingModeSessionWidget::Connection::setState(const std::type_index & index) {

    _stateItem->setText(Common::toString(index));

    // blank out or set other fields depending on new state
    // Q: how do we get values for other fields if we need then?,
    // seems we may possibly need to hold on to the core::Connection instance.

    // typeid(protocol_statemachine::ChooseMode) == index
    // ...

}

void BuyingModeSessionWidget::Connection::setFunds(quint64 funds) {
    _priceItem->setText(BitcoinRepresentation(funds).toString(_settings));
}

void BuyingModeSessionWidget::Connection::setRefundLockTime(quint64 refundLockTime) {
    _lockItem->setText(QString::number(refundLockTime));
}

void BuyingModeSessionWidget::Connection::setPrice(quint64 price) {
    _priceItem->setText(BitcoinRepresentation(price).toString(_settings));
}

void BuyingModeSessionWidget::Connection::setNumberOfPayments(quint64 numberOfPayments) {
    _numberOfPayments->setText(QString::number(numberOfPayments));
}

void BuyingModeSessionWidget::Connection::setBalance(qint64 balance) {
    _balance->setText(BitcoinRepresentation(balance).toString(_settings));
}

void BuyingModeSessionWidget::Connection::setAnnouncedModeAndTerms(const protocol_statemachine::AnnouncedModeAndTerms & announcedTerms) {

    // Show if peer is seller, otherwise hide
    if(announcedTerms.modeAnnounced() == protocol_statemachine::ModeAnnounced::sell)
        emit show(row());
    else
        emit hide(row());
}

}
}
}
