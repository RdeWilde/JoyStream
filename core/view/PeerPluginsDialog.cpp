/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "PeerPluginsDialog.hpp"
#include "ui_PeerPluginsDialognsdialog.h"

PeerPluginsDialog::PeerPluginsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PeerPluginsDialog)
{
    ui->setupUi(this);
}

PeerPluginsDialog::~PeerPluginsDialog()
{
    delete ui;
}
