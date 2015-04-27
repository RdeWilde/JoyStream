#include "BuyerTorrentPluginConfigurationDialog.hpp"
#include "ui_BuyerTorrentPluginConfigurationDialog.h"
#include "extension/PluginMode.hpp"

//#include "extension/TorrentPluginConfiguration.hpp"
#include "extension/BuyerTorrentPlugin.hpp"

#include "controller/Controller.hpp"
#include "extension/BitCoin/Wallet.hpp"

#include "extension/BitCoin/BitSwaprjs.hpp"
#include "extension/BitCoin/UnspentP2PKHOutput.hpp"

#include <QtMath>
#include <QMessageBox>
#include <QVector>

BuyerTorrentPluginConfigurationDialog::BuyerTorrentPluginConfigurationDialog(Controller * controller, Wallet * wallet, const libtorrent::torrent_info & torrentInfo)
    : ui(new Ui::BuyerTorrentPluginConfigurationDialog)
    , _controller(controller)
    , _wallet(wallet)
    , _torrentInfo(torrentInfo) {

    ui->setupUi(this);

    // Set total price
    on_maxPriceLineEdit_textChanged(ui->maxPriceLineEdit->text());
}

BuyerTorrentPluginConfigurationDialog::~BuyerTorrentPluginConfigurationDialog() {
    delete ui;
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // Number of sellers
    qint32 numberOfSellers = ui->minPeersLineEdit->text().toInt();

    // Get max price
    // https://en.bitcoin.it/wiki/Units
    // 100000 satoshi == 1 mBTC = 1/1000 BTC
    quint64 maxPrice = 100000 * ui->maxPriceLineEdit->text().toInt();

    // How much is required in total at the very least
    // We are putting enough into each channel, change later
    // DOES NOT INCLUDE FEES!!!
    quint64 minimalFunds = numberOfSellers*maxPrice*_torrentInfo.num_pieces();

    // Get funding output - this has to be grabbed from wallet/chain later
    UnspentP2PKHOutput utxo = _wallet->getUtxo(minimalFunds, 1);

    // Check that an utxo was indeed found
    if(utxo.value() == 0) {

        // Show modal dialog on same thread, we block untill it is closed
        QMessageBox msgBox;
        msgBox.setText(QString("No utxo found with value no less than: ") + QString::number(minimalFunds));
        msgBox.exec();

        return;
    }

    // Maximum Lock time
    QTime maxLockTime = ui->maxLockTimeEdit->time();
    quint32 maxLock = maxLockTime.hour()*3600 + maxLockTime.minute()*60 + maxLockTime.second();

    // max fee per byte (satoshi)
    quint64 maxFeePerByte = ui->feeLineEdit->text().toInt();



    BuyerTorrentPlugin::Configuration configuration(false,
                                                    maxPrice,
                                                    maxLock,
                                                    maxFeePerByte,
                                                    numberOfSellers);

    // Set in seller mode
    _controller->startBuyerTorrentPlugin(_torrentInfo.info_hash(), configuration, utxo);

    // close window
    done(0);
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_rejected() {

    // Close window
    done(0);
}

void BuyerTorrentPluginConfigurationDialog::on_maxPriceLineEdit_textChanged(const QString &arg1) {

    bool ok;
    int maxPrice = arg1.toInt(&ok);

    if(ok) {

        // Calculate new total price
        QString totalPrice = QString::number(_torrentInfo.num_pieces() * maxPrice);

        // Update total price label
        ui->totalPriceValueLabel->setText(totalPrice);
    }

}
