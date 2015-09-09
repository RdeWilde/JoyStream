/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/AddTorrentDialog.hpp>
#include "ui_AddTorrentDialog.h"

#include <QFileDialog>
#include <QDebug>

#include <libtorrent/magnet_uri.hpp> // libtorrent::parse_magnet_uri
#include <libtorrent/add_torrent_params.hpp> // parsing torrent magnet/file

AddTorrentDialog::AddTorrentDialog(Controller * controller, const QString & resource, bool isTorrentFile)
    : ui(new Ui::AddTorrentDialog)
    , _controller(controller)
    , _resource(resource)
    , _isTorrentFile(isTorrentFile)
    , _torrentInfo(NULL) {

    // Setup ui using QtCreator routine
    ui->setupUi(this);

    /**
     * Setup signals and slots
     */

    // Ok in button box should cause adding of torrent
    QObject::connect(ui->buttonBox,
                     SIGNAL(accepted()),
                     this,
                     SLOT(clickedAcceptPushButton()));

    // Handle save to folder button
    QObject::connect(ui->saveToFolderPushButton,
                     SIGNAL(clicked()),
                     this,
                     SLOT(clickedSaveToFolderPushButton()));

    // Handle cancel by shutting down dialog window
    QObject::connect(ui->buttonBox,
                     SIGNAL(rejected()),
                     this,
                     SLOT(reject()));

    // Error code
    libtorrent::error_code ec;

    if(_isTorrentFile) { // From torrent file

        // Load torrent file
        _torrentInfo = new libtorrent::torrent_info(_resource.toStdString().c_str(), ec);

        // Was torrent file valid?
        if(ec) {
            qDebug() << "Invalid torrent file: " << ec.message().c_str();
            return;
        }

    } else { // From magnet link

        // Get magnet link
        _url = _resource.toStdString();

        // Parse link to get info_hash
        libtorrent::parse_magnet_uri(_url, _params, ec);

        // Was magnet link malformed
        if(ec) {
            qDebug() << "Invalid magnet link: " << ec.message().c_str();
            return;
        }
    }
}

AddTorrentDialog::~AddTorrentDialog() {

    delete ui;

    // Delete torrent information we allocated in constructor
    if(_isTorrentFile)
        delete _torrentInfo;
}

void AddTorrentDialog::clickedAcceptPushButton() {

    // Info hash
    libtorrent::sha1_hash info_hash;

    if(_isTorrentFile)
        info_hash = _torrentInfo->info_hash();
    else
        info_hash = _params.info_hash;

    // Name
    std::string name;

    if(_isTorrentFile)
        name = _torrentInfo->name();
    else
        name = _params.name;

    // Save path
    std::string save_path = this->ui->saveToFolderLineEdit->text().toStdString();

    // Resume data
    std::vector<char> resume_data;

    // Create configuration for adding torrent
    libtorrent::torrent_info * t;

    // make copy
    if(_torrentInfo != NULL)
        t = new libtorrent::torrent_info(*_torrentInfo);

    Controller::Torrent::Configuration configuration(info_hash,
                                                      name,
                                                      save_path,
                                                      resume_data,
                                                      libtorrent::add_torrent_params::flag_update_subscribe,
                                                      t);

    // Add torrent, user will later have to supply torrent plugin configuration
    _controller->addTorrent(configuration);

    // Exit event loop
    close();
}

void AddTorrentDialog::clickedSaveToFolderPushButton() {

    // Show directory chooser
    QString path = QFileDialog::getExistingDirectory (this, tr("Directory"), "C:\\");

    // If a directory was chosen, then set the corresponding line edit
    if(path.isNull() == false)
        this->ui->saveToFolderLineEdit->setText(path);
}