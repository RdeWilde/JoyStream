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

    window.setTorrentTreeViewModel(model.standardItemModel());

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
    classic::gui::PeerTableModel model(&settings);

    for(int i = 0;i < 10;i++) {

        libtorrent::error_code ec;
        libtorrent::tcp::endpoint endPoint = libtorrent::parse_endpoint(std::string("198.9.") + std::to_string(i) + ".12:800", ec);
        assert(!ec);

        classic::gui::PeerTableRowModel * rowModel = model.add(endPoint);

        rowModel->setHost(endPoint);
        rowModel->setClientName("uTorrent");
        rowModel->setBEPSupport(extension::BEPSupportStatus::supported);
    }

    peersDialog.setPeerTreeViewModel(model.standardItemModel());

    // Start local event loop
    QEventLoop loop;
    QObject::connect(&peersDialog,
                     &classic::gui::PeersDialog::accepted,
                     &loop,
                     &QEventLoop::quit);

    loop.exec();
}

}
}
