#include "SellerTorrentPluginConfigurationDialog.hpp"
#include "ui_SellerTorrentPluginConfigurationDialog.h"

#include "controller/Controller.hpp"
//#include "extension/TorrentPluginConfiguration.hpp"
#include "extension/SellerTorrentPlugin.hpp"

#include "extension/BitCoin/BitCoin.hpp"

/*
SellerTorrentPluginConfigurationDialog::SellerTorrentPluginConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SellerTorrentPluginConfigurationDialog)
{
    ui->setupUi(this);
}
*/

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

    // minPrice
    bool okMinPrice;
    quint32 minPrice = SATOSHIES_PER_M_BTC * ui->minPriceLineEdit->text().toInt();

    if(!okMinPrice || minPrice < 0)
        return;

    // minLockTime
    QTime minLockTime = ui->minLockTimeEdit->time();
    quint32 minLock = minLockTime.hour()*3600 + minLockTime.minute()*60 + minLockTime.second();

    // minFeePerKByte
    bool okMinFeePerKByte;
    quint32 minFeePerKByte = ui->minFeeLineEdit->text().toInt(&okMinFeePerKByte);

    if(!okMinFeePerKByte || minFeePerKByte < 0)
        return;

    // maxNumberOfSellers
    bool okMaxNumberOfSellers;
    quint32 maxNumberOfSellers = ui->maxNumberOfSellersLineEdit->text().toInt(&okMaxNumberOfSellers);

    if(!okMaxNumberOfSellers || maxNumberOfSellers < 1)
        return;

    // maxContractConfirmationDelay
    QTime maxContractConfirmationDelayTime = ui->maxConfirmationTimeTimeEdit->time();
    quint32 maxContractConfirmationDelay = maxContractConfirmationDelayTime.hour()*3600 + maxContractConfirmationDelayTime.minute()*60 + maxContractConfirmationDelayTime.second();;

    // Tell controller to start plugin
    _controller->startSellerTorrentPlugin(_torrentInfo.info_hash(), SellerTorrentPlugin::Configuration(true,
                                                                                                        minPrice,
                                                                                                        minLock,
                                                                                                        minFeePerKByte,
                                                                                                        maxNumberOfSellers,
                                                                                                        maxContractConfirmationDelay));

    // close window
    done(0);
}
