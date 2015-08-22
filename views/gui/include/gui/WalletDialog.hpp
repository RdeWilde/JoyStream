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
#include <gui/UtxoEventViewModel.hpp>

#include <QMap>

namespace Ui {
class WalletDialog;
}

namespace Wallet {
    class Manager;
}

class QMenu;
class BitcoinDisplaySettings;

class WalletDialog : public QDialog
{
    Q_OBJECT

public:

    //explicit WalletDialog(QWidget *parent = 0)

    WalletDialog(Wallet::Manager * wallet, const BitcoinDisplaySettings * settings);
    ~WalletDialog();

    // Updates view
    void refresh();

    // Clear view model table
    //void clearWalletTableView();

    // Clears and repopulates walletTableView
    void updateWalletTableView();

    //QList<QStandardItem *> toModelViewRow(const Wallet::TxOEvent & event) const;

private slots:

    void on_receivePushButton_clicked();

    void on_synchronizePushButton_clicked();

    void updateNumberOfKeysInWallet(quint64 numberOfKeys);

    void updateNumberOfTransactions(quint64 numberOfTransactions);

private:
    Ui::WalletDialog *ui;

    Wallet::Manager * _wallet;

    const BitcoinDisplaySettings * _settings;

    QList<UtxoEventViewModel> _utxoViewModels;

    /**
     * View-models
     */

    QStandardItemModel _walletTableViewModel; // View model

    QMenu * _walletTableContextMenu; // Context menu
    QModelIndex _walletTableLastIndexClicked; // Last model index for mouse click

    //std::map<libtorrent::sha1_hash, TorrentViewModel *> _torrentViewModels; // Maps info_hash of models to corresponding TorrentViewModel
};

#endif // WALLETDIALOG_HPP
