/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP
#define BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP

#include <QDialog>

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/torrent_info.hpp>

namespace Ui {
class BuyerTorrentPluginConfigurationDialog;
}

class Controller;
class OldWallet;
class BitCoinDisplaySettings;

class BuyerTorrentPluginConfigurationDialog : public QDialog
{
    Q_OBJECT

public:

    //explicit BuyerTorrentPluginConfigurationDialog(QWidget *parent = 0);

    BuyerTorrentPluginConfigurationDialog(Controller * controller, OldWallet * wallet, const libtorrent::torrent_info & torrentInfo, const BitCoinDisplaySettings * settings);

    // Destructor
    ~BuyerTorrentPluginConfigurationDialog();

    // Try to read off ui fields.
    // Return value indicates validity of corresponding ui field,
    // and passed reference is only modified with valid value if
    // return value is true
    bool tryToGetNumberOfSellers(int & numberOfSellers) const;
    bool tryToGetFeePerkB(int & feePerkB) const;
    bool tryToGetMaxTotalSpend(quint64 & maxTotalSpend) const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_maxTotalSpendLineEdit_textChanged(const QString &arg1);

    void on_feePrKbLineEdit_textChanged(const QString &arg1);

    void on_numPeersLineEdit_textEdited(const QString &arg1);

private:
    Ui::BuyerTorrentPluginConfigurationDialog *ui;

    Controller * _controller;

    OldWallet * _wallet;

    libtorrent::torrent_info _torrentInfo;

    const BitCoinDisplaySettings * _settings;

    // Derives maxPrice, for given torrent and parameters
    quint64 maxPriceFromTotalSpend(quint64 maxTotalSpend, int numberOfSellers, quint64 feePerkB);

    // Updates the total expenditure field based on the current configuration values
    void updateTotal();
};

#endif // BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP