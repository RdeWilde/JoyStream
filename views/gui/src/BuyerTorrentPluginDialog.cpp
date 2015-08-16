/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/BuyerTorrentPluginDialog.hpp>
#include "ui_BuyerTorrentPluginDialog.h"
#include <gui/ChannelView.hpp>
#include <gui/BuyerPeerPluginView.hpp>
#include <core/controller/BuyerTorrentPluginViewModel.hpp>
#include <core/controller/BuyerPeerPluginViewModel.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <libtorrent/socket_io.hpp> // print_endpoint

/**
 * BuyerTorrentDialog
 */

BuyerTorrentPluginDialog::BuyerTorrentPluginDialog(QWidget * parent,
                                                   const BuyerTorrentPluginViewModel * model,
                                                   const BitcoinDisplaySettings * settings)
    : QDialog(parent)
    , ui(new Ui::BuyerTorrentDialog)
    , _settings(settings) {

    // Setup Qt ui
    ui->setupUi(this);

    const PayorViewModel * payorViewModel = model->payorViewModel();

    // Make contract label clickable
    //ui->contractTxIdLabel->setTextFormat(Qt::RichText);
    //ui->contractTxIdLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    //ui->contractTxIdLabel->setOpenExternalLinks(true);

    // Main fields
    updatePluginState(model->state());
    updatePayorState(payorViewModel->state());
    updateUtxo(payorViewModel->utxo());
    updateContractTxId(payorViewModel->contractTxId());

    // Hide top status label,no use
    ui->extensionStateLabel->setVisible(false);

    // Make selectable
    ui->stateLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->utxoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->contractTxIdLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    /**
     * Connect model signals to view slots
     */

    // addChannel, in future

    QObject::connect(model,
                     SIGNAL(peerAdded(const BuyerPeerPluginViewModel*)),
                     this,
                     SLOT(addPeer(const BuyerPeerPluginViewModel*)));

    QObject::connect(model,
                     SIGNAL(peerRemoved(libtorrent::tcp::endpoint)),
                     this,
                     SLOT(removePeer(libtorrent::tcp::endpoint)));

    QObject::connect(model,
                     SIGNAL(stateChanged(BuyerTorrentPlugin::State)),
                     this,
                     SLOT(updatePluginState(BuyerTorrentPlugin::State)));

    QObject::connect(payorViewModel,
                     SIGNAL(stateChanged(Payor::State)),
                     this,
                     SLOT(updatePayorState(Payor::State)));

    QObject::connect(payorViewModel,
                     SIGNAL(contractTxIdChanged(TxId)),
                     this,
                     SLOT(updateContractTxId(TxId)));

    QObject::connect(payorViewModel,
                     SIGNAL(utxoChanged(UnspentP2PKHOutput)),
                     this,
                     SLOT(updateUtxo(UnspentP2PKHOutput)));

    /**
     * Channels
     */

    // Add columns to channel table view model
    QStringList channelTableColumnNames;

    channelTableColumnNames << "Index"
                            << "State"
                            << "Funds"
                            << "Lock"
                            << "Price"
                            << "#Payments"
                            << "Balance";

    _channelTableViewModel.setHorizontalHeaderLabels(channelTableColumnNames);

    // Create channel views and add to table model
    QVector<ChannelViewModel *> channelViewModels = payorViewModel->channelViewModels();

    for(QVector<ChannelViewModel *>::const_iterator
        i = channelViewModels.constBegin(),
        end = channelViewModels.constEnd();
        i != end;i++)
        _channelViews.append(new ChannelView(this, *i, &_channelTableViewModel, _settings));

    // Set model for channels table
    ui->channelsTableView->setModel(&_channelTableViewModel);

    /**
     * Peer plugins
     */
    // Add columns to peer plugin table view model
    QStringList buyerPeerPluginTableColumnNames;
    buyerPeerPluginTableColumnNames << "Host"
                                    << "State"
                                    << "Slot";

    _buyerPeerPluginTableViewModel.setHorizontalHeaderLabels(buyerPeerPluginTableColumnNames);

    // Create peer plugin views and connect to table view model
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> peerViewModels = model->buyerPeerPluginViewModels();

    for(QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *>::const_iterator
        i = peerViewModels.constBegin(),
        end = peerViewModels.constEnd();
        i != end;i++)
        addPeer(i.value());

    // Set model for channels table
    ui->peerPluginsTableView->setModel(&_buyerPeerPluginTableViewModel);
}

QString BuyerTorrentPluginDialog::pluginStateToString(BuyerTorrentPlugin::State state) {

    QString text;

    switch(state) {

        case BuyerTorrentPlugin::State::waiting_for_payor_to_be_ready:
            text = "Contract being constructed";
            break;
        case BuyerTorrentPlugin::State::downloading_pieces:
            text = "Downloading";
            break;
        case BuyerTorrentPlugin::State::done:
            text = "Finished";
            break;
    }

    return text;
}

QString BuyerTorrentPluginDialog::payorStateToString(Payor::State state) {

    QString text;

    switch(state) {

        case Payor::State::waiting_for_full_set_of_sellers:
            text = "Waiting for sellers";
            break;

        case Payor::State::waiting_for_full_set_of_refund_signatures:
            text = "Waiting for all refunds";
            break;

        case Payor::State::can_pay:
            text = "Can pay on channel";
            break;

        case Payor::State::all_contract_outputs_spent:
            text = "All channels closed";
            break;
    }

    return text;
}

QString BuyerTorrentPluginDialog::contractTxIdToString(const Coin::TransactionId & id) {
    return id.toHex();
}

QString BuyerTorrentPluginDialog::utxoToString(const Coin::UnspentP2PKHOutput & utxo) {
    return utxo.outPoint().toString();
}

BuyerTorrentPluginDialog::~BuyerTorrentPluginDialog() {
    delete ui;
}

void BuyerTorrentPluginDialog::addPeer(const BuyerPeerPluginViewModel * model) {

    libtorrent::tcp::endpoint endPoint = model->endPoint();

    Q_ASSERT(!_buyerPeerPluginViews.contains(endPoint));

    // Creating row items
    QStandardItem * endPointItem = new QStandardItem(),
                  * clientStateItem = new QStandardItem(),
                  * payorSlotItem = new QStandardItem();

    // Create item row
    QList<QStandardItem *> items;

    items << endPointItem
          << clientStateItem
          << payorSlotItem;

    // Add row to model
    _buyerPeerPluginTableViewModel.appendRow(items);

    // Create peer view
    _buyerPeerPluginViews[endPoint] = new BuyerPeerPluginView(this,
                                                              model,
                                                              endPointItem,
                                                              clientStateItem,
                                                              payorSlotItem);
}

void BuyerTorrentPluginDialog::removePeer(const libtorrent::tcp::endpoint & endPoint) {

    Q_ASSERT(_buyerPeerPluginViews.contains(endPoint));

    // Take out peer plugin view
    BuyerPeerPluginView * view = _buyerPeerPluginViews.take(endPoint);

    // Get index of endpoint item
    QModelIndex index = _buyerPeerPluginTableViewModel.indexFromItem(view->endPointItem());

    // Release row for this item, which corresponds to peer view
    _buyerPeerPluginTableViewModel.removeRow(index.row());

    // Delete view
    delete view;
}

void BuyerTorrentPluginDialog::updatePluginState(BuyerTorrentPlugin::State state) {
    ui->extensionStateLabel->setText(pluginStateToString(state));
}

void BuyerTorrentPluginDialog::updatePayorState(Payor::State state) {
    ui->stateLabel->setText(payorStateToString(state));
}

void BuyerTorrentPluginDialog::updateContractTxId(const Coin::TransactionId & id) {
    //ui->contractTxIdLabel->setText("<a href=\"http://explorer.chain.com/transactions\">" + contractTxIdToString(id) + "</a>");

    ui->contractTxIdLabel->setText(contractTxIdToString(id));
}

void BuyerTorrentPluginDialog::updateUtxo(const Coin::UnspentP2PKHOutput & utxo) {
    ui->utxoLabel->setText(utxoToString(utxo));
}

QStandardItemModel * BuyerTorrentPluginDialog::channelTableViewModel() {
    return &_channelTableViewModel;
}

QStandardItemModel * BuyerTorrentPluginDialog::buyerPeerPluginTableViewModel() {
    return &_buyerPeerPluginTableViewModel;
}

QVector<ChannelView *> BuyerTorrentPluginDialog::channelViews() const {
    return _channelViews;
}

QMap<libtorrent::tcp::endpoint, BuyerPeerPluginView *> BuyerTorrentPluginDialog::buyerPeerPluginViews() const {
    return _buyerPeerPluginViews;
}
