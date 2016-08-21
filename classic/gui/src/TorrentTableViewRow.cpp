/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#include <gui/TorrentTableViewRow.hpp>
#include <gui/Language.hpp>
#include <core/core.hpp>
#include <common/DataSizeRepresentation.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

TorrentTableViewRow * TorrentTableViewRow::create(QStandardItemModel * itemModel,
                                                const std::shared_ptr<core::Torrent> & torrentModel,
                                                const BitcoinDisplaySettings * settings) {

    // Create items
    QStandardItem * nameItem = new QStandardItem(),
                  * sizeItem = new QStandardItem(),
                  * stateItem = new QStandardItem(),
                  * uploadSpeedItem = new QStandardItem(),
                  * downloadSpeedItem = new QStandardItem(),
                  * numberOfBuyerPeersItem = new QStandardItem(),
                  * numberOfSellerPeersitem = new QStandardItem(),
                  * sessionModeItem = new QStandardItem(),
                  * balanceItem = new QStandardItem();

    // Center content
    nameItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    sizeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    stateItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    uploadSpeedItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    downloadSpeedItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    numberOfBuyerPeersItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    numberOfSellerPeersitem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    sessionModeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    balanceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Don't allow edits
    nameItem->setEditable(false);
    sizeItem->setEditable(false);
    stateItem->setEditable(false);
    uploadSpeedItem->setEditable(false);
    downloadSpeedItem->setEditable(false);
    numberOfBuyerPeersItem->setEditable(false);
    numberOfSellerPeersitem->setEditable(false);
    sessionModeItem->setEditable(false);
    balanceItem->setEditable(false);

    // Add row to view model, which takes ownership of items
    QList<QStandardItem *> items;

    items << nameItem
          << sizeItem
          << stateItem
          << uploadSpeedItem
          << downloadSpeedItem
          << numberOfBuyerPeersItem
          << numberOfSellerPeersitem
          << sessionModeItem
          << balanceItem;

    itemModel->appendRow(items);

    // Create torrent view model
    TorrentTableViewRow * torrentTreeViewRow = new TorrentTableViewRow(itemModel,
                                                                     settings,
                                                                     nameItem,
                                                                     sizeItem,
                                                                     stateItem,
                                                                     uploadSpeedItem,
                                                                     downloadSpeedItem,
                                                                     numberOfBuyerPeersItem,
                                                                     numberOfSellerPeersitem,
                                                                     sessionModeItem,
                                                                     balanceItem);

    /// Set inital values

    torrentTreeViewRow->setName(torrentModel->name());

    if(torrentModel->isPaused())
        torrentTreeViewRow->setPaused(true);
    else
        torrentTreeViewRow->setState(torrentModel->state(), torrentModel->progress());

    torrentTreeViewRow->setUploadSpeed(torrentModel->uploadRate());
    torrentTreeViewRow->setDownloadSpeed(torrentModel->downloadRate());

    // When plugin is present, then set it up, otherwise set it as absent
    if(torrentModel->torrentPlugin().get())
        torrentTreeViewRow->setTorrentPluginPresent(torrentModel->torrentPlugin());
    else
        torrentTreeViewRow->setTorrentPluginAbsent();

    /// Connect model signals to our slots

    core::Torrent * rawTorrent = torrentModel.get();

    QObject::connect(rawTorrent, SIGNAL(torrentPluginAdded(std::weak_ptr<core::TorrentPlugin>)),
                     torrentTreeViewRow, SLOT(setTorrentPluginPresent(std::weak_ptr<core::TorrentPlugin>)));

    QObject::connect(rawTorrent, SIGNAL(torrentPluginRemoved()),
                     torrentTreeViewRow, SLOT(setTorrentPluginAbsent()));

    QObject::connect(rawTorrent, SIGNAL(stateChanged(libtorrent::torrent_status::state_t,float)),
                     torrentTreeViewRow, SLOT(setState(libtorrent::torrent_status::state_t,float)));

    QObject::connect(rawTorrent, SIGNAL(pausedChanged(bool)),
                     torrentTreeViewRow, SLOT(setPaused(bool)));

    QObject::connect(rawTorrent, SIGNAL(downloadRateChanged(int)),
                     torrentTreeViewRow, SLOT(setDownloadSpeed(int)));

    QObject::connect(rawTorrent, SIGNAL(uploadLimitChanged(int)),
                     torrentTreeViewRow, SLOT(setUploadSpeed(int)));

    return torrentTreeViewRow;
}

TorrentTableViewRow::TorrentTableViewRow(QObject * parent,
                                       const BitcoinDisplaySettings * settings,
                                       QStandardItem * nameItem,
                                       QStandardItem * sizeItem,
                                       QStandardItem * stateItem,
                                       QStandardItem * uploadSpeed,
                                       QStandardItem * downloadSpeed,
                                       QStandardItem * numberOfBuyerPeersItem,
                                       QStandardItem * numberOfSellerPeersitem,
                                       QStandardItem * sessionModeItem,
                                       QStandardItem * balanceItem)
    : QObject(parent)
    , _settings(settings)
    , _nameItem(nameItem)
    , _sizeItem(sizeItem)
    , _stateItem(stateItem)
    , _uploadSpeedItem(uploadSpeed)
    , _downloadSpeedItem(downloadSpeed)
    , _numberOfBuyerPeersItem(numberOfBuyerPeersItem)
    , _numberOfSellerPeersitem(numberOfSellerPeersitem)
    , _sessionModeItem(sessionModeItem)
    , _balanceItem(balanceItem) {
}


void TorrentTableViewRow::setName(const std::string & name) {
    _nameItem->setText(QString::fromStdString(name));
}

void TorrentTableViewRow::setSize(qint64 totalSize) {
    _sizeItem->setText(DataSizeRepresentation(totalSize, DataSizeRepresentation::Base::Byte).toString());
}

void TorrentTableViewRow::setState(libtorrent::torrent_status::state_t state, float progress) {
    _stateItem->setText(Language::toString(state, progress));
}

void TorrentTableViewRow::setPaused(bool paused) {

    if(paused)
        _stateItem->setText(tr("Paused"));
    else
        _stateItem->setText(tr("Resuming")); // we must get old state refershed, so lets just wait for new state update
}

void TorrentTableViewRow::setUploadSpeed(int speed) {
    _uploadSpeedItem->setText(DataSizeRepresentation(speed, DataSizeRepresentation::Base::Byte).toString() + "/s");
}

void TorrentTableViewRow::setDownloadSpeed(int speed) {
    _downloadSpeedItem->setText(DataSizeRepresentation(speed, DataSizeRepresentation::Base::Byte).toString() + "/s");
}

void TorrentTableViewRow::setNumberOfBuyers(quint32 num) {
    _numberOfBuyerPeersItem->setText(QString::number(num));
}

void TorrentTableViewRow::setNumberOfSellers(quint32 num) {
    _numberOfSellerPeersitem->setText(QString::number(num));
}

void TorrentTableViewRow::setSessionMode(protocol_session::SessionMode mode) {
    _sessionModeItem->setText(Language::toString(mode));
}

void TorrentTableViewRow::setBalance(qint64 balance) {
    _balanceItem->setText(BitcoinRepresentation(balance).toString(_settings));
}

void TorrentTableViewRow::setTorrentPluginPresent(const std::weak_ptr<core::TorrentPlugin> & p) {

    if(std::shared_ptr<core::TorrentPlugin> plugin = p.lock()) {

        core::Session * session = plugin->session().get();

        assert(session);

        // Set session mode
        setSessionMode(session->mode());

        // and update when it is changed
        QObject::connect(session,
                         &core::Session::modeChanged,
                         this,
                         [session, this](protocol_session::SessionMode mode) {
                            this->setSessionMode(mode);
                            this->updateBalance(session);
                         });

        // Set initial peer counts
        updateConnectionCounts(session);

        // When a connection is made
        // NB: This is too complicated, may warrant introducing a connection class on the view side
        QObject::connect(session,
                         &core::Session::connectionAdded,
                         this,
                         [session, this](const std::weak_ptr<core::Connection> & c) {

                            // update connectino counts
                            // NB: here we could do more intelligent approach by just subtracting balance in this connection
                            this->updateConnectionCounts(session);

                            // and update balance when a payment is made/received
                            if(const std::shared_ptr<core::Connection> connection = c.lock()) {

                                core::Payor * payor = connection->machine()->payor().get();

                                QObject::connect(payor,
                                                 &core::Payor::numberOfPaymentsMadeChanged,
                                                 this,
                                                 [session, this]() {
                                                    this->updateBalance(session);
                                                 });

                                core::Payee * payee = connection->machine()->payee().get();

                                QObject::connect(payee,
                                                 &core::Payee::numberOfPaymentsMadeChanged,
                                                 this,
                                                 [session, this]() {
                                                    this->updateBalance(session);
                                                 });
                            }

                         });

        // or removed
        QObject::connect(session,
                         &core::Session::connectionRemoved,
                         this,
                         [session, this](const libtorrent::tcp::endpoint &) {

                            // here we could do more intelligent approach by just subtracting balance in this connection
                            this->updateConnectionCounts(session);
                         });

        // Set initial balance
        updateBalance(session);

    } else // plugin has expired, removal signal should arrive promptly
        std::clog << "Ignoring torrent plugin creation announcement, already expired!" << std::endl;
}

void TorrentTableViewRow::setTorrentPluginAbsent() {

    _numberOfBuyerPeersItem->setText("-");
    _numberOfSellerPeersitem->setText("-");
    _sessionModeItem->setText("-");
    _balanceItem->setText("-");

    // Do something to the menu options

}

void TorrentTableViewRow::updateBalance(const core::Session * session) {

    if(session->mode() == protocol_session::SessionMode::buying)
        setBalance(getBalanceInBuyingMode(session));
    else if(session->mode() == protocol_session::SessionMode::selling)
        setBalance(getBalanceInSellingMode(session));
    else
        setBalance(0);
}

int64_t TorrentTableViewRow::getBalanceInBuyingMode(const core::Session * session) {

    int64_t balance = 0;

    // Does not include contract fees,
    // we need to know state information in state machine
    // to properly account for it.

    // Iterate connections and count towards balance
    for(auto mapping : session->connections()) {

        std::shared_ptr<core::CBStateMachine> machine = mapping.second->machine();

        auto payor = machine->payor();

        balance -= payor->numberOfPaymentsMade() * payor->price();
    }

    return balance;
}

int64_t TorrentTableViewRow::getBalanceInSellingMode(const core::Session * session) {

    int64_t balance = 0;

    // Does not include settlement fees,
    // we need to know state information in state machine
    // to properly account for it.

    // Iterate connections and count towards balance
    for(auto mapping : session->connections()) {

        std::shared_ptr<core::CBStateMachine> machine = mapping.second->machine();

        auto payee = machine->payee();

        balance += payee->numberOfPaymentsMade() * payee->price();
    }

    return balance;
}

}
}
}
