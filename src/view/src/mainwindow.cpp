#include "view/include/mainwindow.h"
#include "view/include/addtorrentdialog.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QInputDialog>
#include <QAction>

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>

MainWindow::MainWindow(Controller * controller) :
    ui(new Ui::MainWindow),
    controller_(controller),
    model(new QStandardItemModel(0, 4, this)), //0 Rows and 4 Columns
    tableViewContextMenu(new QMenu(this))
{
    ui->setupUi(this);

    // Setup tableView
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Size")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Status")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Speed (Down|Up)")));
    ui->tableView->setModel(model);

    // Setup context menu capacity on table view
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)),
                             SLOT(customMenuRequested(QPoint)));

    // add menu buttons
    QAction * pauseAction = new QAction("Pause", this);
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(pauseMenuAction()));
    tableViewContextMenu->addAction(pauseAction);

    QAction * startAction = new QAction("Start", this);
    connect(startAction, SIGNAL(triggered()), this, SLOT(startMenuAction()));
    tableViewContextMenu->addAction(startAction);

    QAction * removeAction = new QAction("Remove", this);
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeMenuAction()));
    tableViewContextMenu->addAction(removeAction);
}

MainWindow::~MainWindow()
{
    delete ui;

    // Delete everything in model, should be recursive
    delete model;

    // Are Actions also deleted? hope so
    delete tableViewContextMenu;

    //rowMap.clear();
}

void MainWindow::customMenuRequested(QPoint pos) {

    // Figure out where user clicked
    lastIndexClicked = ui->tableView->indexAt(pos);

    // Show menu
    tableViewContextMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void MainWindow::pauseMenuAction() {

    std::cout << "pause" << std::endl;

    // Get row
    int row = lastIndexClicked.row();

    // Get info_hash
    libtorrent::sha1_hash & info_hash = infoHashInRow[row];

    // Get handle
    libtorrent::torrent_handle & torrentHandle = controller_->getTorrentHandleFromInfoHash(info_hash);

    // Pause
    torrentHandle.pause();
}

void MainWindow::startMenuAction() {
    std::cout << "start" << std::endl;
}

void MainWindow::removeMenuAction() {
    std::cout << "remove" << std::endl;
}


void MainWindow::on_addTorrentFilePushButton_clicked()
{
    // Choose torrent file
    QString torrentFile = QFileDialog::getOpenFileName(this, tr("Open Torrent"), "/home/jana", tr("Torrent file (*.torrent)"));

    // Exit if no file chosen
    if(torrentFile.isNull())
        return;

    // Tell controller
    controller_->addTorrentFromTorrentFile(torrentFile);
}

void MainWindow::on_addMagnetLinkPushButton_clicked()
{
    // Input magnet link
    bool ok;
    QString magnetLink = QInputDialog::getText(this,
                                         tr("Add Magnet Link"),
                                         tr("Magnet link:"),
                                         QLineEdit::Normal,
                                         QString("magnet:?xt=urn:btih:781ad3adbd9b81b64e4c530712ae9199b1dfbae5&dn=Now+You+See+Me+%282013%29+1080p+EXTENDED+BrRip+x264+-+YIFY&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3"),
                                         &ok);

    // Exit if no link provided
    if (!ok || magnetLink.isEmpty())
        return;

    // Notify controller
    controller_->addTorrentFromMagnetLink(magnetLink);
}

void MainWindow::on_closePushButton_clicked() {

}

void MainWindow::addTorrent(const libtorrent::sha1_hash & info_hash, const std::string & torrentName, int totalSize) {

    /*
     * change model to avoid the whole hash map mess
     */

    // Create row for model
    QList<QStandardItem *> modelRow;

    QString name(torrentName.c_str());
    //QStandardItem * nameQStandardItem = new QStandardItem(name);
    //modelRow.append(nameQStandardItem);
    modelRow.append(new QStandardItem(name));

    QString size = QString::number(totalSize);
    modelRow.append(new QStandardItem(size));

    QString status("Starting");
    modelRow.append(new QStandardItem(status));

    QString speed("");
    modelRow.append(new QStandardItem(speed));

    // Add to name model item to to make it recoverable from info_hash
    //rowMap.insert(std::make_pair(info_hash, nameQStandardItem));
    infoHashInRow.push_back(info_hash);

    // Add row to model
    model->appendRow(modelRow);
}

void MainWindow::updateTorrentStatus(const std::vector<libtorrent::torrent_status> & torrentStatusVector) {

    // Iterate status vector, process each
    for(std::vector<libtorrent::torrent_status>::const_iterator i = torrentStatusVector.begin(),
        end(torrentStatusVector.end()); i != end;i++)
        updateTorrentStatus(*i);
}

void MainWindow::updateTorrentStatus(const libtorrent::torrent_status & torrentStatus) {

    /*
    // Find model item corresponding to torrent
    QStandardItem * firstItem = rowMap.at(torrentStatus.info_hash);

    // Find row of model item
    QModelIndex index = model->indexFromItem(firstItem);
    int row = index.row();
    */

    // Find row where torrent is located
    int row;
    std::vector<libtorrent::sha1_hash>::iterator i = std::find(infoHashInRow.begin(),
                                                               infoHashInRow.end(),
                                                               torrentStatus.info_hash);
    // Check if we hit end, could be due to no match
    if(i != infoHashInRow.end())
        row = std::distance(infoHashInRow.begin(), i);
    else if (*i == infoHashInRow.back())
        row = infoHashInRow.size() - 1;
    else {
        std::cout << "no match found" << std::endl;
        return;
    }

    // Change row in model row, by updating: status (2) and speed (3)
    QString status;
    status.append(QString::number(torrentStatus.progress*100,'f',0)).append("%");
    model->item(row,2)->setText(status);

    QString speed;
    speed.append(QString::number(torrentStatus.download_rate/1024))
         .append("Kb/s | ")
         .append(QString::number(torrentStatus.upload_rate/1024))
         .append("Kb/s");
    model->item(row, 3)->setText(speed);
}

void MainWindow::removeTorrent(const libtorrent::sha1_hash & info_hash) {

}

void MainWindow::addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec) {

    // Do something more clever, this is just a hacky fix for now
    std::cerr << "Failed to add torrent " << name.c_str() << " because: " << ec.message().c_str() << std::endl;
}
