#include "BuyerTorrentPluginConfigurationDialog.hpp"
#include "ui_BuyerTorrentPluginConfigurationDialog.h"
#include "extension/PluginMode.hpp"

//#include "extension/TorrentPluginConfiguration.hpp"
#include "extension/BuyerTorrentPlugin.hpp"

#include "controller/Controller.hpp"

#include "extension/BitCoin/BitSwaprjs.hpp"
#include "extension/BitCoin/UnspentP2PKHOutput.hpp"

#include <QtMath>
#include <QMessageBox>

BuyerTorrentPluginConfigurationDialog::BuyerTorrentPluginConfigurationDialog(Controller * controller, const libtorrent::torrent_info & torrentInfo)
    : ui(new Ui::BuyerTorrentPluginConfigurationDialog)
    , _controller(controller)
    , _torrentInfo(torrentInfo) {

    ui->setupUi(this);
}

BuyerTorrentPluginConfigurationDialog::~BuyerTorrentPluginConfigurationDialog() {
    delete ui;
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // Get max price
    quint64 maxPrice = ui->maxPriceLineEdit->text().toInt();

    // Get funding output - this has to be grabbed from wallet/chain later
    quint64 minimalFunds = _torrentInfo.num_pieces()*maxPrice;
    UnspentP2PKHOutput utxo = BitSwaprjs::get_utxo(minimalFunds);

    // Check that an utxo was indeed found
    if(utxo.fundingValue() == 0) {

        // Show modal dialog on same thread, we block untill it is closed
        QMessageBox msgBox;
        msgBox.setText("No utxo found.");
        msgBox.exec();

        return;
    }

    // Figure out how much to lock up with each seller
    qint32 numberOfSellers = ui->minPeersLineEdit->text().toInt();
    quint64 fundingPerSeller = qFloor(utxo.fundingValue()/numberOfSellers);
    quint64 tx_fee = 0; // Get from where?
    quint64 changeValue = utxo.fundingValue() - numberOfSellers*fundingPerSeller - tx_fee;

    // Get other values
    QTime maxLockTime = ui->maxLockTimeEdit->time();
    quint32 maxLock = maxLockTime.hour()*3600 + maxLockTime.minute()*60 + maxLockTime.second();
    //quint64 maxFeePerByte = ui->feeLineEdit->text().toInt();

    // Use this for something?
    //QTime waitTime = ui->waitTimeTimeEdit->time();

    // Create configuration
    BuyerTorrentPlugin::Configuration * configuration = new BuyerTorrentPlugin::Configuration(QVector<quint64>(numberOfSellers, fundingPerSeller),
                                                                                              changeValue,
                                                                                              utxo.fundingOutput(),
                                                                                              utxo.fundingOutputKeyPair(),
                                                                                              maxPrice,
                                                                                              maxLock);

    // Set in seller mode
    _controller->startTorrentPlugin(_torrentInfo.info_hash(), configuration);

    // close window
    done(0);
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_rejected() {

    // Close window
    done(0);
}
