#include "BuyerTorrentPluginConfigurationDialog.hpp"
#include "ui_BuyerTorrentPluginConfigurationDialog.h"
#include "extension/PluginMode.hpp"
#include "extension/TorrentPluginConfiguration.hpp"
#include "controller/Controller.hpp"

/*
BuyerTorrentPluginConfigurationDialog::BuyerTorrentPluginConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuyerTorrentPluginConfigurationDialog)
{
    ui->setupUi(this);
}
*/

BuyerTorrentPluginConfigurationDialog::BuyerTorrentPluginConfigurationDialog(Controller * controller, const libtorrent::sha1_hash & infoHash)
    : ui(new Ui::BuyerTorrentPluginConfigurationDialog)
    , _controller(controller)
    , _infoHash(infoHash) {

    ui->setupUi(this);
}

BuyerTorrentPluginConfigurationDialog::~BuyerTorrentPluginConfigurationDialog() {
    delete ui;
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // Grab fields
    quint32 maxPrice = ui->maxPriceLineEdit->text().toInt();
    QTime maxLock = ui->maxLockTimeEdit->time();
    QTime waitTime = ui->waitTimeTimeEdit->time();
    qint8 minPeers = ui->minPeersLineEdit->text().toInt();
    quint32 fee = ui->feeLineEdit->text().toInt();

    // Create configuration
    TorrentPluginConfiguration * configuration = new TorrentPluginConfiguration(true,
                                                                                maxPrice,
                                                                                maxLock,
                                                                                waitTime,
                                                                                minPeers,
                                                                                fee);

    // Set in seller mode
    _controller->updateTorrentPluginConfiguration(_infoHash, configuration);

    // close window
    done(0);
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_rejected() {

    // Close window
    done(0);
}
