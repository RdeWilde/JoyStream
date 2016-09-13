/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/SellerTermsDialog.hpp>
#include "ui_SellerTermsDialog.h"
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>
#include <protocol_wire/protocol_wire.hpp>

#include <QMessageBox>

namespace joystream {
namespace classic {
namespace gui {

const int SellerTermsDialog::REJECTED_EXIT_CODE = 1;
const int SellerTermsDialog::SUCCESS_EXIT_CODE = 2;

SellerTermsDialog::SellerTermsDialog(const BitcoinDisplaySettings * settings)
    :ui(new Ui::SellerTermsDialog)
    , _displayMode(DisplayMode::simple)
    , _minContractFeePerKbPrefixSettings(BitcoinRepresentation::BitCoinPrefix::Milli, BitcoinRepresentation::MetricPrefix::Micro, settings)
    , _minPiecePricePrefixSettings(BitcoinRepresentation::BitCoinPrefix::Satoshi, BitcoinRepresentation::MetricPrefix::Nano, settings)
    , _pricePerGBPrefixSettings(BitcoinRepresentation::BitCoinPrefix::Milli, BitcoinRepresentation::MetricPrefix::Centi, settings)
    , _settlementFeePrefixSettings(BitcoinRepresentation::BitCoinPrefix::Milli, BitcoinRepresentation::MetricPrefix::Centi, settings)
    , _maxRevenuePrefixSettings(BitcoinRepresentation::BitCoinPrefix::Milli, BitcoinRepresentation::MetricPrefix::Centi, settings) {

    ui->setupUi(this);

    setDisplayMode(_displayMode);
    updateTitleLabels();
}

SellerTermsDialog::~SellerTermsDialog() {
    delete ui;
}

quint64 SellerTermsDialog::minPrice() const {

    bool ok;
    int minPrice = ui->minPiecePriceLineEdit->text().toInt(&ok);

    if(!ok)
        throw std::runtime_error("Invalid minimum price.");
    else if(minPrice <= 0)
        throw std::runtime_error("Minimum price must be no less than 0.");
    else
        return minPrice;
}

void SellerTermsDialog::setMinPrice(quint64 minPrice) const {
    ui->minPiecePriceLineEdit->setText(QString::number(minPrice));
}

quint32 SellerTermsDialog::minLock() const {

    bool ok;
    double minLockHours = ui->minLockTimeLineEdit->text().toDouble(&ok);
    int minLock = static_cast<int>(minLockHours * 3600);

    if(!ok)
        throw std::runtime_error("Invalid minimum lock.");
    else if(minLock <= 0)
        throw std::runtime_error("Minimum price must be greater than 0.");
    else
        return minLock;
}

void SellerTermsDialog::setMinLock(quint32 minLock) const {

    double numberOfHours = (double)minLock / 3600;

    ui->minLockTimeLineEdit->setText(QString::number(numberOfHours));
}

quint32 SellerTermsDialog::maxNumberOfSellers() const {

    bool ok;
    int maxNumberOfSellers = ui->maxNumberOfSellersLineEdit->text().toInt(&ok);

    if(!ok)
        throw std::runtime_error("Invalid maximum number of sellers.");
    else if(maxNumberOfSellers <= 0)
        throw std::runtime_error("Maximum number of sellers must be greater than 0.");
    else
        return maxNumberOfSellers;
}

void SellerTermsDialog::setMaxNumberOfSellers(quint32 maxNumberOfSellers) const {
    ui->maxNumberOfSellersLineEdit->setText(QString::number(maxNumberOfSellers));
}

quint64 SellerTermsDialog::minContractFeePerKb() const {

    bool ok;
    double minContractFeePerKb = ui->minContractFeePrKbLineEdit->text().toDouble(&ok);

    if(!ok)
        throw std::runtime_error("Minimum contract fee/Kb was malformed.");
    else if(minContractFeePerKb < 0)
        throw std::runtime_error("Minimum contract fee/Kb must be non-negative.");
    else
        return _minContractFeePerKbPrefixSettings.toSatoshies(minContractFeePerKb);
}

void SellerTermsDialog::setMinContractFeePerKb(quint64 minContractFeePerKb) const {

    QString str = _minContractFeePerKbPrefixSettings.toString(minContractFeePerKb);

    ui->minContractFeePrKbLineEdit->setText(str);
}

quint64 SellerTermsDialog::settlementFee() const {

    bool ok;
    double minContractFeePerKb = ui->minContractFeePrKbLineEdit->text().toDouble(&ok);

    if(!ok)
        throw std::runtime_error("Minimum contract fee/Kb was malformed.");
    else if(minContractFeePerKb < 0)
        throw std::runtime_error("Minimum contract fee/Kb must be non-negative.");
    else
        return _minContractFeePerKbPrefixSettings.toSatoshies(minContractFeePerKb);
}

void SellerTermsDialog::setSettlementFee(quint64 settlementFee) const {

    QString str = _settlementFeePrefixSettings.toString(settlementFee);

    ui->settlementFeeLineEdit->setText(str);
}

protocol_wire::SellerTerms SellerTermsDialog::terms() const {
    return protocol_wire::SellerTerms(minPrice(), minLock(), maxNumberOfSellers(), minContractFeePerKb(), 0);
}

boost::optional<SellerTermsDialog::TorrentInfo> SellerTermsDialog::torrentInfo() const noexcept {
    return _torrentInfo;
}
void SellerTermsDialog::setTorrentInfo(const boost::optional<TorrentInfo> & torrentInfo) {

    // set/unset torrent information
    _torrentInfo = torrentInfo;

    // Update value labels
    updateValueLabels();
}

SellerTermsDialog::DisplayMode SellerTermsDialog::displayMode() const noexcept {
    return _displayMode;
}

void SellerTermsDialog::setDisplayMode(const DisplayMode mode) {

    if(mode == DisplayMode::simple) {

        _displayMode = DisplayMode::simple;
        ui->displayModeButton->setText("Simple");

        ui->minLockTimeTitleLabel->hide();
        ui->minLockTimeLineEdit->hide();
        ui->maxNumberOfSellersTitleLabel->hide();
        ui->maxNumberOfSellersLineEdit->hide();
        ui->minContractFeePrKbTitleLabel->hide();
        ui->minContractFeePrKbLineEdit->hide();
        ui->settlementFeeTitleLabel->hide();
        ui->settlementFeeLineEdit->hide();

    } else {

        _displayMode = DisplayMode::advanced;
        ui->displayModeButton->setText("Advanced");

        ui->minLockTimeTitleLabel->show();
        ui->minLockTimeLineEdit->show();
        ui->maxNumberOfSellersTitleLabel->show();
        ui->maxNumberOfSellersLineEdit->show();
        ui->minContractFeePrKbTitleLabel->show();
        ui->minContractFeePrKbLineEdit->show();
        ui->settlementFeeTitleLabel->show();
        ui->settlementFeeLineEdit->show();
    }

}

void SellerTermsDialog::on_displayModeButton_clicked() {

    switch(_displayMode) {
        case DisplayMode::advanced: setDisplayMode(DisplayMode::simple); break;
        case DisplayMode::simple: setDisplayMode(DisplayMode::advanced); break;
        default:
            assert(false);
    }
}

void SellerTermsDialog::on_cancelPushButton_clicked() {

    // Close window with failure code
    done(REJECTED_EXIT_CODE);
}

void SellerTermsDialog::on_okPushButton_clicked() {

    try {

        // Try to decode all fields
        terms();

        // No exception was thrown, so close window
        // with success code
        done(SUCCESS_EXIT_CODE);

    } catch(const std::runtime_error & e) {
        QMessageBox::critical(this, "", QString::fromStdString(e.what()));
    }
}

void SellerTermsDialog::updateValueLabels() {

    quint64 minimumPrice;

    // Try to recover minimum price if possible
    try {
        minimumPrice = minPrice();
    } catch(std::runtime_error &) {

        ui->maxRevenuePrUploadValueLabel->setText("");
        ui->pricePrGBValueLabel->setText("");
        return;
    }

    // If torrent information is not present, then we stop
    if(!_torrentInfo.is_initialized()) {

        ui->maxRevenuePrUploadValueLabel->setText("");
        ui->pricePrGBValueLabel->setText("");
        return;
    }

    // Set Maximum revenue per upload
    {
        quint64 value = maxRevenuePrUpload(minimumPrice, _torrentInfo.get()._numberOfPieces);

        QString text = _maxRevenuePrefixSettings.toString(value);

        ui->maxRevenuePrUploadValueLabel->setText(text);
    }

    // Set Price pr. GB
    {
        quint64 value = pricePrGB(minimumPrice, _torrentInfo.get()._bytesPerPiece);

        QString text = _pricePerGBPrefixSettings.toString(value);

        ui->pricePrGBValueLabel->setText(text);
    }

}

void SellerTermsDialog::updateTitleLabels() {

    ui->minPiecePriceTitleLabel->setText("Minium price per piece (" + _minPiecePricePrefixSettings.prefix() + ")");
    ui->pricePrGBTitleLabel->setText("Price/GB (" + _pricePerGBPrefixSettings.prefix() + ")");
    ui->minContractFeePrKbTitleLabel->setText("Maximum contract fee (" + _minContractFeePerKbPrefixSettings.prefix() + "/Kb)");
    ui->settlementFeeTitleLabel->setText("Settlement (" + _settlementFeePrefixSettings.prefix() + "/Kb)");
    ui->maxRevenuePrUploadTitleLabel->setText("Max revenue/upload (" + _maxRevenuePrefixSettings.prefix() + ")");
}


quint64 SellerTermsDialog::maxRevenuePrUpload(quint64 price, unsigned int numberOfPieces) {
    return price*numberOfPieces;
}

quint64 SellerTermsDialog::pricePrGB(quint64 price, unsigned int bytesPerPiece) {

    float pricePrByte = (float)price/bytesPerPiece;

    float pricePrGB = pricePrByte*(1000*1000*1000);

    if(pricePrGB < 0)
        return 0;
    else
        return static_cast<quint64>(ceil(pricePrGB));
}

}
}
}
