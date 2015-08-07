/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef WALLETDIALOG_HPP
#define WALLETDIALOG_HPP

#include <QDialog>
#include <QStandardItemModel>

#include "extension/BitCoin/Wallet.hpp" // Required to forward declare Wallet::TxOEvent

namespace Ui {
class WalletDialog;
}

class Wallet;
class QMenu;
class BitCoinDisplaySettings;

class WalletDialog : public QDialog
{
    Q_OBJECT

public:
    //explicit WalletDialog(QWidget *parent = 0)

    WalletDialog(Wallet * wallet, const BitCoinDisplaySettings * settings);
    ~WalletDialog();

    // Updates view
    void refresh();

    // Clear view model table
    void clearWalletTableView();

    // Clears and repopulates walletTableView
    void updateWalletTableView();

    //QList<QStandardItem *> toModelViewRow(const Wallet::TxOEvent & event) const;

private slots:
    void on_receivePushButton_clicked();

    void on_synchronizePushButton_clicked();

private:
    Ui::WalletDialog *ui;

    Wallet * _wallet;

    const BitCoinDisplaySettings * _settings;

    /**
     * View-models
     */

    QStandardItemModel _walletTableViewModel; // View model
    QMenu * _walletTableContextMenu; // Context menu
    QModelIndex _walletTableLastIndexClicked; // Last model index for mouse click

    //std::map<libtorrent::sha1_hash, TorrentViewModel *> _torrentViewModels; // Maps info_hash of models to corresponding TorrentViewModel
};

#endif // WALLETDIALOG_HPP
