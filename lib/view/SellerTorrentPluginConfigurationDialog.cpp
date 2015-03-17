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

    /**
    // Grab fields
    bool enableBanningSets = true;
    quint32 minPrice = ui->minPriceLineEdit->text().toInt();
    QTime minLock = ui->minLockTimeEdit->time();
    quint32 minFee = ui->minFeeLineEdit->text().toInt();
    QTime maxContractConfirmationDelay = ui->maxContractConfirmationDelayTimeEdit->time();


    enableBanningSets,
    minPrice,
    minLock,
    minFee,
    maxContractConfirmationDelay
    */

    // Create configuration
    SellerTorrentPlugin::Configuration * configuration = new SellerTorrentPlugin::Configuration();

    // Set in seller mode
    _controller->startTorrentPlugin(_torrentInfo.info_hash(), configuration);

    // close window
    done(0);
}
