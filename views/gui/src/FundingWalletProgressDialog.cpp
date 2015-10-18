/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 17 2015
 */

#include <gui/FundingWalletProgressDialog.hpp>
#include <common/BitcoinRepresentation.hpp>

#include "ui_FundingWalletProgressDialog.h"

FundingWalletProgressDialog::FundingWalletProgressDialog(quint64 fundingAmount, const BitcoinDisplaySettings * bitcoinDisplaySettings, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FundingWalletProgressDialog)
    , _fundingAmount(fundingAmount)
    , _settings(bitcoinDisplaySettings){

    ui->setupUi(this);

    // Make into animated bar
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);

    // Set funding amount display
    ui->fundingAmountLabel->setText(fundingAmountToString());
}

QString FundingWalletProgressDialog::fundingAmountToString() {
    return BitcoinRepresentation(_fundingAmount).toString(_settings);
}

FundingWalletProgressDialog::~FundingWalletProgressDialog()
{
    delete ui;
}
