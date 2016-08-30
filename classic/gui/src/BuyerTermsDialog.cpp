/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/BuyerTermsDialog.hpp>
#include "ui_BuyerTermsDialog.h"
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>
#include <protocol_session/protocol_session.hpp>

#include <libtorrent/socket.hpp>

#include <QMessageBox>

namespace joystream {
namespace classic {
namespace gui {

const int BuyerTermsDialog::REJECTED_EXIT_CODE = 1;
const int BuyerTermsDialog::SUCCESS_EXIT_CODE = 2;

BuyerTermsDialog::BuyerTermsDialog(const BitcoinDisplaySettings * settings)
    : ui(new Ui::BuyerTermsDialog)
    , _displayMode(DisplayMode::simple)
    , _maxContractFeePerKbPrefixSettings(BitcoinRepresentation::BitCoinPrefix::Milli, BitcoinRepresentation::MetricPrefix::Micro, settings)
    , _maxPiecePricePrefixSettings(BitcoinRepresentation::BitCoinPrefix::Satoshi, BitcoinRepresentation::MetricPrefix::Nano, settings)
    , _totalFundsPrefixSettings(BitcoinRepresentation::BitCoinPrefix::Milli, BitcoinRepresentation::MetricPrefix::Centi, settings) {

    ui->setupUi(this);

    setDisplayMode(_displayMode);
    updateCurrencyFields();
}

BuyerTermsDialog::~BuyerTermsDialog() {
    delete ui;
}

quint64 BuyerTermsDialog::maxPrice() const {

    bool ok;
    int maxPrice = ui->maxPiecePriceLineEdit->text().toInt(&ok);

    if(!ok)
        throw std::runtime_error("Invalid maximum price.");
    else if(maxPrice <= 0)
        throw std::runtime_error("Maximum price must be greater than 0.");
    else
        return maxPrice;
}

void BuyerTermsDialog::setMaxPrice(quint64 maxPrice) const {
    ui->maxPiecePriceLineEdit->setText(QString::number(maxPrice));
}

quint32 BuyerTermsDialog::maxLock() const {

    bool ok;
    double maxLockHousrs = ui->maxLockTimeLineEdit->text().toDouble(&ok);
    int maxLock = static_cast<int>(maxLockHousrs * 3600);

    if(!ok)
        throw std::runtime_error("Invalid maximum lock.");
    else if(maxLock <= 0)
        throw std::runtime_error("Maximum price must be greater than 0.");
    else
        return maxLock;
}

void BuyerTermsDialog::setMaxLock(quint32 maxLock) const {

    double numberOfHours = (double)maxLock / 3600;

    ui->maxLockTimeLineEdit->setText(QString::number(numberOfHours));
}

quint32 BuyerTermsDialog::minNumberOfSellers() const {

    bool ok;
    double minNumberOfSellers = ui->minNumberOfSellersLineEdit->text().toInt(&ok);

    if(!ok)
        throw std::runtime_error("Invalid minimum number of sellers.");
    else if(minNumberOfSellers <= 0)
        throw std::runtime_error("Minimum number of sellers must be greater than 0.");
    else
        return minNumberOfSellers;
}

void BuyerTermsDialog::setMinNumberOfSellers(quint32 minNumberOfSellers) const {
    ui->minNumberOfSellersLineEdit->setText(QString::number(minNumberOfSellers));
}

quint64 BuyerTermsDialog::maxContractFeePerKb() const {

    bool ok;
    double maxContractFeePerKb = ui->maxContractFeePrKbLineEdit->text().toDouble(&ok);

    if(!ok)
        throw std::runtime_error("Maximum contract fee/Kb was malformed.");
    else if(maxContractFeePerKb < 0)
        throw std::runtime_error("Maximum contract fee/Kb must be non-negative.");
    else
        return _maxContractFeePerKbPrefixSettings.toSatoshies(maxContractFeePerKb);
}

void BuyerTermsDialog::setMaxContractFeePerKb(quint64 maxContractFeePerKb) const {

    QString str = _maxContractFeePerKbPrefixSettings.toString(maxContractFeePerKb);

    ui->maxContractFeePrKbLineEdit->setText(str);
}

protocol_wire::BuyerTerms BuyerTermsDialog::terms() const {
    return protocol_wire::BuyerTerms(maxPrice(), maxLock(), minNumberOfSellers(), maxContractFeePerKb(), 0);
}

boost::optional<unsigned int> BuyerTermsDialog::numberOfPieces() const noexcept {
    return _numberOfPieces;
}

void BuyerTermsDialog::setNumberOfPieces(const boost::optional<unsigned int> & numberOfPieces) {

    _numberOfPieces = numberOfPieces;

    if(_numberOfPieces.is_initialized())
        updateTotal();
}

void BuyerTermsDialog::setDisplayMode(const DisplayMode mode) {

    if(mode == DisplayMode::simple) {

        _displayMode = DisplayMode::simple;

        ui->displayModeButton->setText("Simple");
        ui->maxLockTimeLabel->hide();
        ui->maxLockTimeLineEdit->hide();
        ui->minNumberOfSellersLabel->hide();
        ui->minNumberOfSellersLineEdit->hide();
        ui->maxContractFeePrKbLabel->hide();
        ui->maxContractFeePrKbLineEdit->hide();

    } else {

        _displayMode = DisplayMode::advanced;

        ui->displayModeButton->setText("Advanced");
        ui->maxLockTimeLabel->show();
        ui->maxLockTimeLineEdit->show();
        ui->minNumberOfSellersLabel->show();
        ui->minNumberOfSellersLineEdit->show();
        ui->maxContractFeePrKbLabel->show();
        ui->maxContractFeePrKbLineEdit->show();
    }

}

void BuyerTermsDialog::updateTotal() {

    if(_numberOfPieces.is_initialized()) {

        protocol_wire::BuyerTerms t;

        try {

            // Try to decode all fields
            t = terms();

        } catch(const std::runtime_error & e) {

            // Jump ship, can't do it
            ui->totalValueLabel->setText(e.what());

            return;
        }

        // Amount needed to fund contract (satoshies)
        quint64 minFunds = protocol_session::Session<libtorrent::tcp::endpoint>::minimumFundsRequiredAsBuyer(t, _numberOfPieces.get());

        // Update total price label
        QString minFundsString = _totalFundsPrefixSettings.toString(minFunds);

        ui->totalValueLabel->setText(minFundsString);
    }
}

void BuyerTermsDialog::on_displayModeButton_clicked() {

    switch(_displayMode) {
        case DisplayMode::advanced: setDisplayMode(DisplayMode::simple); break;
        case DisplayMode::simple: setDisplayMode(DisplayMode::advanced); break;
        default:
            assert(false);
    }
}

void BuyerTermsDialog::on_cancelPushButton_clicked() {

    // Close window with failure code
    done(REJECTED_EXIT_CODE);
}

void BuyerTermsDialog::on_okPushButton_clicked() {

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

void BuyerTermsDialog::updateCurrencyFields() {

    ui->maxPiecePriceLabelTitle->setText("Maximum price per piece (" + _maxPiecePricePrefixSettings.prefix() + ")");
    ui->maxContractFeePrKbLabel->setText("Maximum contract fee (" + _maxContractFeePerKbPrefixSettings.prefix() + "/Kb)");
    ui->totalLabel->setText("Total funds required (" + _totalFundsPrefixSettings.prefix() + ")");
}

}
}
}
