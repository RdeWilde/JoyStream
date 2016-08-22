/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#include <gui/SellingModeSessionWidget.hpp>
#include <gui/Language.hpp>
#include "ui_SellingModeSessionWidget.h"
#include <common/BitcoinRepresentation.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <protocol_statemachine/protocol_statemachine.hpp>
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace gui {

SellingModeSessionWidget * SellingModeSessionWidget::create(QWidget * parent,
                                                            const BitcoinDisplaySettings * settings,
                                                            const std::shared_ptr<core::Session> & session) {

    if(session->mode() != protocol_session::SessionMode::buying)
        throw std::runtime_error("Session is not in buy mode."); // add type safe later

    // Create widget
    SellingModeSessionWidget * widget = new SellingModeSessionWidget(parent, settings);

    // Set fields
    std::shared_ptr<core::Selling> selling = session->selling();

    widget->setTerms(selling->terms());
    widget->setPolicy(selling->policy());

    // Add Connections
    for(auto mapping : session->connections())
        widget->addConnections(mapping.second);

    // Connect buying mode to widget
    core::Selling * rawSelling = selling.get();

    QObject::connect(rawSelling, &core::Selling::policyChanged,
                     widget, &SellingModeSessionWidget::setPolicy);

    QObject::connect(rawSelling, &core::Selling::termsChanged,
                     widget, &SellingModeSessionWidget::setTerms);

    // Connect session to widget
    core::Session * rawSession = session.get();

    QObject::connect(rawSession, &core::Session::connectionAdded,
                     widget, &SellingModeSessionWidget::addConnections);

    QObject::connect(rawSession, &core::Session::connectionRemoved,
                     widget, &SellingModeSessionWidget::removeConnections);

    return widget;
}

SellingModeSessionWidget::SellingModeSessionWidget(QWidget *parent,
                                                   const BitcoinDisplaySettings * settings)
    : QWidget(parent)
    , ui(new Ui::SellingModeSessionWidget)
    , _settings(settings)
{
    ui->setupUi(this);
}

SellingModeSessionWidget::~SellingModeSessionWidget()
{
    delete ui;
}

void SellingModeSessionWidget::setTerms(const protocol_wire::SellerTerms & terms) {

    ui->minContractFeePrKbValueLabel->setText(BitcoinRepresentation(terms.minContractFeePerKb()).toString(_settings));

    ui->maxNumberOfSellersValueLabel->setText(QString::number(terms.maxSellers()));

    ui->minPriceValueLabel->setText(BitcoinRepresentation(terms.minPrice()).toString(_settings));

    ui->minRefundDelayValueLabel->setText(QString::number(terms.minLock()));
}

void SellingModeSessionWidget::setPolicy(const protocol_session::SellingPolicy &) {
    // nothing to do really
}

void SellingModeSessionWidget::addConnections(const std::weak_ptr<core::Connection> & c) {

    if(std::shared_ptr<core::Connection> connection = c.lock()) {

        libtorrent::tcp::endpoint endPoint = connection->connectionId();

        assert(_connections.count(endPoint) == 0);

        auto * widgetConnection = SellingModeSessionWidget::Connection::create(this, _settings, &_sellers, connection);

        // Add to map
        _connections.insert(std::make_pair(endPoint, widgetConnection));
    }
}

void SellingModeSessionWidget::removeConnections(const libtorrent::tcp::endpoint & endPoint) {

    // Remove from map
    auto it = _connections.find(endPoint);

    assert(it != _connections.cend());

    _connections.erase(it);

    // Remove from table view model
    int row = it->second->row();

    if(row != -1)
        _sellers.removeRow(row);

    // Delete connection
    delete it->second;
}

/// SellingModeSessionWidget::Connection

SellingModeSessionWidget::Connection * SellingModeSessionWidget::Connection::create(SellingModeSessionWidget * parent,
                                                                                    const BitcoinDisplaySettings * settings,
                                                                                    QStandardItemModel * itemModel,
                                                                                    const std::shared_ptr<core::Connection> & c) {

    // Create items
    QStandardItem * hostItem  = new QStandardItem(),
                  * stateItem = new QStandardItem(),
                  * anchorItem = new QStandardItem(),
                  * fundsItem = new QStandardItem(),
                  * lockItem = new QStandardItem(),
                  * priceItem = new QStandardItem(),
                  * numberOfPayments = new QStandardItem(),
                  * balance = new QStandardItem();

    // Center content
    hostItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    stateItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    anchorItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    fundsItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    lockItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    priceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    numberOfPayments->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    balance->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Don´t allow edits
    hostItem->setEditable(false);
    stateItem->setEditable(false);
    anchorItem->setEditable(false);
    fundsItem->setEditable(false);
    lockItem->setEditable(false);
    priceItem->setEditable(false);
    numberOfPayments->setEditable(false);
    balance->setEditable(false);

    // Add row to view model, which takes ownership of items
    QList<QStandardItem *> items;

    items << hostItem
          << stateItem
          << anchorItem
          << fundsItem
          << lockItem
          << priceItem
          << numberOfPayments
          << balance;

    itemModel->appendRow(items);

    // Create connection
    SellingModeSessionWidget::Connection * connection = new SellingModeSessionWidget::Connection(parent,
                                                                                                 settings,
                                                                                                 hostItem,
                                                                                                 stateItem,
                                                                                                 anchorItem,
                                                                                                 fundsItem,
                                                                                                 lockItem,
                                                                                                 priceItem,
                                                                                                 numberOfPayments,
                                                                                                 balance);

    /// Set inital values
    connection->setHost(c->connectionId());

    auto machine = c->machine();

    connection->setState(machine->innerStateTypeIndex());

    auto payee = machine->payee();

    // <Perhaps we should only set these if the state is suitable>
    connection->setAnchor(payee->anchor());
    connection->setFunds(payee->funds());
    connection->setRefundLockTime(payee->lockTime());
    connection->setPrice(payee->price());
    connection->setNumberOfPayments(payee->numberOfPaymentsMade());
    connection->setBalance(payee->price() * payee->numberOfPaymentsMade());
    connection->setAnnouncedModeAndTerms(machine->announcedModeAndTermsFromPeer());

    /// Connect model signals to our slots
    core::CBStateMachine * rawMachine = machine.get();

    QObject::connect(rawMachine, &core::CBStateMachine::innerStateTypeIndexChanged,
                     connection, &Connection::setState);

    QObject::connect(rawMachine, &core::CBStateMachine::announcedModeAndTermsFromPeerChanged,
                     connection, &Connection::setAnnouncedModeAndTerms);

    core::Payee * rawPayee = payee.get();

    QObject::connect(rawPayee, &core::Payee::anchorChanged,
                     connection, &Connection::setAnchor);

    QObject::connect(rawPayee, &core::Payee::fundsChanged,
                     connection, &Connection::setFunds);

    QObject::connect(rawPayee, &core::Payee::lockTimeChanged,
                     connection, &Connection::setRefundLockTime);

    QObject::connect(rawPayee, &core::Payee::priceChanged,
                     connection, [connection, payee](quint64 price) {
        connection->setPrice(price);
        connection->setBalance(price * payee->numberOfPaymentsMade());
    });

    QObject::connect(rawPayee, &core::Payee::numberOfPaymentsMadeChanged,
                     connection, [connection, payee](quint64 n) {
        connection->setNumberOfPayments(n);
        connection->setBalance(payee->price() * n);
    });

    return connection;
}

SellingModeSessionWidget::Connection::Connection(SellingModeSessionWidget * parent,
                                                 const BitcoinDisplaySettings * settings,
                                                 QStandardItem * hostItem,
                                                 QStandardItem * stateItem,
                                                 QStandardItem * anchorItem,
                                                 QStandardItem * fundsItem,
                                                 QStandardItem * lockItem,
                                                 QStandardItem * priceItem,
                                                 QStandardItem * numberOfPayments,
                                                 QStandardItem * balance)
    : QObject(parent)
    , _settings(settings)
    , _hostItem(hostItem)
    , _stateItem(stateItem)
    , _anchorItem(anchorItem)
    , _fundsItem(fundsItem)
    , _lockItem(lockItem)
    , _priceItem(priceItem)
    , _numberOfPayments(numberOfPayments)
    , _balance(balance) {
}

int SellingModeSessionWidget::Connection::row() const noexcept {
    return _hostItem->row();
}

void SellingModeSessionWidget::Connection::setHost(const libtorrent::tcp::endpoint & endPoint) {
    std::string hostString = libtorrent::print_endpoint(endPoint);
    _hostItem->setText(QString::fromStdString(hostString));
}

void SellingModeSessionWidget::Connection::setState(const std::type_index & index) {

    _stateItem->setText(Language::toString(index));

    // blank out or set other fields depending on new state
    // Q: how do we get values for other fields if we need then?,
    // seems we may possibly need to hold on to the core::Connection instance.

    // typeid(protocol_statemachine::ChooseMode) == index
    // ...

}

void SellingModeSessionWidget::Connection::setAnchor(const Coin::typesafeOutPoint & anchor) {
    _anchorItem->setText(anchor.toLittleEndianTxIdString());
}

void SellingModeSessionWidget::Connection::setFunds(quint64 funds) {
    _priceItem->setText(BitcoinRepresentation(funds).toString(_settings));
}

void SellingModeSessionWidget::Connection::setRefundLockTime(quint64 refundLockTime) {
    _lockItem->setText(QString::number(refundLockTime));
}

void SellingModeSessionWidget::Connection::setPrice(quint64 price) {
    _priceItem->setText(BitcoinRepresentation(price).toString(_settings));
}

void SellingModeSessionWidget::Connection::setNumberOfPayments(quint64 numberOfPayments) {
    _numberOfPayments->setText(QString::number(numberOfPayments));
}

void SellingModeSessionWidget::Connection::setBalance(qint64 balance) {
    _balance->setText(BitcoinRepresentation(balance).toString(_settings));
}

void SellingModeSessionWidget::Connection::setAnnouncedModeAndTerms(const protocol_statemachine::AnnouncedModeAndTerms & announcedTerms) {

    // Show if peer is buyer, otherwise hide
    if(announcedTerms.modeAnnounced() == protocol_statemachine::ModeAnnounced::buy)
        emit show(row());
    else
        emit hide(row());
}


}
}
}
