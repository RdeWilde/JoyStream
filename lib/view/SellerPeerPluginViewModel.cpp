#include "SellerPeerPluginViewModel.hpp"
#include "SellerTorrentPluginViewModel.hpp"

#include <QStandardItem>

SellerPeerPluginViewModel::SellerPeerPluginViewModel(SellerTorrentPluginViewModel * parent)
    : QObject(parent) {
}

SellerPeerPluginViewModel::SellerPeerPluginViewModel(SellerTorrentPluginViewModel * parent, const libtorrent::tcp::endpoint & endPoint, QStandardItemModel * model)
    : QObject(parent)
    , _endPoint(endPoint)
    , _hostItem(new QStandardItem())
    , _clientStateItem(new QStandardItem())
    , _contractOutPointItem(new QStandardItem())
    , _fundsItem(new QStandardItem())
    , _lockTime(new QStandardItem())
    , _priceItem(new QStandardItem())
    , _numberOfPaymentsMadeItem(new QStandardItem())
    , _balanceItem(new QStandardItem()) {

    // Add as row to model
    QList<QStandardItem *> row;

    row.append(_hostItem);
    row.append(_clientStateItem);
    row.append(_contractOutPointItem);
    row.append(_fundsItem);
    row.append(_lockTime);
    row.append(_priceItem);
    row.append(_numberOfPaymentsMadeItem);
    row.append(_balanceItem);

    model->appendRow(row);
}

SellerPeerPluginViewModel::~SellerPeerPluginViewModel() {
}

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
