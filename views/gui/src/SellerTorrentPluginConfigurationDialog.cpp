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

#define SLIDE_MIN 1
#define SLIDER_MAX 200
#define SLIDER_TICK 1

SellerTorrentPluginConfigurationDialog::SellerTorrentPluginConfigurationDialog(Controller * controller, joystream::bitcoin::SPVWallet * wallet, const libtorrent::torrent_info & torrentInfo, const BitcoinDisplaySettings * settings)
    :ui(new Ui::SellerTorrentPluginConfigurationDialog)
    , _controller(controller)
    , _wallet(wallet)
    , _torrentInfo(torrentInfo)
    , _settings(settings) {

    ui->setupUi(this);

    // Hide some fields
    ui->minFeeLabel->setVisible(false);
    ui->minFeeLineEdit->setVisible(false);

    /**
    // Set label based on bitconi display settings
    if(_settings->currency() == BitcoinDisplaySettings::Currency::BitCoin) {
        ui->minPriceLabel->setText("Price for full torrent (mɃ/GB):");
        //ui->minPriceLineEdit->setText("3");
    } else { // == BitCoinDisplaySettings::Currency::Fiat
        ui->minPriceLabel->setText("Price for full torrent (¢/GB):");
        //ui->minPriceLineEdit
    }
    */

    // Setup slider
    ui->minPriceSlider->setRange(SLIDE_MIN, SLIDER_MAX);
    ui->minPriceSlider->setSingleStep(SLIDER_TICK);
    ui->minPriceSlider->setValue(SLIDE_MIN);

    // Set inital total to begin with
    updatePrice();
}

SellerTorrentPluginConfigurationDialog::~SellerTorrentPluginConfigurationDialog() {
    delete ui;
}

/**
bool SellerTorrentPluginConfigurationDialog::tryToMinPrice(quint64 & minPrice) const {

    bool ok;
    double x = ui->minPriceLineEdit->text().toDouble(&ok);

    if(!ok || x < 0)
        return false;
    else {

        // Convert to satoshies
        if(_settings->currency() == BitcoinDisplaySettings::Currency::BitCoin)
            minPrice = BitcoinRepresentation(BitcoinRepresentation::BitCoinPrefix::Milli, x).satoshies();
        else
            minPrice = BitcoinRepresentation(BitcoinRepresentation::MetricPrefix::Centi, x, _settings->rate()).satoshies();

        return true;
    }
}
*/
/**
quint32 SellerTorrentPluginConfigurationDialog::minPriceFromPricePrGB(quint64 pricePrGB) const {

    // Number of pieces pr gigabyte of given torrent
    float numberOfPiecesPrGB = ((float)1000*1000*1000) /_torrentInfo.piece_length();

    // Piece price (satoshies)
    return static_cast<quint32>(floor(pricePrGB/numberOfPiecesPrGB));

}
*/

quint64 SellerTorrentPluginConfigurationDialog::pricePrGBFromTotalPrice(quint64 price) const {

    quint64 totalPrice = price*_torrentInfo.num_pieces();

    float pricePrByte = (float)totalPrice/_torrentInfo.total_size();

    float pricePrGB = pricePrByte*(1000*1000*1000);

    if(pricePrGB < 0)
        return 0;
    else
        return static_cast<quint64>(ceil(pricePrGB));
}

/**
quint64 SellerTorrentPluginConfigurationDialog::pricePrPieceFromTotalPrice(quint64 price) const {

    float pricePrPiece = (float)price/_torrentInfo.num_pieces();

    if(pricePrPiece < 0)
        return 0;
    else
        return static_cast<quint64>(ceil(pricePrPiece));
}
*/

void SellerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // ============================================================
    // minPrice (satoshies)

    /**
     * Minimum price based on GB
    quint64 pricePrGB;
    if(!tryToGetPricePrGB(pricePrGB)) {

        QMessageBox::critical(this,
                              "Invalid per GB price value",
                              "Must be positive number: " + ui->minPriceLineEdit->text());
        return;
    }

    quint64 minPrice = minPriceFromPricePrGB(pricePrGB);
    */

    /**
    // Total price for torrent
    quint64 totalPrice = ui->minPriceLineEdit->text().toLongLong();


    if(!tryToMinPrice(minPrice)) {

        QMessageBox::critical(this,
                              "Invalid per GB price value",
                              "Must be positive number: " + ui->minPriceLineEdit->text());
        return;
    }
    */

    // Direct piece price
    //quint64 minPrice = pricePrPieceFromTotalPrice(minPrice); //static_cast<quint64>(ceil((float)totalPrice / _torrentInfo.num_pieces()));

    quint64 minPrice = ui->minPriceSlider->value();

    // Warn user if selling for free
    if(minPrice == 0) {

        QMessageBox::StandardButton buttonPressed = QMessageBox::question(this,
                                                                          "Are you sure",
                                                                          "You are selling for free, do you wish to proceed?",
                                                                          QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

        // Quit if user chose no
        if(buttonPressed == QMessageBox::StandardButton::No)
            return;
    } else if(minPrice * _torrentInfo.num_pieces() <= BITCOIN_DUST_LIMIT) {

        QMessageBox::critical(this,
                              "Dust limit",
                              "Pick a price no less than : " + QString::number((float)BITCOIN_DUST_LIMIT/_torrentInfo.num_pieces()) + " (satoshies)");
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

/**
void SellerTorrentPluginConfigurationDialog::updateComputedSatoshiesMinPriceValue() {

    quint64 pricePrGB;

    if(!tryToGetPrice(pricePrGB))
        return;

    quint64 minPrice = minPriceFromPricePrGB(pricePrGB);

    ui->computedSatoshiesMinPriceValue->setText(QString::number(minPrice));
}
*/

void SellerTorrentPluginConfigurationDialog::updatePrice() {

    quint64 minPrice = ui->minPriceSlider->value();

    QString minPriceString = BitcoinRepresentation(minPrice).toString(_settings);
    ui->pricePrPiece->setText(minPriceString);

    quint64 pricePrGB = pricePrGBFromTotalPrice(minPrice);
    QString pricePrGBString = BitcoinRepresentation(pricePrGB).toString(_settings);
    ui->pricePerGBLabel->setText(pricePrGBString);
}

void SellerTorrentPluginConfigurationDialog::on_minPriceLineEdit_textChanged(const QString &arg1) {
    updatePrice();
}

void SellerTorrentPluginConfigurationDialog::on_minPriceSlider_sliderMoved(int position) {
    updatePrice();
}

void SellerTorrentPluginConfigurationDialog::on_minPriceSlider_sliderReleased() {
    updatePrice();
}
