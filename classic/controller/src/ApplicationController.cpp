/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include <controller/ApplicationController.hpp>
#include <controller/Torrent.hpp>
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace controller {

void ApplicationController::setTorrentTableModelHorizontalHeaderLabels(QStandardItemModel * model) noexcept {

    /**
     * Add columns to model view model
     */
    QStringList columnNames;

    columnNames << "Name"
                << "Size"
                << "State"
                << "Upload Speed"
                << "Download Speed"
                << "#Buyers"
                << "#Sellers"
                << "Mode"
                << "Balance";

    model->setHorizontalHeaderLabels(columnNames);
}

ApplicationController::ApplicationController()
    : _mainWindowTorrentTableModel(0, 9) {

    setTorrentTableModelHorizontalHeaderLabels(&_mainWindowTorrentTableModel);
}

ApplicationController::~ApplicationController() {

}

void ApplicationController::addTorrent(core::Torrent * t) {

    if(_torrents.find(t->infoHash()) != _torrents.cend())
        throw std::runtime_error("Torrent already added");

    // Create row model
    gui::TorrentTableRowModel * rowModel = _mainWindowTorrentTableModel.add(t->infoHash());

    // Create row
    Torrent * torrent = new Torrent(t,
                                    rowModel);

    //
    _torrents[t->infoHash()] = torrent;

    /**
    // Set inital values
    _torrentTreeViewRow->setName(torrent->name());

    if(torrent->isPaused())
        _torrentTreeViewRow->setPaused(true);
    else
        _torrentTreeViewRow->setState(torrent->state(), torrent->progress());

    _torrentTreeViewRow->setUploadSpeed(torrent->uploadRate());
    _torrentTreeViewRow->setDownloadSpeed(torrent->downloadRate());

    // When plugin is present, then set it up, otherwise set it as absent
    if(torrent->torrentPlugin())
        setTorrentPluginPresent(torrent->torrentPlugin());
    else
        setTorrentPluginAbsent();

    */
}

void ApplicationController::removeTorrent(const libtorrent::tcp::endpoint &) {

}


/// Wallet signals

//    // Update balance when it changes
//    qRegisterMetaType<uint64_t>("uint64_t");

//    QObject::connect(_wallet,
//                    &joystream::bitcoin::SPVWallet::balanceChanged,
//                    this,
//                    &MainWindow::on_updatedWalletBalance);

//    // Wallet Synchronised
//    QObject::connect(_wallet,
//                     SIGNAL(synched()),
//                     this,
//                     SLOT(walletSynched()));

//    QObject::connect(_wallet,
//                     SIGNAL(connected()),
//                     this,
//                     SLOT(walletConnected()));

//    void MainWindow::quitApplication() {

//        QMessageBox::StandardButton reply = QMessageBox::question(this, "Quit JoyStream", "Are you sure you want to exit?", QMessageBox::Yes|QMessageBox::No);

//        if (reply == QMessageBox::Yes)
//            _controller->begin_close();
//    }

//    void MainWindow::on_walletConnected() {
//        if(_wallet->unconfirmedBalance() < RELOAD_WALLET_LOWER_BOUND )
//            fundWalletFromFaucet();
//    }


//void MainWindow::startVLC(const libtorrent::sha1_hash & infoHash) {

//    // Turn info hash into hex string
//    std::string infoHashHexString = libtorrent::to_hex(infoHash.to_string());

//    // Use to build path
//    QString serverUrl = "http://localhost:" + QString::number(_controller->getServerPort()) + "/" + QString::fromStdString(infoHashHexString);

//    QUrl url(serverUrl);

//    QDesktopServices::openUrl(url);

////    // Start VLC at local host on given port asking for this info hash
////    qDebug() << "Starting VLC pointing at:" << url;

////    // Escaped VLC path
////    QString vlcExecutable = "\"C:/Program Files (x86)/VideoLAN/VLC/vlc.exe\"";

////    // Launch VLC pointing at URL
////    _processLauncher.start(vlcExecutable, QStringList() << url);

////    // Check that it started
////    if(_processLauncher.state() != QProcess::UnknownError)
////        qDebug() << "Could not start VLC.";
//}

//    void MainWindow::fundWalletFromFaucet() {

//        //TODO - limit funding to once every hour? - check api limits

//    <<<<<<< HEAD
//            blockcypher::Address oldAddr =_wallet->BLOCKCYPHER_lastAdress();
//            updateWalletBalances(oldAddr._balance, oldAddr._unconfirmed_balance);
//    =======
//        qDebug() << "Topping up wallet from testnet faucet";
//    >>>>>>> development

//        FundingWalletProgressDialog dialog(RELOAD_WALLET_AMOUNT, &_bitcoinDisplaySettings);
//        dialog.show();

//        try {
//            _controller->fundWallet(RELOAD_WALLET_AMOUNT);
//        } catch(const std::exception & e) {
//            qDebug() << "Unable to fund wallet from faucet";
//        }

//        dialog.hide();
//    }


//    void MainWindow::on_updatePushButton_clicked() {

//        QMessageBox::StandardButton reply;

//        reply = QMessageBox::question(this, "Test", "Quit?", QMessageBox::Yes|QMessageBox::No);

//        if (reply == QMessageBox::Yes) {
//            qDebug() << "Yes was clicked";
//        } else {
//            qDebug() << "Yes was *not* clicked";
//        }

//        // Note:
//        // To close application:
//        // call slot (with signal or regular call) Controller::void Controller::begin_close.
//        // It will start an asyncrhonous shut down process (primarily due to libtorrent at the moment).
//        // When controller is done, it will emit a Controller::closed signal
//        // which has already been connected to the QApplication::quit event which
//        // stops the application event loop started in main.cpp
//        // with the QApplication::exec()

//    }

//    void MainWindow::on_topUpWalletPushButton_clicked() {

//        ReceiveFundsDialog dialog(_wallet);
//        dialog.exec();

//    }

//    void MainWindow::on_bugsPushButton_clicked() {
//        QDesktopServices::openUrl(QUrl("https://www.reddit.com/r/JoyStream/comments/3sazif/post_bugs_here/"));
//    }

//    void MainWindow::on_addMagnetLinkPushButton_clicked()
//    {
//        // Input magnet link
//        bool ok;
//        QString magnetLink = QInputDialog::getText(this,
//                                             tr("Add Magnet Link"),
//                                             tr("Magnet link:"),
//                                             QLineEdit::Normal,
//                                             //QString("magnet:?xt=urn:btih:781ad3adbd9b81b64e4c530712ae9199b1dfbae5&dn=Now+You+See+Me+%282013%29+1080p+EXTENDED+BrRip+x264+-+YIFY&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3"),
//                                             QString(""),
//                                             &ok);

//        // Exit if no link provided
//        if (!ok || magnetLink.isEmpty())
//            return;

//        ApplicationController::Torrent::Configuration config;

//        try{
//            // create configuration from torrent file
//            config = ApplicationController::Torrent::Configuration::fromMagnetLink(magnetLink);

//        } catch(std::runtime_error e){
//            //invalid magnet link
//            return;
//        }

//        // use standard download path
//        QString save_path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

//        // check path exists and is writable
//        if(!save_path.isNull() && QFile::exists(save_path) && QFileInfo(save_path).isWritable()){
//            config.setSavePath(save_path.toStdString());
//            _controller->addTorrent(config);
//        } else {
//            // Open dialog for adding torrent
//            showAddTorrentFromMagnetLinkDialog(config);
//        }
//    }

}
}
}
