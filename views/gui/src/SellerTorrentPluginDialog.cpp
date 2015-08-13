/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/SellerTorrentPluginDialog.hpp>
#include <gui/SellerPeerPluginView.hpp>
#include "ui_SellerTorrentPluginDialog.h"
#include <core/controller/SellerTorrentPluginViewModel.hpp>
#include <core/controller/SellerPeerPluginViewModel.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <common/BitcoinDisplaySettings.hpp>
#include <common/TimeTools.hpp> // time conversion
#include <common/Bitcoin.hpp>

SellerTorrentPluginDialog::SellerTorrentPluginDialog(QWidget * parent,
                                                     const SellerTorrentPluginViewModel * model,
                                                     const BitcoinDisplaySettings * settings)
    : QDialog(parent)
    , ui(new Ui::SellerTorrentPluginDialog)
    , _settings(settings) {

    ui->setupUi(this);

    // Main fields
    updateMinPrice(model->minPrice());
    updateMinLockTime(model->minLock());
    updateMinFeePerkB(model->minFeePerByte());
    updateMaxNumberOfSellers(model->maxNumberOfSellers());
    updateMaxContractConfirmationDelay(model->maxContractConfirmationDelay());

    // Hide confirmation time, since we dont use it yet
    //ui->maxConfirmationTimeTimeEdit->setVisible(false);
    //ui->maxConfirmationTimeLabel->setVisible(false);

    /**
     * Connect model signals to view slots
     */

    QObject::connect(model,
                     SIGNAL(peerAdded(const SellerPeerPluginViewModel*)),
                     this,
                     SLOT(addPeer(const SellerPeerPluginViewModel*)));

    QObject::connect(model,
                     SIGNAL(peerRemoved(libtorrent::tcp::endpoint)),
                     this,
                     SLOT(removePeer(libtorrent::tcp::endpoint)));

    QObject::connect(model,
                     SIGNAL(minPriceChanged(quint64)),
                     this,
                     SLOT(updateMinPrice(quint64)));

    QObject::connect(model,
                     SIGNAL(minLockChanged(quint32)),
                     this,
                     SLOT(updateMinLockTime(quint32)));

    QObject::connect(model,
                     SIGNAL(minFeePerByteChanged(quint64)),
                     this,
                     SLOT(updateMinFeePerkB(quint64)));

    QObject::connect(model,
                     SIGNAL(maxNumberOfSellersChanged(quint32)),
                     this,
                     SLOT(updateMaxNumberOfSellers(quint32)));

    QObject::connect(model,
                     SIGNAL(maxContractConfirmationDelayChanged(quint32)),
                     this,
                     SLOT(updateMaxContractConfirmationDelay(quint32)));

    /**
     * Setup table
     */

    // Add columns to channel table view model
    QStringList peersTableColumnNames;

    peersTableColumnNames << "Host"
                          << "State"
                          << "Contract"
                          << "Funds"
                          << "Lock"
                          << "Price"
                          << "#Payments"
                          << "Balance";

    _sellerPeerPluginTableViewModel.setHorizontalHeaderLabels(peersTableColumnNames);

    // Create peer views and connect to table view model
    QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *> sellerPeerPluginViewModels = model->sellerPeerPluginViewModels();

    for(QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *>::const_iterator
        i = sellerPeerPluginViewModels.constBegin(),
        end = sellerPeerPluginViewModels.constEnd();
        i != end;i++)
        addPeer(i.value());

    // Set table view model
    ui->peerPluginsTableView->setModel(&_sellerPeerPluginTableViewModel);
}

SellerTorrentPluginDialog::~SellerTorrentPluginDialog() {
    delete ui;
}

QString SellerTorrentPluginDialog::minPriceToString(quint64 minPrice, const BitcoinDisplaySettings * settings) {
    return QString::number(minPrice);
    //return BitCoinRepresentation(minPrice).toString(settings);

    //BitCoinRepresentation rep(minPrice);
    //rep.unitsWithPrefix()

}

QString SellerTorrentPluginDialog::minLockTimeToString(quint32 minLockTime) {
    return QString::number(minLockTime);
}

QString SellerTorrentPluginDialog::minFeePerkBToString(quint64 minFeePerByte) {
    return QString::number((float)minFeePerByte/SATOSHIES_PER_M_BTC);
}

void SellerTorrentPluginDialog::addPeer(const SellerPeerPluginViewModel * model) {

    libtorrent::tcp::endpoint endPoint = model->endPoint();

    Q_ASSERT(!_sellerPeerPluginViews.contains(endPoint));

    // Creating row items
    QStandardItem * endPointItem = new QStandardItem(),
                  * clientStateItem = new QStandardItem(),
                  * contractOutPointItem = new QStandardItem(),
                  * fundsItem = new QStandardItem(),
                  * refundLockTimeItem = new QStandardItem(),
                  * priceItem = new QStandardItem(),
                  * numberOfPaymentMadeItem = new QStandardItem(),
                  * balanceItem = new QStandardItem();

    // Create item row
    QList<QStandardItem *> items;

    items << endPointItem
          << clientStateItem
          << contractOutPointItem
          << fundsItem
          << refundLockTimeItem
          << priceItem
          << numberOfPaymentMadeItem
          << balanceItem;

    // Add row to model
    _sellerPeerPluginTableViewModel.appendRow(items);

    // Create peer view
    _sellerPeerPluginViews[endPoint] = new SellerPeerPluginView(this,
                                                                model,
                                                                _settings,
                                                                endPointItem,
                                                                clientStateItem,
                                                                contractOutPointItem,
                                                                fundsItem,
                                                                refundLockTimeItem,
                                                                priceItem,
                                                                numberOfPaymentMadeItem,
                                                                balanceItem);
}

void SellerTorrentPluginDialog::removePeer(const libtorrent::tcp::endpoint & endPoint) {

    Q_ASSERT(_sellerPeerPluginViews.contains(endPoint));

    // Take out peer plugin view
    SellerPeerPluginView * view = _sellerPeerPluginViews.take(endPoint);

    // Delete view
    delete view;
}

void SellerTorrentPluginDialog::updateMinPrice(quint64 minPrice) {
    ui->minPriceLineEdit->setText(minPriceToString(minPrice, _settings));
}

void SellerTorrentPluginDialog::updateMinLockTime(quint32 minLockTime) {
    ui->minLockTimeEdit->setTime(TimeTools::secondsToQTime(minLockTime));
}

void SellerTorrentPluginDialog::updateMinFeePerkB(quint64 minFeePerByte) {
    ui->minFeeLineEdit->setText(minFeePerkBToString(minFeePerByte));
}

void SellerTorrentPluginDialog::updateMaxNumberOfSellers(quint32 maxNumberOfSellers) {
    ui->maxNumberOfSellersLineEdit->setText(QString::number(maxNumberOfSellers));
}

void SellerTorrentPluginDialog::updateMaxContractConfirmationDelay(quint32 delay) {
    ui->maxConfirmationTimeTimeEdit->setTime(TimeTools::secondsToQTime(delay));
}

/**
SellerTorrentPlugin::Configuration SellerTorrentPluginDialog::configuration() const {

    // minPrice
    bool okMinPrice;
    quint32 minPrice = SATOSHIES_PER_M_BTC * ui->minPriceLineEdit->text().toInt();

    if(!okMinPrice || minPrice < 0)
        throw std::exception("Invalid minPrice field.");

    // minLockTime
    QTime minLockTime = ui->minLockTimeEdit->time();
    quint32 minLock = minLockTime.hour()*3600 + minLockTime.minute()*60 + minLockTime.second();

    // minFeePerKByte
    bool okMinFeePerKByte;
    quint32 minFeePerKByte = ui->minFeeLineEdit->text().toInt(&okMinFeePerKByte);

    if(!okMinFeePerKByte || minFeePerKByte < 0)
        throw std::exception("Invalid minFeePerKByte field.");

    // maxNumberOfSellers
    bool okMaxNumberOfSellers;
    quint32 maxNumberOfSellers = ui->maxNumberOfSellersLineEdit->text().toInt(&okMaxNumberOfSellers);

    if(!okMaxNumberOfSellers || maxNumberOfSellers < 1)
        throw std::exception("Invalid maxNumberOfSellers field.");

    // maxContractConfirmationDelay
    QTime maxContractConfirmationDelayTime = ui->maxConfirmationTimeTimeEdit->time();
    quint32 maxContractConfirmationDelay = maxContractConfirmationDelayTime.hour()*3600 + maxContractConfirmationDelayTime.minute()*60 + maxContractConfirmationDelayTime.second();;

    // Tell controller to start plugin
    return SellerTorrentPlugin::Configuration(true,
                                            minPrice,
                                            minLock,
                                            minFeePerKByte,
                                            maxNumberOfSellers,
                                            maxContractConfirmationDelay);
}
*/
