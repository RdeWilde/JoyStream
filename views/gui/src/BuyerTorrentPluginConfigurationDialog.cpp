/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/BuyerTorrentPluginConfigurationDialog.hpp>
#include "ui_BuyerTorrentPluginConfigurationDialog.h"
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/Bitcoin.hpp>
#include <core/extension/PluginMode.hpp>
#include <core/extension/BuyerTorrentPlugin.hpp> // BuyerTorrentPlugin::Configuration, BuyerTorrentPlugin::contractFee
#include <core/controller/Controller.hpp>
#include <wallet/Manager.hpp>

#include <QtMath>
#include <QMessageBox>
#include <QVector>

BuyerTorrentPluginConfigurationDialog::BuyerTorrentPluginConfigurationDialog(Controller * controller, Wallet::Manager * wallet, const libtorrent::torrent_info & torrentInfo, const BitcoinDisplaySettings * settings)
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
    if(_settings->currency() == BitcoinDisplaySettings::Currency::BitCoin)
        ui->maxTotalSpendLabel->setText("Maximum total spend (mɃ):");
    else  // == BitCoinDisplaySettings::Currency::Fiat
        ui->maxTotalSpendLabel->setText("Maximum total spend (¢):");

    // Set total price
    updateTotal();
}

BuyerTorrentPluginConfigurationDialog::~BuyerTorrentPluginConfigurationDialog() {
    delete ui;
}

bool BuyerTorrentPluginConfigurationDialog::tryToGetNumberOfSellers(int & numberOfSellers) const {

    bool ok;
    int x = ui->numPeersLineEdit->text().toInt(&ok);

    // Check if it worked
    if(!ok || x < 0)
        return false;
    else {

        // If it did, then save value to reference, and return success
        numberOfSellers = x;
        return true;
    }
}

bool BuyerTorrentPluginConfigurationDialog::tryToGetFeePerkB(int & feePerkB) const {

    bool ok;
    double x = ui->feePrKbLineEdit->text().toDouble(&ok);

    if(!ok || x < 0)
        return false;
    else {
        feePerkB = static_cast<int>(SATOSHIES_PER_M_BTC * x);
        return true;
    }
}

bool BuyerTorrentPluginConfigurationDialog::tryToGetMaxTotalSpend(quint64 & maxTotalSpend) const {

    bool ok;
    double x = ui->maxTotalSpendLineEdit->text().toDouble(&ok);

    if(!ok || x < 0)
        return false;
    else {

        if(_settings->currency() == BitcoinDisplaySettings::Currency::BitCoin)
            maxTotalSpend = BitcoinRepresentation(BitcoinRepresentation::BitCoinPrefix::Milli, x).satoshies();
        else
            maxTotalSpend = BitcoinRepresentation(BitcoinRepresentation::MetricPrefix::Centi, x, _settings->rate()).satoshies();

        return true;
    }
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // Number of sellers
    int numberOfSellers;

    if(!tryToGetNumberOfSellers(numberOfSellers)) {

        QMessageBox::critical(this,
                              "Invalid #sellers",
                              "Must be positive number: " + ui->numPeersLineEdit->text());
        return;
    }

    // Max fee per kB (satoshi)
    int feePerkB;

    if(!tryToGetFeePerkB(feePerkB)) {

        QMessageBox::critical(this,
                              "Invalid fee per kb",
                              "Must be positive number: " + ui->feePrKbLineEdit->text());
        return;
    }

    // ============================================================
    // The total amount the buyer at most wants to spend (satoshies)
    quint64 maxTotalSpend;

    if(!tryToGetMaxTotalSpend(maxTotalSpend)) {

        QMessageBox::critical(this,
                              "Invalid max total spend",
                              "Must be positive number: " + ui->maxTotalSpendLineEdit->text());
        return;
    }

    // Maximum piece price (satoshies)
    quint64 maxPrice = maxPriceFromTotalSpend(maxTotalSpend, numberOfSellers, feePerkB);

    // ============================================================
    // Amount needed to fund contract (satoshies)
    quint64 minFunds = Payor::minimalFunds(_torrentInfo.num_pieces(),
                                           maxPrice,
                                           numberOfSellers,
                                           feePerkB); //

    // Get funding output - this has to be grabbed from wallet/chain later
    Coin::UnspentP2PKHOutput utxo = _wallet->BLOCKCYPHER_lock_one_utxo(minFunds);

    // = _wallet->getUtxo(minFunds, 1);
    //qDebug() << "Skipping grabbing real utxo, just empty crap for now";

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

    // Number of sellers
    int numberOfSellers;

    if(!tryToGetNumberOfSellers(numberOfSellers))
        return;

    // Max fee per kB (satoshi)
    int feePerkB;
    if(!tryToGetFeePerkB(feePerkB))
        return;

    // The total amount the buyer at most wants to spend (satoshi)
    quint64 maxTotalSpend;

    if(!tryToGetMaxTotalSpend(maxTotalSpend))
        return;

    // Corresponding maximum piece price (satoshi)
    quint64 maxPrice = maxPriceFromTotalSpend(maxTotalSpend, numberOfSellers, feePerkB);

    // Amount needed to fund contract (satoshies)
    quint64 minFunds = Payor::minimalFunds(_torrentInfo.num_pieces(),
                                           maxPrice,
                                           numberOfSellers,
                                           feePerkB);

    // Update total price label
    QString minFundsString = BitcoinRepresentation(minFunds).toString(_settings);
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
