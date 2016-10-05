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

    // peersTreeView
    ui->peersTreeView->setColumnWidth(0, 150);
    ui->peersTreeView->setColumnWidth(1, 100);
    ui->peersTreeView->setColumnWidth(2, 100);

    // observersTreeView
    ui->observersTreeView->setColumnWidth(0, 150);

    // buyersTreeView
    ui->buyersTreeView->setColumnWidth(0, 150);
    ui->buyersTreeView->setColumnWidth(1, 100);
    ui->buyersTreeView->setColumnWidth(2, 100);
    ui->buyersTreeView->setColumnWidth(3, 100);
    ui->buyersTreeView->setColumnWidth(4, 100);

    // sellersTreeView
    ui->sellersTreeView->setColumnWidth(0, 150);
    ui->sellersTreeView->setColumnWidth(1, 100);
    ui->sellersTreeView->setColumnWidth(2, 100);
    ui->sellersTreeView->setColumnWidth(3, 100);
    ui->sellersTreeView->setColumnWidth(4, 100);
    ui->sellersTreeView->setColumnWidth(5, 100);
}

PeersDialog::~PeersDialog()
{
    delete ui;
}

void PeersDialog::setPeerTreeViewModel(QAbstractItemModel * model) {
    ui->peersTreeView->setModel(model);
}

void PeersDialog::setObserverTreeViewModel(QAbstractItemModel * model) {
    ui->observersTreeView->setModel(model);
}

void PeersDialog::setBuyerTreeViewModel(QAbstractItemModel * model) {
    ui->buyersTreeView->setModel(model);
}

void PeersDialog::setSellerTreeViewModel(QAbstractItemModel * model) {
    ui->sellersTreeView->setModel(model);
}

}
}
}
