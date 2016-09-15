/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/MainWindow.hpp>
#include "ui_MainWindow.h"
#include <gui/TorrentTreeViewRow.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <QCloseEvent>

namespace joystream {
namespace classic {
namespace gui {

MainWindow::MainWindow(const QString & title,
                       unsigned int applicationMajorVersion,
                       unsigned int applicationMinorVersion,
                       unsigned int applicationPatchVersion,
                       const BitcoinDisplaySettings * settings)
    : ui(new Ui::MainWindow)
    , _torrentTreeViewModel(0, 9)
    , _bitcoinDisplaySettings(settings) {

    ui->setupUi(this);

    /**
     * Look of main windw
     */

    // Alter window title
    setWindowTitle(title);

    // Set window Icon
    //setWindowIcon(QIcon(":/prefix/test") );

    // Status bar
    _statusLabel.setText("Release v" + QString::number(applicationMajorVersion) + "."
                                     + QString::number(applicationMinorVersion) + "."
                                     + QString::number(applicationPatchVersion));

    _statusBar.addWidget(&_statusLabel);
    setStatusBar(&_statusBar);

    /**
     * Balance labels
     */

    // earnedBalanceLabel
    QPalette earnedBalancePalette = ui->earnedBalanceLabel->palette();
    earnedBalancePalette.setColor(ui->earnedBalanceLabel->foregroundRole(), Qt::green);
    ui->earnedBalanceLabel->setPalette(earnedBalancePalette);
    ui->earnedTitleQlabel->setPalette(earnedBalancePalette);
    ui->earnedBalanceLabel->setAlignment(Qt::AlignRight);

    // spentBalanceLabel
    QPalette spentBalancePalette = ui->spentBalanceLabel->palette();
    spentBalancePalette.setColor(ui->spentBalanceLabel->foregroundRole(), Qt::red);
    ui->spentBalanceLabel->setPalette(spentBalancePalette);
    ui->spentTitleLabel->setPalette(spentBalancePalette);
    ui->spentBalanceLabel->setAlignment(Qt::AlignRight);

    // unconfirmedBalanceLabel
    QPalette unconfirmedBalanceLabelPalette = ui->unconfirmedBalanceLabel->palette();
    unconfirmedBalanceLabelPalette.setColor(ui->unconfirmedBalanceLabel->foregroundRole(), Qt::magenta);
    ui->unconfirmedBalanceLabel->setPalette(unconfirmedBalanceLabelPalette);
    ui->unconfirmedBalanceTitleLabel->setPalette(unconfirmedBalanceLabelPalette);
    ui->unconfirmedBalanceLabel->setAlignment(Qt::AlignRight);

    // balanceLabel
    QPalette balanceLabelPalette = ui->balanceLabel->palette();
    balanceLabelPalette.setColor(ui->balanceLabel->foregroundRole(), Qt::blue);
    ui->balanceLabel->setPalette(balanceLabelPalette);
    ui->balanceTitleLabel->setPalette(balanceLabelPalette);
    ui->balanceLabel->setAlignment(Qt::AlignRight);

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << "Name"
                << "Size"
                << "State"
                << "Upload Speed"
                << "Download Speed"
                << "#Buyers"
                << "#Sellers"
                << "Mode"
                << "Balance";

    _torrentTreeViewModel.setHorizontalHeaderLabels(columnNames);

    // Set table model to view model
    ui->torrentsTreeView->setModel(&_torrentTreeViewModel);

    // Set column width
    ui->torrentsTreeView->setColumnWidth(0, 350);
    ui->torrentsTreeView->setColumnWidth(1, 90);
    ui->torrentsTreeView->setColumnWidth(2, 140);
    ui->torrentsTreeView->setColumnWidth(3, 190);
    ui->torrentsTreeView->setColumnWidth(4, 190);
    ui->torrentsTreeView->setColumnWidth(5, 70);
    ui->torrentsTreeView->setColumnWidth(6, 70);
    ui->torrentsTreeView->setColumnWidth(7, 90);
    ui->torrentsTreeView->setColumnWidth(8, 50);

    // Setup context menu capacity on table view
    ui->torrentsTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->torrentsTreeView,
            &QTreeView::customContextMenuRequested,
            this,
            &MainWindow::contextMenuRequestedAtTreeViewPoint);

    // Setup mouse click on torrent table view
    connect(ui->torrentsTreeView,
            &QTreeView::clicked,
            this,
            &MainWindow::torrentTreeViewClicked);

    // Accept drag and drop
    setAcceptDrops(true);
}

MainWindow::~MainWindow() {

    // Delete Ui::MainWindow
    delete ui;
}

void MainWindow::addToTorrentsTreeView(QStandardItem * nameItem,
                                       QStandardItem * sizeItem,
                                       QStandardItem * stateItem,
                                       QStandardItem * uploadSpeedItem,
                                       QStandardItem * downloadSpeedItem,
                                       QStandardItem * numberOfBuyerPeersItem,
                                       QStandardItem * numberOfSellerPeersitem,
                                       QStandardItem * sessionModeItem,
                                       QStandardItem * balanceItem) {

    // Add row to view model, which takes ownership of items
    QList<QStandardItem *> items;

    items << nameItem
          << sizeItem
          << stateItem
          << uploadSpeedItem
          << downloadSpeedItem
          << numberOfBuyerPeersItem
          << numberOfSellerPeersitem
          << sessionModeItem
          << balanceItem;

    _torrentTreeViewModel.appendRow(items);
}

void MainWindow::removeFromTorrentsTreeView(int row) {
    _torrentTreeViewModel.removeRow(row);
}

void MainWindow::setTotalSentSinceStart(quint64 sentSinceStart) {

    QString stringSpentBalance = BitcoinRepresentation(sentSinceStart).toString(_bitcoinDisplaySettings);
    ui->spentBalanceLabel->setText(stringSpentBalance);
}

void MainWindow::setTotalReceivedSinceStart(quint64 receivedSinceStart) {

    QString stringEarnedBalance = BitcoinRepresentation(receivedSinceStart).toString(_bitcoinDisplaySettings);
    ui->earnedBalanceLabel->setText(stringEarnedBalance);
}

void MainWindow::setConfirmedBalance(quint64 confirmedBalance) {

    QString stringConfirmedBalance = BitcoinRepresentation(confirmedBalance).toString(_bitcoinDisplaySettings);
    ui->balanceLabel->setText(stringConfirmedBalance);
}

void MainWindow::setUnconfirmedBalance(quint64 unconfirmedBalance) {

    QString stringUnconfirmedBalance = BitcoinRepresentation(unconfirmedBalance).toString(_bitcoinDisplaySettings);
    ui->unconfirmedBalanceLabel->setText(stringUnconfirmedBalance);
}

void MainWindow::walletSynched() {

}

void MainWindow::walletConnected() {

}

void MainWindow::walletDisconnected() {

}

void MainWindow::maximize() {
    setWindowState(Qt::WindowState::WindowActive);
}

void MainWindow::minimize() {
    setWindowState(Qt::WindowState::WindowMinimized);
}

QModelIndex MainWindow::indexAtTorrentTreeViewPosition(const QPoint & pos) const noexcept {
    return ui->torrentsTreeView->indexAt(pos);
}

void MainWindow::closeEvent(QCloseEvent *event) {

    event->ignore();

    emit ignoredCloseEventOccured();
}

}
}
}

