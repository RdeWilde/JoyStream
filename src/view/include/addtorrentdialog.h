#ifndef ADDTORRENTDIALOG_H
#define ADDTORRENTDIALOG_H

#include <QDialog>
#include <QString>

#include <libtorrent/add_torrent_params.hpp>
#include "controller/include/Controller.hpp"

namespace Ui {
class AddTorrentDialog;
}

class AddTorrentDialog : public QDialog
{
    Q_OBJECT

public:

    /*
    enum torrentType {
        torrent_file,
        magnet_link
    };
    */

    // explicit AddTorrentDialog(QWidget *parent = 0);
    ~AddTorrentDialog();

    // Constructors
    AddTorrentDialog(Controller * controller, libtorrent::add_torrent_params & params); //, torrentType type);

    void closeWindow();

private slots:
    void on_AddTorrentDialog_accepted();

    void on_AddTorrentDialog_rejected();

    void on_saveToFolderPushButton_clicked();

private:
    Ui::AddTorrentDialog * ui;

    Controller * controller_;

    //torrentType type_;

    libtorrent::add_torrent_params & params_;

};

#endif // ADDTORRENTDIALOG_H
