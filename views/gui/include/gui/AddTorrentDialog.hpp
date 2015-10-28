/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef ADD_TORRENT_DIALOG_HPP
#define ADD_TORRENT_DIALOG_HPP

#include <QDialog>
#include <QString>

#include <core/controller/Controller.hpp>

namespace Ui {
class AddTorrentDialog;
}

/*
 * FACTOR OUT TO TWO DIALOGS LATER, SO
 * THAT STATE IS NOT SHARDED LIKE NOW, AND
 * MAGNET LINK CASE CAN FETCH METADATA AND
 * SHOW CONTENT OF TORRENT IN DELAYED SENSE.
 */
class AddTorrentDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructors
    AddTorrentDialog(Controller * controller, const Controller::Torrent::Configuration & config);

    // explicit AddTorrentDialog(QWidget *parent = 0);
    ~AddTorrentDialog();

private slots:

    void clickedAcceptPushButton();

    void clickedSaveToFolderPushButton();

private:

    // View
    Ui::AddTorrentDialog * ui;

    // Controller
    Controller * _controller;

    Controller::Torrent::Configuration _configuration;

};

#endif // ADD_TORRENT_DIALOG_HPP
