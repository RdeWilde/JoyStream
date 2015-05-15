#include "BuyerPeerPluginView.hpp"
#include "controller/BuyerPeerPluginViewModel.hpp"

#include <libtorrent/socket_io.hpp> // print_endpoint

#include <QStandardItem>
#include <QStandardItemModel>

BuyerPeerPluginView::BuyerPeerPluginView(QObject * parent,
                                         const BuyerPeerPluginViewModel * peerModel,
                                         QStandardItemModel * model)
    : QObject(parent)
    , _endPointItem(new QStandardItem(endPointToString(peerModel->endPoint())))
    , _clientStateItem(new QStandardItem(clientStateToString(peerModel->status().clientState())))
    , _payorSlotItem(new QStandardItem(payorSlotToString(peerModel->status().payorSlot()))) {

    // Add row to model
    QList<QStandardItem *> items;

    items << _endPointItem
          << _clientStateItem
          << _payorSlotItem;

    model->appendRow(items);

    // Connect view model signals to slots of this view object
    QObject::connect(peerModel,
                     SIGNAL(clientStateChanged(BuyerPeerPlugin::ClientState)),
                     this,
                     SLOT(updateClientState(BuyerPeerPlugin::ClientState)));

    QObject::connect(peerModel,
                     SIGNAL(payorSlotChanged(quint32)),
                     this,
                     SLOT(updatePayorSlot(quint32)));
}

QString BuyerPeerPluginView::endPointToString(const libtorrent::tcp::endpoint & endPoint) {

    std::string endPointString = libtorrent::print_endpoint(endPoint);
    return QString::fromStdString(endPointString);
}

QString BuyerPeerPluginView::clientStateToString(BuyerPeerPlugin::ClientState state) {

    // Get text representation of state
    QString text;

    switch(state) {

        case BuyerPeerPlugin::ClientState::no_bitswapr_message_sent:
            text = "No message sent";
            break;

        case BuyerPeerPlugin::ClientState::buyer_mode_announced:
            text = "Announced buyer mode";
            break;

        case BuyerPeerPlugin::ClientState::invited_to_contract:
            text = "Invited to contract";
            break;

        case BuyerPeerPlugin::ClientState::ignored_join_contract_from_peer:
            text = "Ignored join contract";
            break;

        case BuyerPeerPlugin::ClientState::asked_for_refund_signature:
            text = "Requested refund";
            break;

        case BuyerPeerPlugin::ClientState::received_valid_refund_signature_and_waiting_for_others:
            text = "Received valid refund";
            break;

        case BuyerPeerPlugin::ClientState::needs_to_be_assigned_piece:
            text = "Needs to be assigned piece";
            break;

        case BuyerPeerPlugin::ClientState::waiting_for_full_piece:
            text = "Waiting for piece ";
            break;

        case BuyerPeerPlugin::ClientState::waiting_for_libtorrent_to_validate_piece:
            text = "Validating piece";
            break;
    }

    return text;
}

QString BuyerPeerPluginView::payorSlotToString(quint32 payorSlot) {
    return QString::number(payorSlot);
}

void BuyerPeerPluginView::updateEndPoint(const libtorrent::tcp::endpoint & endPoint) {
    _endPointItem->setText(endPointToString(endPoint));
}

void BuyerPeerPluginView::updateClientState(BuyerPeerPlugin::ClientState clientState) {
    _clientStateItem->setText(clientStateToString(clientState));
}

void BuyerPeerPluginView::updatePayorSlot(quint32 payorSlot) {
    _payorSlotItem->setText(payorSlotToString(payorSlot));
}
