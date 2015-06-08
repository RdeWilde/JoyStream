#include "BuyerTorrentPluginConfigurationDialog.hpp"
#include "ui_BuyerTorrentPluginConfigurationDialog.h"
#include "BitCoinDisplaySettings.hpp"
#include "BitCoinRepresentation.hpp"
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

BuyerTorrentPluginConfigurationDialog::BuyerTorrentPluginConfigurationDialog(Controller * controller, Wallet * wallet, const libtorrent::torrent_info & torrentInfo, const BitCoinDisplaySettings * settings)
    : ui(new Ui::BuyerTorrentPluginConfigurationDialog)
    , _controller(controller)
    , _wallet(wallet)
    , _torrentInfo(torrentInfo)
    , _settings(settings) {

    ui->setupUi(this);

    // Hide some fields
    ui->feeLabel->setVisible(false);
    ui->feePrKbLineEdit->setVisible(false);
    ui->waitConfirmationTimeLabel->setVisible(false);
    ui->maxConfirmationTimeTimeEdit->setVisible(false);

    // Set label based on bitconi display settings
    if(_settings->currency() == BitCoinDisplaySettings::Currency::BitCoin)
        ui->maxTotalSpendLabel->setText("Maximum total spend (mɃ):");
    else  // == BitCoinDisplaySettings::Currency::Fiat
        ui->maxTotalSpendLabel->setText("Maximum total spend (¢):");

    // Set total price
    updateTotal();
}

BuyerTorrentPluginConfigurationDialog::~BuyerTorrentPluginConfigurationDialog() {
    delete ui;
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // Message box used to alert user if invalid value is provided
    QMessageBox msgBox;

    // Number of sellers
    bool okNumberOfSellers;
    int numberOfSellers = ui->numPeersLineEdit->text().toInt(&okNumberOfSellers);

    if(!okNumberOfSellers || numberOfSellers < 0) {

        msgBox.setText("Invalid #sellers: " + ui->numPeersLineEdit->text());
        msgBox.exec();
        return;
    }

    // Max fee per kB (satoshi)
    bool okFeePerkB;
    int feePerkB = static_cast<int>(SATOSHIES_PER_M_BTC * ui->feePrKbLineEdit->text().toDouble(&okFeePerkB));

    if(!okFeePerkB || feePerkB < 0) {

        msgBox.setText("Invalid fee per kb: " + ui->feePrKbLineEdit->text());
        msgBox.exec();
        return;
    }

    // ============================================================
    // The total amount the buyer at most wants to spend (satoshies)

    // Parse value from line edit, check that it is valid
    bool okMaxTotalSpendLineEdit;
    double maxTotalSpendDouble = ui->maxTotalSpendLineEdit->text().toDouble(&okMaxTotalSpendLineEdit);

    if(!okMaxTotalSpendLineEdit || maxTotalSpendDouble < 0) {

        msgBox.setText("Invalid max total spend: " + ui->maxTotalSpendLineEdit->text());
        msgBox.exec();
        return;
    }

    // Maximum amount which can be spent on download (satoshies)
    quint64 maxTotalSpendInSatoshies;

    if(_settings->currency() == BitCoinDisplaySettings::Currency::BitCoin)
        maxTotalSpendInSatoshies = BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, maxTotalSpendDouble).satoshies();
    else
        maxTotalSpendInSatoshies = BitCoinRepresentation(BitCoinRepresentation::MetricPrefix::Centi, maxTotalSpendDouble, _settings->rate()).satoshies();

    // Maximum piece price (satoshies)
    quint64 maxPrice = maxPriceFromTotalSpend(maxTotalSpendInSatoshies, numberOfSellers, feePerkB);

    // ============================================================
    // Amount needed to fund contract (satoshies)
    quint64 minFunds = Payor::minimalFunds(_torrentInfo.num_pieces(), maxPrice, numberOfSellers, feePerkB);

    // Get funding output - this has to be grabbed from wallet/chain later
    UnspentP2PKHOutput utxo = _wallet->getUtxo(minFunds, 1);

    // Check that an utxo was indeed found
    if(utxo.value() == 0) {

        // Show modal dialog on same thread, we block untill it is closed
        QMessageBox msgBox;
        msgBox.setText(QString("No utxo found with value no less than: ") + QString::number(minFunds) + QString("(satoshies)"));
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

quint64 BuyerTorrentPluginConfigurationDialog::maxPriceFromTotalSpend(quint64 maxTotalSpend, int numberOfSellers, quint64 feePerkB) {

    quint64 totalFee = Payor::computeContractFee(numberOfSellers, feePerkB);

    return qFloor(((double)maxTotalSpend - totalFee)/_torrentInfo.num_pieces());
}

void BuyerTorrentPluginConfigurationDialog::updateTotal() {

    // Message box used to alert user if invalid value is provided
    QMessageBox msgBox;

    // Number of sellers
    bool okNumberOfSellers;
    int numberOfSellers = ui->numPeersLineEdit->text().toInt(&okNumberOfSellers);

    if(!okNumberOfSellers || numberOfSellers < 0) {

        msgBox.setText("Invalid #sellers: " + ui->numPeersLineEdit->text());
        msgBox.exec();
        return;
    }

    // Max fee per kB (satoshi)
    bool okFeePerkB;
    int feePerkB = static_cast<int>(SATOSHIES_PER_M_BTC * ui->feePrKbLineEdit->text().toDouble(&okFeePerkB));

    if(!okFeePerkB || feePerkB < 0) {

        msgBox.setText("Invalid fee per kb: " + ui->feePrKbLineEdit->text());
        msgBox.exec();
        return;
    }

    // The total amount the buyer at most wants to spend (satoshi)
    bool okMaxTotalSpend;
    int maxTotalSpend = static_cast<int>(SATOSHIES_PER_M_BTC * ui->maxTotalSpendLineEdit->text().toDouble(&okMaxTotalSpend));

    if(!okMaxTotalSpend || maxTotalSpend < 0) {

        msgBox.setText("Invalid fee per kb: " + ui->feePrKbLineEdit->text());
        msgBox.exec();
        return;
    }

    // Corresponding maximum piece price (satoshi)
    quint64 maxPrice = maxPriceFromTotalSpend(maxTotalSpend, numberOfSellers, feePerkB);

    // Amount needed to fund contract (satoshies)
    quint64 minFunds = Payor::minimalFunds(_torrentInfo.num_pieces(), maxPrice, numberOfSellers, feePerkB);

    // Update total price label
    //ui->totalValueLabel->setText(QString::number((float)minFunds/SATOSHIES_PER_M_BTC) + "mɃ");
    QString minFundsString = BitCoinRepresentation(minFunds).toString(_settings);
    ui->totalValueLabel->setText(minFundsString);
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
