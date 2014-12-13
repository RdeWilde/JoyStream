#ifndef ADD_TORRENT_DIALOG_HPP
#define ADD_TORRENT_DIALOG_HPP

#include <QDialog>
#include <QString>

#include "controller/Controller.hpp"

namespace Ui {
class AddTorrentDialog;
}

class AddTorrentDialog : public QDialog
{
    Q_OBJECT

public:

    // explicit AddTorrentDialog(QWidget *parent = 0);
    ~AddTorrentDialog();

    // Constructors
    AddTorrentDialog(Controller * controller, const QString & resource, bool isTorrentFile, bool withPlugin);

private slots:
    void on_AddTorrentDialog_accepted();

    void on_AddTorrentDialog_rejected();

    void on_saveToFolderPushButton_clicked();

private:
    Ui::AddTorrentDialog * ui;

    Controller * _controller;

    // Torrent resouce, i.e. magnet link or file location
    QString _resource;

    // Indicates whether adding was magnet link or file location
    bool _isTorrentFile;

    // Should plugin be installed
    bool _withPlugin;
};

#endif // ADD_TORRENT_DIALOG_HPP
