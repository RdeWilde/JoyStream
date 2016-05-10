/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 17 2015
 */

#include <gui/GeneralLoadingProgressDialog.hpp>
#include <QCloseEvent>

#include "ui_GeneralLoadingProgressDialog.h"

GeneralLoadingProgressDialog::GeneralLoadingProgressDialog(const QString title, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GeneralLoadingProgressDialog){

    ui->setupUi(this);

    setWindowTitle(title);

    // Set blank message
    ui->messageLabel->setText("");
}

GeneralLoadingProgressDialog::~GeneralLoadingProgressDialog()
{
    delete ui;
}

void GeneralLoadingProgressDialog::updateMessage(const QString message) {
    ui->messageLabel->setText(message);
}

void GeneralLoadingProgressDialog::closeEvent(QCloseEvent * event) {

    // Do not close
    event->ignore();
}
