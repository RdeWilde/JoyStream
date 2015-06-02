#ifndef ADD_TORRENT_DIALOG_HPP
#define ADD_TORRENT_DIALOG_HPP

#include <QDialog>
#include <QString>

#include "controller/Controller.hpp"

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
    AddTorrentDialog(Controller * controller, const QString & resource, bool isTorrentFile);

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

    /**
     * INPUTS CONTROLLER VIEW
     */

    // Torrent resouce, i.e. magnet link or file location
    QString _resource;

    // Indicates whether adding was magnet link or file location
    bool _isTorrentFile;

    /**
     * USER SPECIFIED THROUGH UI
     */

    // Should plugin be installed
    bool _withPlugin;

    // Should banning sets be used
    bool _enableBanningSets;

    /**
     * PARSED STATE
     */

    // TORRENT FILE CASE ===================

    // Torrent information : is pointer since there is only one constructor, and in this constructor we need a test which
    // does not allow this field to be set in initialization list, this in combination with the fact that torrent_info has
    // no assignment operator forces us to save as pointer member.
    libtorrent::torrent_info * _torrentInfo;

    // MAGNET LINK CASE ===================

    // Magnet link url
    std::string _url;

    // parse_magnet_uri
    libtorrent::add_torrent_params _params;
};

#endif // ADD_TORRENT_DIALOG_HPP
