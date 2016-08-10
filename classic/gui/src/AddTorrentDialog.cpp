/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/AddTorrentDialog.hpp>
#include "ui_AddTorrentDialog.h"

#include <libtorrent/magnet_uri.hpp> // libtorrent::parse_magnet_uri
#include <libtorrent/add_torrent_params.hpp> // parsing torrent magnet/file

#include <QFileDialog>
#include <QDebug>
#include <QStandardPaths>

AddTorrentDialog::AddTorrentDialog(Controller * controller, const Controller::Torrent::Configuration & config)
    : ui(new Ui::AddTorrentDialog)
    , _controller(controller)
    , _configuration(config) {

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

    // Set default folder location
    ui->saveToFolderLineEdit->setText(QStandardPaths::standardLocations(QStandardPaths::DownloadLocation).first());
}

AddTorrentDialog::~AddTorrentDialog() {

    delete ui;
}

void AddTorrentDialog::clickedAcceptPushButton() {

    //Save Path
    QString save_path = this->ui->saveToFolderLineEdit->text();

    // Add torrent, user will later have to supply torrent plugin configuration
    if(!save_path.isNull() && QFile::exists(save_path) && QFileInfo(save_path).isWritable()){
        _configuration.setSavePath(save_path.toStdString());
        _controller->addTorrent(_configuration);
    }

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
