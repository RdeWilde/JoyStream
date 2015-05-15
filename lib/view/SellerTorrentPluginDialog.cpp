#include "SellerTorrentPluginDialog.hpp"
#include "ui_SellerTorrentPluginDialog.h"
#include "controller/SellerTorrentPluginViewModel.hpp"

#include "extension/BitCoin/BitCoin.hpp"

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


    updateConfiguration(model->configuration());

    // Set table view model
    ui->peerPluginsTableView->setModel(model->sellerPeerPluginTableViewModel());*/
}

SellerTorrentPluginDialog::~SellerTorrentPluginDialog() {
    delete ui;
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

#include "Utilities.hpp"

/**
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
*/

/**
 *
void SellerPeerPluginViewModel::update(const SellerPeerPlugin::Status & status) {

    // Get state
    SellerPeerPlugin::ClientState clientState = status.clientState();

    // Update state field
    updateClientState(clientState);

    // Update other fields if state has moved sufficiently far along
    if(clientState == SellerPeerPlugin::ClientState::signed_refund ||
       clientState == SellerPeerPlugin::ClientState::ignored_sign_refund_invitation ||
       clientState == SellerPeerPlugin::ClientState::awaiting_fullpiece_request_after_ready_announced ||
       clientState == SellerPeerPlugin::ClientState::awaiting_payment ||
       clientState == SellerPeerPlugin::ClientState::awaiting_piece_request_after_payment ||
       clientState == SellerPeerPlugin::ClientState::reading_piece_from_disk) {

        const Payee::Status & payeeStatus = status.payeeStatus();

        //updateContractOutPointItem(payeeStatus.); // contractOutPoint
        updateFunds(payeeStatus.funds());
        updatePrice(payeeStatus.price());
        updateNumberOfPaymentsMadeItem(payeeStatus.numberOfPaymentsMade());
        updateBalanceItem(payeeStatus.price() * payeeStatus.numberOfPaymentsMade());
    }
}

void SellerPeerPluginViewModel::updateClientState(SellerPeerPlugin::ClientState state) {

    // Get text representation of state
    QString clientState;

    switch(state) {

        case SellerPeerPlugin::ClientState::no_bitswapr_message_sent:
            clientState = "No message sent";
            break;

        case SellerPeerPlugin::ClientState::seller_mode_announced:
            clientState = "Announced seller mode";
            break;

        case SellerPeerPlugin::ClientState::joined_contract:
            clientState = "Joined contract";
            break;

        case SellerPeerPlugin::ClientState::ignored_contract_invitation:
            clientState = "Ignored contract";
            break;

        case SellerPeerPlugin::ClientState::signed_refund:
            clientState = "Signed refund";
            break;

        case SellerPeerPlugin::ClientState::ignored_sign_refund_invitation:
            clientState = "Ignored refund request";
            break;

        case SellerPeerPlugin::ClientState::awaiting_fullpiece_request_after_ready_announced:
            clientState = "Awaiting piece request";
            break;

        case SellerPeerPlugin::ClientState::awaiting_payment:
            clientState = "Awaiting payment ";
            break;

        case SellerPeerPlugin::ClientState::awaiting_piece_request_after_payment:
            clientState = "Awaiting piece request";
            break;

        case SellerPeerPlugin::ClientState::reading_piece_from_disk:
            clientState = "Reading from disk";
            break;
    }

    // Update model view
    _clientStateItem->setText(clientState);
}

void SellerPeerPluginViewModel::updateContractOutPointItem(const OutPoint & outPoint) {
    _contractOutPointItem->setText(outPoint.toString());
}

void SellerPeerPluginViewModel::updateFunds(quint64 funds) {
    _fundsItem->setText(QString::number(funds));
}

void SellerPeerPluginViewModel::updateLockTime(const QDateTime & lockTime) {

    // Get local string version of time
    QString lockTimeString = lockTime.toLocalTime().toString();

    // Update view model
    _fundsItem->setText(lockTimeString);
}

void SellerPeerPluginViewModel::updatePrice(quint64 price) {

    // Get string version of price
    QString priceString = QString::number(price);

    // Update view model
    _priceItem->setText(priceString);
}

void SellerPeerPluginViewModel::updateNumberOfPaymentsMadeItem(quint64 numPayments) {

    // Get string version of price
    QString numberOfPaymentsMadeItemString = QString::number(numPayments);

    // Update view model
    _priceItem->setText(numberOfPaymentsMadeItemString);
}

void SellerPeerPluginViewModel::updateBalanceItem(quint64 balance) {

    // Get string version of price
    QString updateBalanceItemItemString = QString::number(balance);

    // Update view model
    _priceItem->setText(updateBalanceItemItemString);
}
*/
