#include "SellerPeerPluginView.hpp"
#include "controller/SellerPeerPluginViewModel.hpp"

#include <libtorrent/socket_io.hpp> // print_endpoint

#include <QStandardItem>
#include <QStandardItemModel>

SellerPeerPluginView::SellerPeerPluginView(QObject * parent,
                                           const SellerPeerPluginViewModel * peerModel,
                                           QStandardItemModel * model)
    : QObject(parent)
    , _endPointItem(new QStandardItem())
    , _clientStateItem(new QStandardItem())
    , _contractOutPointItem(new QStandardItem())
    , _fundsItem(new QStandardItem())
    , _refundLockTimeItem(new QStandardItem())
    , _priceItem(new QStandardItem())
    , _numberOfPaymentMadeItem(new QStandardItem())
    , _balanceItem(new QStandardItem()) {

    // Add row to model
    QList<QStandardItem *> items;

    items << _endPointItem
          << _clientStateItem
          << _contractOutPointItem
          << _fundsItem
          << _refundLockTimeItem
          << _priceItem
          << _numberOfPaymentMadeItem
          << _balanceItem;

    model->appendRow(items);

    // Center content
    _endPointItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _clientStateItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _contractOutPointItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _fundsItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _refundLockTimeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _priceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _numberOfPaymentMadeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _balanceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Connect view model signals to slots of this view object
    QObject::connect(peerModel,
                     SIGNAL(clientStateChanged(SellerPeerPlugin::ClientState)),
                     this,
                     SLOT(updateClientState(SellerPeerPlugin::ClientState)));

    const PayeeViewModel * payeeViewModel = peerModel->payeeViewModel();

    QObject::connect(payeeViewModel,
                     SIGNAL(contractOutPointChanged(OutPoint)),
                     this,
                     SLOT(updateContractOutPointItem(OutPoint)));

    QObject::connect(payeeViewModel,
                     SIGNAL(numberOfPaymentsMadeChanged(quint64)),
                     this,
                     SLOT(updateNumberOfPaymentMade(quint64)));

    QObject::connect(payeeViewModel,
                     SIGNAL(lockTimeChanged(quint32)),
                     this,
                     SLOT(updateRefundLockTime(quint32)));

    QObject::connect(payeeViewModel,
                     SIGNAL(priceChanged(quint64)),
                     this,
                     SLOT(updatePrice(quint64)));

    QObject::connect(payeeViewModel,
                     SIGNAL(funds(quint64)),
                     this,
                     SLOT(updateFunds(quint64)));

}

QString SellerPeerPluginView::endPointToString(const libtorrent::tcp::endpoint & endPoint) {

    std::string endPointString = libtorrent::print_endpoint(endPoint);
    return QString::fromStdString(endPointString);
}

QString SellerPeerPluginView::clientStateToString(SellerPeerPlugin::ClientState state) {

    QString text;

    switch(state) {

        case SellerPeerPlugin::ClientState::no_bitswapr_message_sent:
            text = "No message sent";
            break;

        case SellerPeerPlugin::ClientState::seller_mode_announced:
            text = "Announced seller mode";
            break;

        case SellerPeerPlugin::ClientState::joined_contract:
            text = "Joined contract";
            break;

        case SellerPeerPlugin::ClientState::ignored_contract_invitation:
            text = "Ignored contract";
            break;

        case SellerPeerPlugin::ClientState::signed_refund:
            text = "Signed refund";
            break;

        case SellerPeerPlugin::ClientState::ignored_sign_refund_invitation:
            text = "Ignored refund request";
            break;

        case SellerPeerPlugin::ClientState::awaiting_fullpiece_request_after_ready_announced:
            text = "Awaiting piece request";
            break;

        case SellerPeerPlugin::ClientState::awaiting_payment:
            text = "Awaiting payment ";
            break;

        case SellerPeerPlugin::ClientState::awaiting_piece_request_after_payment:
            text = "Awaiting piece request";
            break;

        case SellerPeerPlugin::ClientState::reading_piece_from_disk:
            text = "Reading from disk";
            break;
    }

    return text;
}

QString SellerPeerPluginView::contractOutPointString(const OutPoint & o) {
    return o.toString();
}

QString SellerPeerPluginView::fundsToString(quint64 funds) {
    return QString::number(funds) + "Ƀ";
}

QString SellerPeerPluginView::refundLockTimeString(quint32 refundLockTime) {
    return QString::number(refundLockTime)+"s";
}

QString SellerPeerPluginView::priceToString(quint64 price) {
    return QString::number(price) + "Ƀ";
}

QString SellerPeerPluginView::numberOfPaymentMadeToString(quint32 numberOfPaymentMade) {
    return QString::number(numberOfPaymentMade);
}

QString SellerPeerPluginView::balanceToString(quint32 balance) {
return QString::number(balance) + "Ƀ";
}

void SellerPeerPluginView::updateEndPoint(const libtorrent::tcp::endpoint & endPoint) {
    _endPointItem->setText(endPointToString(endPoint));
}

void SellerPeerPluginView::updateClientState(SellerPeerPlugin::ClientState state) {
    _clientStateItem->setText(clientStateToString(state));
}

void SellerPeerPluginView::updateContractOutPointItem(const OutPoint & o) {
    _contractOutPointItem->setText(contractOutPointString(o));
}

void SellerPeerPluginView::updateFunds(quint64 funds) {
    _fundsItem->setText(fundsToString(funds));
}

void SellerPeerPluginView::updateRefundLockTime(quint32 refundLockTime) {
    _refundLockTimeItem->setText(refundLockTimeString(refundLockTime));
}

void SellerPeerPluginView::updatePrice(quint64 price) {
    _priceItem->setText(priceToString(price));
}

void SellerPeerPluginView::updateNumberOfPaymentMade(quint64 numberOfPaymentMade) {
    _numberOfPaymentMadeItem->setText(numberOfPaymentMadeToString(numberOfPaymentMade));
}

void SellerPeerPluginView::updateBalance(quint32 balance) {
    _balanceItem->setText(balanceToString(balance));
}
