#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
#include <QPoint>
#include <QMenu>

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

    MainWindow(Controller * controller);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

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

    // These slots are used to tap into native QMenu ui signals.
    void customMenuRequested(QPoint pos);
    void pauseMenuAction();
    void startMenuAction();
    void removeMenuAction();

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
    //std::map<libtorrent::sha1_hash, QStandardItem *> rowMap;
    std::vector<libtorrent::sha1_hash> infoHashInRow;

    // Context menu on torrents
    QMenu * tableViewContextMenu;
    QModelIndex lastIndexClicked;

    int findRowFromInfoHash(const libtorrent::sha1_hash & info_hash);
    libtorrent::torrent_handle getTorrentHandleLastClicked();
};

#endif // MAINWINDOW_H
