/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SELLER_TORRENT_PLUGIN_CONFIGURATION_DIALOG_HPP
#define SELLER_TORRENT_PLUGIN_CONFIGURATION_DIALOG_HPP

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/torrent_info.hpp> // torrent_info
#include <QDialog>

class Controller;
class OldWallet;
class BitCoinDisplaySettings;

namespace Ui {
class SellerTorrentPluginConfigurationDialog;
}

class SellerTorrentPluginConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    //explicit SellerTorrentPluginConfigurationDialog(QWidget *parent = 0);

    SellerTorrentPluginConfigurationDialog(Controller * controller, OldWallet * wallet, const libtorrent::torrent_info & torrentInfo, const BitCoinDisplaySettings * settings);

    ~SellerTorrentPluginConfigurationDialog();

    // Try to read off ui fields
    // Return value indicates validity of corresponding ui field,
    // and passed reference is only modified with valid value if
    // return value is true
    bool tryToGetPricePrGB(quint64 & minSatoshiesPrGB) const;

    // Compute minPrice from price pr. GB (in satoshies)
    quint32 minPriceFromPricePrGB(quint64 pricePrGB) const;

private slots:
    void on_buttonBox_accepted();

    void on_minPriceLineEdit_textChanged(const QString &arg1);

private:

    Ui::SellerTorrentPluginConfigurationDialog *ui;

    Controller * _controller;

    OldWallet * _wallet;

    const BitCoinDisplaySettings * _settings;

    // Hash of torrent
    //libtorrent::sha1_hash _infoHash;

    // Torrent file
    libtorrent::torrent_info _torrentInfo;

    // Updates min price field based on present input fields
    void updateComputedSatoshiesMinPriceValue();
};

#endif // SELLER_TORRENT_PLUGIN_CONFIGURATION_DIALOG_HPP
