/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef BUYER_TORRENT_DIALOG_HPP
#define BUYER_TORRENT_DIALOG_HPP

#include "extension/BuyerTorrentPlugin.hpp"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class BuyerTorrentDialog;
}

class BuyerTorrentPluginViewModel;
class ChannelView;
class BuyerPeerPluginView;
class BuyerPeerPluginViewModel;
class BitcoinDisplaySettings;

class BuyerTorrentPluginDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    BuyerTorrentPluginDialog(QWidget * parent,
                             const BuyerTorrentPluginViewModel * model,
                             const BitcoinDisplaySettings * settings);

    // Text conversion routines
    static QString pluginStateToString(BuyerTorrentPlugin::State state);
    static QString payorStateToString(Payor::State state);
    static QString contractTxIdToString(const Coin::TransactionId & id);
    static QString utxoToString(const Coin::UnspentP2PKHOutput & utxo);

    // Destructor
    ~BuyerTorrentPluginDialog();

    // Getters
    QStandardItemModel * channelTableViewModel();
    QStandardItemModel * buyerPeerPluginTableViewModel();
    QVector<ChannelView *> channelViews() const;
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginView *> buyerPeerPluginViews() const;

public slots:

    // Adds channels and peers,
    //void addChannel(const ChannelViewModel * model);
    void addPeer(const BuyerPeerPluginViewModel * model);
    void removePeer(const libtorrent::tcp::endpoint & endPoint);

    // Update
    void updatePluginState(BuyerTorrentPlugin::State state);
    void updatePayorState(Payor::State state);
    void updateContractTxId(const Coin::TransactionId & id);
    void updateUtxo(const Coin::UnspentP2PKHOutput & utxo);

private:

    Ui::BuyerTorrentDialog *ui;

    // Display settings for bitcoins
    const BitcoinDisplaySettings * _settings;

    // Payor channel table view model
    QStandardItemModel _channelTableViewModel;

    // Buyer peer plugin table view model
    QStandardItemModel _buyerPeerPluginTableViewModel;

    // Views for channels
    // Pointers are owned by *this object
    QVector<ChannelView *> _channelViews;

    // Views for buyer peer plugins
    // Pointers are owned by *this object
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginView *> _buyerPeerPluginViews;

};

#endif // BUYER_TORRENT_DIALOG_HPP
