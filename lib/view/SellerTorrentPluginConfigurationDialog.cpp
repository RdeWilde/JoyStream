#include "SellerTorrentPluginConfigurationDialog.hpp"
#include "ui_SellerTorrentPluginConfigurationDialog.h"

#include "controller/Controller.hpp"
//#include "extension/TorrentPluginConfiguration.hpp"
#include "extension/SellerTorrentPlugin.hpp"

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

    // Grab fields
    quint32 minPrice = ui->minPriceLineEdit->text().toInt();

    QTime minLockTime = ui->minLockTimeEdit->time();
    quint32 minLock = minLockTime.hour()*3600 + minLockTime.minute()*60 + minLockTime.second();

    quint32 minFeePerByte = ui->minFeeLineEdit->text().toInt();

    QTime maxContractConfirmationDelayTime = ui->maxContractConfirmationDelayTimeEdit->time();
    quint32 maxContractConfirmationDelay = maxContractConfirmationDelayTime.hour()*3600 + maxContractConfirmationDelayTime.minute()*60 + maxContractConfirmationDelayTime.second();;

    // Create configuration
    SellerTorrentPlugin::Configuration configuration(true,
                                                     minPrice,
                                                     minLock,
                                                     minFeePerByte,
                                                     maxContractConfirmationDelay);

    // Set in seller mode
    _controller->startSellerTorrentPlugin(_torrentInfo.info_hash(), configuration);

    // close window
    done(0);
}
