#include "BuyerTorrentPluginConfigurationDialog.hpp"
#include "ui_BuyerTorrentPluginConfigurationDialog.h"
#include "extension/PluginMode.hpp"

//#include "extension/TorrentPluginConfiguration.hpp"
#include "extension/BuyerTorrentPlugin.hpp"

#include "controller/Controller.hpp"

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
    bool enableBanningSets = true;
    BuyerTorrentPlugin::State state = BuyerTorrentPlugin::State::building_contract;
    quint64 maxPrice = ui->maxPriceLineEdit->text().toInt();
    //quint32 maxLock = ui->maxLockTimeEdit->time();
    quint32 maxLock = 1;
    quint64 maxFeePerByte = ui->feeLineEdit->text().toInt();
    qint32 minPeers = ui->minPeersLineEdit->text().toInt();

    // Use this for something?
    QTime waitTime = ui->waitTimeTimeEdit->time();

    // Create configuration
    BuyerTorrentPlugin::Configuration * configuration = new BuyerTorrentPlugin::Configuration(enableBanningSets, state, maxPrice, maxLock, maxFeePerByte, minPeers);

    // Set in seller mode
    _controller->startTorrentPlugin(_infoHash, configuration);

    // close window
    done(0);
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_rejected() {

    // Close window
    done(0);
}
