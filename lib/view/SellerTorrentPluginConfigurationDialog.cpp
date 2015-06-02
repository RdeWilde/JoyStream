#include "SellerTorrentPluginConfigurationDialog.hpp"
#include "ui_SellerTorrentPluginConfigurationDialog.h"

#include "controller/Controller.hpp"
//#include "extension/TorrentPluginConfiguration.hpp"
#include "extension/SellerTorrentPlugin.hpp"

#include "extension/BitCoin/BitCoin.hpp"

#include <QMessageBox>

SellerTorrentPluginConfigurationDialog::SellerTorrentPluginConfigurationDialog(Controller * controller, Wallet * wallet, const libtorrent::torrent_info & torrentInfo)
    :ui(new Ui::SellerTorrentPluginConfigurationDialog)
    , _controller(controller)
    , _wallet(wallet)
    , _torrentInfo(torrentInfo) {

    ui->setupUi(this);
}

SellerTorrentPluginConfigurationDialog::~SellerTorrentPluginConfigurationDialog() {
    delete ui;
}

void SellerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // Message box used to alert user if invalid value is provided
    QMessageBox msgBox;

    // minPrice
    bool okMinPrice;
    quint32 minSatoshiesPrGB = static_cast<int>(SATOSHIES_PER_M_BTC * ui->minPriceLineEdit->text().toDouble(&okMinPrice));
    float piecesPrGB = ((float)1000*1000*1000) /_torrentInfo.piece_length();

    quint32 minPrice = static_cast<int>(minSatoshiesPrGB/piecesPrGB);

    if(!okMinPrice || minPrice < 0) {

        msgBox.setText("Invalid per GB price value: " + ui->minPriceLineEdit->text());
        msgBox.exec();
        return;
    }

    // minLockTime
    QTime minLockTime = ui->minLockTimeEdit->time();
    quint32 minLock = minLockTime.hour()*3600 + minLockTime.minute()*60 + minLockTime.second();

    // minFeePerKByte
    bool okMinFeePerKByte;
    quint32 minFeePerKByte = static_cast<int>(SATOSHIES_PER_M_BTC * ui->minFeeLineEdit->text().toDouble(&okMinFeePerKByte));

    if(!okMinFeePerKByte || minFeePerKByte < 0) {

        msgBox.setText("Invalid fee per kB: " + ui->minFeeLineEdit->text());
        msgBox.exec();
        return;
    }

    // maxNumberOfSellers
    bool okMaxNumberOfSellers;
    quint32 maxNumberOfSellers = ui->maxNumberOfSellersLineEdit->text().toInt(&okMaxNumberOfSellers);

    if(!okMaxNumberOfSellers || maxNumberOfSellers < 1) {

        msgBox.setText("Invalid max #sellers: " + ui->maxNumberOfSellersLineEdit->text());
        msgBox.exec();
        return;
    }

    // maxContractConfirmationDelay
    QTime maxContractConfirmationDelayTime = ui->maxConfirmationTimeTimeEdit->time();
    quint32 maxContractConfirmationDelay = maxContractConfirmationDelayTime.hour()*3600 + maxContractConfirmationDelayTime.minute()*60 + maxContractConfirmationDelayTime.second();;

    // Tell controller to start plugin
    _controller->startSellerTorrentPlugin(_torrentInfo.info_hash(), SellerTorrentPlugin::Configuration(false,
                                                                                                        minPrice,
                                                                                                        minLock,
                                                                                                        minFeePerKByte,
                                                                                                        maxNumberOfSellers,
                                                                                                        maxContractConfirmationDelay));

    // close window
    done(0);
}
