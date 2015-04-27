#include "BuyerTorrentPluginConfigurationDialog.hpp"
#include "ui_BuyerTorrentPluginConfigurationDialog.h"
#include "extension/PluginMode.hpp"

//#include "extension/TorrentPluginConfiguration.hpp"
#include "extension/BuyerTorrentPlugin.hpp" // BuyerTorrentPlugin::Configuration, BuyerTorrentPlugin::contractFee

#include "controller/Controller.hpp"
#include "extension/BitCoin/Wallet.hpp"

#include "extension/BitCoin/BitSwaprjs.hpp"
#include "extension/BitCoin/UnspentP2PKHOutput.hpp"

#include "extension/BitCoin/BitCoin.hpp"

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
    updateTotal();
}

BuyerTorrentPluginConfigurationDialog::~BuyerTorrentPluginConfigurationDialog() {
    delete ui;
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // Number of sellers
    bool okNumberOfSellers;
    int numberOfSellers = ui->numPeersLineEdit->text().toInt(&okNumberOfSellers);

    if(!okNumberOfSellers || numberOfSellers < 0)
        return;

    // Max fee per kB (satoshi)
    bool okFeePerkB;
    int feePerkB = static_cast<int>(SATOSHIES_PER_M_BTC * ui->feePrKbLineEdit->text().toDouble(&okFeePerkB));

    if(!okFeePerkB || feePerkB < 0)
        return;

    // The total amount the buyer at most wants to spend (satoshi)
    bool okMaxTotalSpend;
    int maxTotalSpend = static_cast<int>(SATOSHIES_PER_M_BTC * ui->maxTotalSpendLineEdit->text().toDouble(&okMaxTotalSpend));

    if(!okMaxTotalSpend || maxTotalSpend < 0)
        return;

    // Fee for contract based on fee estimate
    quint64 totalFee = BuyerTorrentPlugin::contractFee(numberOfSellers, feePerkB);

    // Corresponding maximum piece price (satoshi)
    quint64 maxPrice = maxPriceFromTotalSpend(maxTotalSpend, totalFee);

    // Amount needed to fund contract (satoshies)
    // We put enough in each channel to pay for full torrent from each seller,
    // but no more than maxTotalSpend
    quint64 minFunds = minimalFunds(maxPrice, numberOfSellers, totalFee);

    // Get funding output - this has to be grabbed from wallet/chain later
    UnspentP2PKHOutput utxo = _wallet->getUtxo(minFunds, 1);

    // Check that an utxo was indeed found
    if(utxo.value() == 0) {

        // Show modal dialog on same thread, we block untill it is closed
        QMessageBox msgBox;
        msgBox.setText(QString("No utxo found with value no less than: ") + QString::number(minFunds));
        msgBox.exec();

        return;
    }

    // Maximum Lock time
    QTime maxLockTime = ui->maxLockTimeEdit->time();
    quint32 maxLock = maxLockTime.hour()*3600 + maxLockTime.minute()*60 + maxLockTime.second();

    // Create configuration
    BuyerTorrentPlugin::Configuration configuration(false,
                                                    maxPrice,
                                                    maxLock,
                                                    feePerkB,
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

quint64 BuyerTorrentPluginConfigurationDialog::maxPriceFromTotalSpend(quint64 maxTotalSpend, quint64 totalFee) {
    return qFloor(((double)maxTotalSpend - totalFee)/_torrentInfo.num_pieces());
}

quint64 BuyerTorrentPluginConfigurationDialog::minimalFunds(quint64 maxPrice, qint32 numberOfSellers, quint64 totalFee) {
    return maxPrice*_torrentInfo.num_pieces()*numberOfSellers + totalFee;
}

void BuyerTorrentPluginConfigurationDialog::updateTotal() {

    // Number of sellers
    bool okNumberOfSellers;
    int numberOfSellers = ui->numPeersLineEdit->text().toInt(&okNumberOfSellers);

    if(!okNumberOfSellers || numberOfSellers < 0)
        return;

    // Max fee per kB (satoshi)
    bool okFeePerkB;
    int feePerkB = static_cast<int>(SATOSHIES_PER_M_BTC * ui->feePrKbLineEdit->text().toDouble(&okFeePerkB));

    if(!okFeePerkB || feePerkB < 0)
        return;

    // The total amount the buyer at most wants to spend (satoshi)
    bool okMaxTotalSpend;
    int maxTotalSpend = static_cast<int>(SATOSHIES_PER_M_BTC * ui->maxTotalSpendLineEdit->text().toDouble(&okMaxTotalSpend));

    if(!okMaxTotalSpend || maxTotalSpend < 0)
        return;

    // Fee for contract based on fee estimate
    quint64 totalFee = BuyerTorrentPlugin::contractFee(numberOfSellers, feePerkB);

    // Corresponding maximum piece price (satoshi)
    quint64 maxPrice = maxPriceFromTotalSpend(maxTotalSpend, totalFee);

    // Amount needed to fund contract (satoshies)
    // We put enough in each channel to pay for full torrent from each seller,
    // but no more than maxTotalSpend
    quint64 minFunds = minimalFunds(maxPrice, numberOfSellers, totalFee);

    // Update total price label
    ui->totalValueLabel->setText(QString::number(minFunds/SATOSHIES_PER_M_BTC) + "mɃ");
}

void BuyerTorrentPluginConfigurationDialog::on_maxTotalSpendLineEdit_textChanged(const QString &arg1) {
    updateTotal();
}

void BuyerTorrentPluginConfigurationDialog::on_feePrKbLineEdit_textChanged(const QString &arg1) {
    updateTotal();
}

void BuyerTorrentPluginConfigurationDialog::on_numPeersLineEdit_textEdited(const QString &arg1) {
    updateTotal();
}
