/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/SellerTorrentPluginConfigurationDialog.hpp>
#include "ui_SellerTorrentPluginConfigurationDialog.h"
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>
#include <common/Bitcoin.hpp>
#include <core/controller/Controller.hpp>
#include <core/extension/SellerTorrentPlugin.hpp>


#include <QMessageBox>

SellerTorrentPluginConfigurationDialog::SellerTorrentPluginConfigurationDialog(Controller * controller, Wallet * wallet, const libtorrent::torrent_info & torrentInfo, const BitcoinDisplaySettings * settings)
    :ui(new Ui::SellerTorrentPluginConfigurationDialog)
    , _controller(controller)
    , _wallet(wallet)
    , _torrentInfo(torrentInfo)
    , _settings(settings) {

    ui->setupUi(this);

    // Hide some fields
    ui->minFeeLabel->setVisible(false);
    ui->minFeeLineEdit->setVisible(false);

    // Set label based on bitconi display settings
    if(_settings->currency() == BitcoinDisplaySettings::Currency::BitCoin) {
        ui->minPriceLabel->setText("Price (mɃ/GB):");
        //ui->minPriceLineEdit->setText("3");
    } else { // == BitCoinDisplaySettings::Currency::Fiat
        ui->minPriceLabel->setText("Price (¢/GB):");
        //ui->minPriceLineEdit
    }

    // Set inital total to begin with
    updateComputedSatoshiesMinPriceValue();
}

SellerTorrentPluginConfigurationDialog::~SellerTorrentPluginConfigurationDialog() {
    delete ui;
}

bool SellerTorrentPluginConfigurationDialog::tryToGetPricePrGB(quint64 & minSatoshiesPrGB) const {

    bool ok;
    double x = ui->minPriceLineEdit->text().toDouble(&ok);

    if(!ok || x < 0)
        return false;
    else {

        // Convert to satoshies
        if(_settings->currency() == BitcoinDisplaySettings::Currency::BitCoin)
            minSatoshiesPrGB = BitcoinRepresentation(BitcoinRepresentation::BitCoinPrefix::Milli, x).satoshies();
        else
            minSatoshiesPrGB = BitcoinRepresentation(BitcoinRepresentation::MetricPrefix::Centi, x, _settings->rate()).satoshies();
    }
}

quint32 SellerTorrentPluginConfigurationDialog::minPriceFromPricePrGB(quint64 pricePrGB) const {

    // Number of pieces pr gigabyte of given torrent
    float numberOfPiecesPrGB = ((float)1000*1000*1000) /_torrentInfo.piece_length();

    // Piece price (satoshies)
    return static_cast<quint32>(floor(pricePrGB/numberOfPiecesPrGB));

}

void SellerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // ============================================================
    // minPrice (satoshies)

    quint64 pricePrGB;
    if(!tryToGetPricePrGB(pricePrGB)) {

        QMessageBox::critical(this,
                              "Invalid per GB price value",
                              "Must be positive number: " + ui->minPriceLineEdit->text());
        return;
    }

    quint64 minPrice = minPriceFromPricePrGB(pricePrGB);

    // Warn user if selling for free
    if(minPrice == 0) {

        QMessageBox::StandardButton buttonPressed = QMessageBox::question(this,
                                                                          "Are you sure",
                                                                          "You are selling for free, do you wish to proceed?",
                                                                          QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

        // Quit if user chose no
        if(buttonPressed == QMessageBox::StandardButton::No)
            return;
    } else if(minPrice * _torrentInfo.num_pieces() <= 546) {

        QMessageBox::critical(this,
                              "Dust limit",
                              "Pick a price no less than : " + QString::number((float)546/_torrentInfo.num_pieces()) + " (satoshies)");
        return;

    }


    // ============================================================
    // minLockTime
    QTime minLockTime = ui->minLockTimeEdit->time();
    quint32 minLock = minLockTime.hour()*3600 + minLockTime.minute()*60 + minLockTime.second();

    // minFeePerKByte
    bool okMinFeePerKByte;
    quint32 minFeePerKByte = static_cast<int>(SATOSHIES_PER_M_BTC * ui->minFeeLineEdit->text().toDouble(&okMinFeePerKByte));

    if(!okMinFeePerKByte || minFeePerKByte < 0) {

        QMessageBox::critical(this,
                              "Invalid fee per kB",
                              "Must be positive number: " + ui->minFeeLineEdit->text());
        return;
    }

    // maxNumberOfSellers
    bool okMaxNumberOfSellers;
    quint32 maxNumberOfSellers = ui->maxNumberOfSellersLineEdit->text().toInt(&okMaxNumberOfSellers);

    if(!okMaxNumberOfSellers || maxNumberOfSellers < 1) {


        QMessageBox::critical(this,
                              "Invalid max #sellers",
                              "Must be positive number: " + ui->maxNumberOfSellersLineEdit->text());
        return;
    }

    // maxContractConfirmationDelay
    QTime maxContractConfirmationDelayTime = ui->maxConfirmationTimeTimeEdit->time();
    quint32 maxContractConfirmationDelay = maxContractConfirmationDelayTime.hour()*3600 + maxContractConfirmationDelayTime.minute()*60 + maxContractConfirmationDelayTime.second();;

    // Tell controller to start plugin
    _controller->startSellerTorrentPlugin(_torrentInfo.info_hash(), SellerTorrentPlugin::Configuration(false,
                                                                                                        minPrice,
                                                                                                        minLock,
                                                                                                        minFeePerKByte,
                                                                                                        maxNumberOfSellers,
                                                                                                        maxContractConfirmationDelay));

    // close window
    done(0);
}

void SellerTorrentPluginConfigurationDialog::updateComputedSatoshiesMinPriceValue() {

    quint64 pricePrGB;

    if(!tryToGetPricePrGB(pricePrGB))
        return;


    quint64 minPrice = minPriceFromPricePrGB(pricePrGB);

    ui->computedSatoshiesMinPriceValue->setText(QString::number(minPrice));
}

void SellerTorrentPluginConfigurationDialog::on_minPriceLineEdit_textChanged(const QString &arg1) {
    updateComputedSatoshiesMinPriceValue();
}
