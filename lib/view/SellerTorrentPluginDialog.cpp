#include "SellerTorrentPluginDialog.hpp"
#include "ui_SellerTorrentPluginDialog.h"
#include "SellerTorrentPluginViewModel.hpp"

#include "extension/BitCoin/BitCoin.hpp"

SellerTorrentPluginDialog::SellerTorrentPluginDialog(QWidget * parent, SellerTorrentPluginViewModel * model)
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
     */

    updateConfiguration(model->configuration());

    // Set table view model
    ui->peerPluginsTableView->setModel(model->sellerPeerPluginTableViewModel());
}

SellerTorrentPluginDialog::~SellerTorrentPluginDialog() {
    delete ui;
}

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

#include "Utilities.hpp"

void SellerTorrentPluginDialog::updateConfiguration(const SellerTorrentPlugin::Configuration & configuration) {

    // minPrice
    QString minPrice = QString::number(configuration.minPrice()/SATOSHIES_PER_M_BTC);
    ui->minPriceLineEdit->setText(minPrice);

    // minLockTime
    quint32 minLockTimeHours = Utilities::hoursInSeconds(configuration.minLock());
    quint32 minLockTimeMinutes = Utilities::minutesInSeconds(configuration.minLock());
    quint32 minLockTimeSeconds = Utilities::secondsInSeconds(configuration.minLock());

    QTime minLockTime(minLockTimeHours, minLockTimeMinutes, minLockTimeSeconds);
    ui->minLockTimeEdit->setTime(minLockTime);

    // minFeePerKByte
    QString minFeePerKByte = QString::number(configuration.minFeePerByte()/SATOSHIES_PER_M_BTC);
    ui->minFeeLineEdit->setText(minFeePerKByte);

    // maxNumberOfSellers
    QString maxNumberOfSellers = QString::number(configuration.maxNumberOfSellers());
    ui->maxNumberOfSellersLineEdit->setText(maxNumberOfSellers);

    // maxContractConfirmationDelay
    quint32 maxContractConfirmationDelayHours = Utilities::hoursInSeconds(configuration.minLock());
    quint32 maxContractConfirmationDelayMinutes = Utilities::minutesInSeconds(configuration.minLock());
    quint32 maxContractConfirmationDelaySeconds = Utilities::secondsInSeconds(configuration.minLock());
    QTime maxContractConfirmationDelay(maxContractConfirmationDelayHours, maxContractConfirmationDelayMinutes, maxContractConfirmationDelaySeconds);

    ui->maxConfirmationTimeTimeEdit->setTime(maxContractConfirmationDelay);
}
