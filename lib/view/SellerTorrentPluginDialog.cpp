#include "SellerTorrentPluginDialog.hpp"
#include "ui_SellerTorrentPluginDialog.h"
#include "controller/SellerTorrentPluginViewModel.hpp"
#include "controller/SellerPeerPluginViewModel.hpp"
#include "SellerPeerPluginView.hpp"

#include "extension/BitCoin/BitCoin.hpp"
#include "Utilities.hpp" // time conversion

SellerTorrentPluginDialog::SellerTorrentPluginDialog(QWidget * parent,
                                                     const SellerTorrentPluginViewModel * model)
    : QDialog(parent)
    , ui(new Ui::SellerTorrentPluginDialog) {

    ui->setupUi(this);

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
                     SLOT(updateMinFeePerByte(quint64)));

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

QString SellerTorrentPluginDialog::minPriceToString(quint64 minPrice) {
    return QString::number(minPrice) + "Ƀ";
}

QString SellerTorrentPluginDialog::minLockTimeToString(quint32 minLockTime) {
    return QString::number(minLockTime) + "s";
}

QString SellerTorrentPluginDialog::minFeePerByteToString(quint64 minFeePerByte) {
    return QString::number(minFeePerByte) + "Ƀ";
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
    ui->minPriceLineEdit->setText(minPriceToString(minPrice));
}

void SellerTorrentPluginDialog::updateMinLockTime(quint32 minLockTime) {
    ui->minLockTimeEdit->setTime(Utilities::secondsToQTime(minLockTime));
}

void SellerTorrentPluginDialog::updateMinFeePerByte(quint64 minFeePerByte) {
    ui->minFeeLineEdit->setText(minFeePerByteToString(minFeePerByte));
}

void SellerTorrentPluginDialog::updateMaxNumberOfSellers(quint32 maxNumberOfSellers) {
    ui->maxNumberOfSellersLineEdit->setText(QString::number(maxNumberOfSellers));
}

void SellerTorrentPluginDialog::updateMaxContractConfirmationDelay(quint32 delay) {
    ui->maxConfirmationTimeTimeEdit->setTime(Utilities::secondsToQTime(delay));
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
