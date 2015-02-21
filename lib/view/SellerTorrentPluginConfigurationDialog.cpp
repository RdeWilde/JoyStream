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

SellerTorrentPluginConfigurationDialog::SellerTorrentPluginConfigurationDialog(Controller * controller, const libtorrent::sha1_hash & infoHash)
    :ui(new Ui::SellerTorrentPluginConfigurationDialog)
    ,_controller(controller)
    ,_infoHash(infoHash) {
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
    _controller->updateTorrentPluginConfiguration(_infoHash, configuration);

    // close window
    done(0);
}
