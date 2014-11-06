#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/torrent_handle.hpp>

namespace Ui {
class MainWindow;
}

// Forward declarations
class Controller;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //explicit MainWindow(QWidget *parent = 0);

    MainWindow(Controller * controller);

    ~MainWindow();

public slots:
    /*
     * These slots are connected to signals on the controller object,
     * who's constructor does the connecting.
     */

    void addTorrent(const libtorrent::sha1_hash & info_hash, const std::string & torrentName, int totalSize);
    void addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);
    void updateTorrentStatus(const std::vector<libtorrent::torrent_status> & torrentStatusVector);
    void updateTorrentStatus(const libtorrent::torrent_status & torrentStatus); // start, stopp, stats
    void removeTorrent(const libtorrent::sha1_hash & info_hash);


private slots:

    void on_addTorrentFilePushButton_clicked();
    void on_addMagnetLinkPushButton_clicked();
    void on_closePushButton_clicked();

private:
    Ui::MainWindow * ui;

    Controller * controller_;

    QStandardItemModel * model;

    /*
    * Maps torrent info_hash to the model item containing the name of the torrent.
    * This makes all model items associated with a given torrent recoverable
    * based on its info_hash. Using only name directly would have been unwise,
    * as two torrents with the same name, but different info_hash, could potentially
    * be added and then cause ambiguity.
    */
    std::map<libtorrent::sha1_hash, QStandardItem *> rowMap;
};

#endif // MAINWINDOW_H
