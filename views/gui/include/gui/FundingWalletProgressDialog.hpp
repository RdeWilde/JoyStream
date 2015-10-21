/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 17 2015
 */

#ifndef FUNDING_WALLET_PROGRESS_DIALOG_HPP
#define FUNDING_WALLET_PROGRESS_DIALOG_HPP

#include <common/BitcoinDisplaySettings.hpp>

#include <QDialog>

namespace Ui {
class FundingWalletProgressDialog;
}

class FundingWalletProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FundingWalletProgressDialog(quint64 fundingAmount, const BitcoinDisplaySettings * bitcoinDisplaySettings, QWidget *parent = 0);
    ~FundingWalletProgressDialog();

    // Text conversion routines
    QString fundingAmountToString();

private:
    Ui::FundingWalletProgressDialog *ui;

    //
    quint64 _fundingAmount;

    // Display settings for bitcoin
    const BitcoinDisplaySettings * _settings;
};

#endif // FUNDING_WALLET_PROGRESS_DIALOG_HPP
