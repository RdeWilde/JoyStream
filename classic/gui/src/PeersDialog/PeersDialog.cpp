/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 23 2016
 */

#include <gui/PeersDialog/PeersDialog.hpp>
#include "ui_PeersDialog.h"

#include <QStandardItemModel>

namespace joystream {
namespace classic {
namespace gui {

PeersDialog::PeersDialog(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::PeersDialog){

    ui->setupUi(this);
}

PeersDialog::~PeersDialog()
{
    delete ui;
}

void PeersDialog::setPeerTreeViewModel(QAbstractItemModel * model) {
    ui->peersTreeView->setModel(model);
}

void PeersDialog::setObserverConnectionTreeViewModel(QAbstractItemModel * model) {
    ui->observersTreeView->setModel(model);
}

void PeersDialog::setBuyerConnectionTreeViewModel(QAbstractItemModel * model) {
    ui->buyersTreeView->setModel(model);
}

void PeersDialog::setSellerConnectionTreeViewModel(QAbstractItemModel * model) {
    ui->sellersTreeView->setModel(model);
}

}
}
}