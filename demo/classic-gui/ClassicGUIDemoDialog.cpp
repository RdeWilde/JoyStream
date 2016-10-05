/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include "ClassicGUIDemoDialog.hpp"
#include "ui_ClassicGUIDemoDialog.h"
#include <gui/gui.hpp>

#include <common/BitcoinDisplaySettings.hpp>
#include <extension/BEPSupportStatus.hpp>
#include <protocol_wire/protocol_wire.hpp>

#include <libtorrent/socket_io.hpp>

namespace joystream {
namespace demo {

ClassicGUIDemoDialog::ClassicGUIDemoDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClassicGUIDemoDialog) {

    ui->setupUi(this);
}

ClassicGUIDemoDialog::~ClassicGUIDemoDialog() {
    delete ui;
}

void ClassicGUIDemoDialog::on_MainWindowPushButton_clicked() {

    BitcoinDisplaySettings settings;

    classic::gui::MainWindow window("JoyStream Classic GUI Demo",
                                    0,
                                    0,
                                    0,
                                    &settings);

    window.show();

    // Setup model
    classic::gui::TorrentTableModel model(&settings);

    libtorrent::sha1_hash infoHash("e940a7a57294e4c98f62514b32611e38181b6cae");

    for(int i = 0;i < 11;i++) {

        infoHash <<= 1; // shift 1 bit to make unique for this torrent

        classic::gui::TorrentTableRowModel * rowModel = model.add(infoHash);

        rowModel->setName("First test torrent");
        rowModel->setSize(1024*10);
        rowModel->setState(libtorrent::torrent_status::state_t::checking_resume_data, 0.55);
        rowModel->setPaused(false);
        rowModel->setUploadSpeed(0);
        rowModel->setDownloadSpeed(0);
        rowModel->setNumberOfBuyers(11);
        rowModel->setNumberOfSellers(12);
        //void setSessionMode(protocol_session::SessionMode::);
        rowModel->setBalance(100000);
    }

    window.setTorrentTreeViewModel(model.model());

    // Start local event loop
    QEventLoop loop;
    QObject::connect(&window,
                     &classic::gui::MainWindow::ignoredCloseEventOccured,
                     &loop,
                     &QEventLoop::quit);

    loop.exec();
}

void ClassicGUIDemoDialog::on_PeersDialogPushButton_clicked() {

    BitcoinDisplaySettings settings;

    classic::gui::PeersDialog peersDialog(this);

    peersDialog.show();

    // Setup model
    classic::gui::PeerTableModel peerTableModel(&settings);

    // Peers
    for(int i = 0;i < 10;i++) {

        libtorrent::error_code ec;
        libtorrent::tcp::endpoint endPoint = libtorrent::parse_endpoint(std::string("198.9.") + std::to_string(i) + ".12:800", ec);
        assert(!ec);

        classic::gui::PeerTableRowModel * rowModel = peerTableModel.add(endPoint);

        rowModel->setHost(endPoint);
        rowModel->setClientName("uTorrent");
        rowModel->setBEPSupport(extension::BEPSupportStatus::supported);
    }

    peersDialog.setPeerTreeViewModel(peerTableModel.model());

    // Buyers
    classic::gui::BuyerTableModel buyerTableModel(&settings);

    for(int i = 0;i < 10;i++) {

        libtorrent::error_code ec;
        libtorrent::tcp::endpoint endPoint = libtorrent::parse_endpoint(std::string("198.9.") + std::to_string(i) + ".12:800", ec);
        assert(!ec);

        classic::gui::BuyerTableRowModel * rowModel = buyerTableModel.add(endPoint);

        rowModel->setHost(endPoint);
        rowModel->setTerms(protocol_wire::BuyerTerms(i,i,i,i,i));
    }

    peersDialog.setBuyerTreeViewModel(buyerTableModel.model());

    // Sellers
    classic::gui::SellerTableModel sellerTableModel(&settings);

    for(int i = 0;i < 10;i++) {

        libtorrent::error_code ec;
        libtorrent::tcp::endpoint endPoint = libtorrent::parse_endpoint(std::string("198.9.") + std::to_string(i) + ".12:800", ec);
        assert(!ec);

        classic::gui::SellerTableRowModel * rowModel = sellerTableModel.add(endPoint);

        rowModel->setHost(endPoint);
        rowModel->setTerms(protocol_wire::SellerTerms(i,i,i,i,i));
    }

    peersDialog.setSellerTreeViewModel(sellerTableModel.model());

    // Observers
    classic::gui::ObserverTableModel observerTableModel(&settings);

    for(int i = 0;i < 10;i++) {

        libtorrent::error_code ec;
        libtorrent::tcp::endpoint endPoint = libtorrent::parse_endpoint(std::string("198.9.") + std::to_string(i) + ".12:800", ec);
        assert(!ec);

        classic::gui::ObserverTableRowModel * rowModel = observerTableModel.add(endPoint);

        rowModel->setHost(endPoint);
    }

    peersDialog.setObserverTreeViewModel(observerTableModel.model());

    // Start local event loop
    QEventLoop loop;
    QObject::connect(&peersDialog,
                     &classic::gui::PeersDialog::accepted,
                     &loop,
                     &QEventLoop::quit);

    loop.exec();
}

void ClassicGUIDemoDialog::on_BuyingSessionDialogPushButton_clicked() {

    BitcoinDisplaySettings settings;

    classic::gui::SessionDialog sessionDialog(this);

    sessionDialog.show();

    // Setup model
    classic::gui::ConnectionTableModel connectionTableModel(&settings);

    // Add connections
    for(int i = 0;i < 10;i++) {

        libtorrent::error_code ec;
        libtorrent::tcp::endpoint endPoint = libtorrent::parse_endpoint(std::string("118.9.") + std::to_string(i) + ".12:800", ec);

        classic::gui::ConnectionTableRowModel * rowModel = connectionTableModel.add(endPoint);

        rowModel->setHost(endPoint);
        rowModel->setState(core::CBStateMachine::InnerStateIndex(typeid(protocol_statemachine::Observing)));
        rowModel->setFunds(i*2);
        rowModel->setRefundLockTime(i*13);
        rowModel->setPrice(i*i);
        rowModel->setNumberOfPayments(i*34);
        rowModel->setBalance(i*i*i);
    }

    // Show dialog with buying mode widet set
    classic::gui::BuyingModeSessionWidget * widget = new classic::gui::BuyingModeSessionWidget(&sessionDialog, &settings);

    widget->setSellerTreeViewModel(connectionTableModel.model());

    sessionDialog.showBuyModeWidget(widget);

    // Start local event loop
    QEventLoop loop;
    QObject::connect(&sessionDialog,
                     &classic::gui::SessionDialog::accepted,
                     &loop,
                     &QEventLoop::quit);

    loop.exec();
}

void ClassicGUIDemoDialog::on_SellingSessionDialogPushButton_clicked() {

    BitcoinDisplaySettings settings;

    classic::gui::SessionDialog sessionDialog(this);

    sessionDialog.show();

    // Setup model
    classic::gui::ConnectionTableModel connectionTableModel(&settings);

    // Add connections
    for(int i = 0;i < 10;i++) {

        libtorrent::error_code ec;
        libtorrent::tcp::endpoint endPoint = libtorrent::parse_endpoint(std::string("118.9.") + std::to_string(i) + ".12:800", ec);

        classic::gui::ConnectionTableRowModel * rowModel = connectionTableModel.add(endPoint);

        rowModel->setHost(endPoint);
        rowModel->setState(core::CBStateMachine::InnerStateIndex(typeid(protocol_statemachine::Observing)));
        rowModel->setFunds(i*2);
        rowModel->setRefundLockTime(i*13);
        rowModel->setPrice(i*i);
        rowModel->setNumberOfPayments(i*34);
        rowModel->setBalance(i*i*i);
    }

    // Show dialog with buying mode widet set
    classic::gui::SellingModeSessionWidget * widget = new classic::gui::SellingModeSessionWidget(&sessionDialog, &settings);

    widget->setBuyerTreeViewModel(connectionTableModel.model());

    sessionDialog.showSellModeWidget(widget);

    // Start local event loop
    QEventLoop loop;
    QObject::connect(&sessionDialog,
                     &classic::gui::SessionDialog::accepted,
                     &loop,
                     &QEventLoop::quit);

    loop.exec();
}

}
}
