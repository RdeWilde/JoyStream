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

    // Set view of configuration
    /**
     * FUTURE
     * connect plugin view model configuraion update signals
     * to slots on this dialog/view object

    QObject::connect(SellerTorrentPluginViewModel,
                     SIGNAL(),)


    // Set table view model
    ui->peerPluginsTableView->setModel(_sellerPeerPluginTableViewModel);*/
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

    _sellerPeerPluginViews[endPoint] = new SellerPeerPluginView(this, model, &_sellerPeerPluginTableViewModel);
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
