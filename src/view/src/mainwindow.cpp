#include "view/include/mainwindow.h"
#include "view/include/addtorrentdialog.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QInputDialog>
#include <QAction>
#include <QCloseEvent>

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

    // Are actions also deleted? hope so
    delete tableViewContextMenu;

    //rowMap.clear();
}

void MainWindow::customMenuRequested(QPoint pos) {

    // Figure out where user clicked
    lastIndexClicked = ui->tableView->indexAt(pos);

    // Check that the clicked item is valid, e.g. if view is empty
    if(!lastIndexClicked.isValid())
        return;

    // Show menu
    tableViewContextMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void MainWindow::pauseMenuAction() {

    // Get handle
    libtorrent::torrent_handle torrentHandle = getTorrentHandleLastClicked();

    // Is it valid, i.e. was an actual match found?
    if(!torrentHandle.is_valid())
        std::cout << "no match found" << std::endl;
    else {

        // Turn off auto managing
        torrentHandle.auto_managed(false);

        // Pause
        torrentHandle.pause(libtorrent::torrent_handle::graceful_pause);
    }
}

void MainWindow::startMenuAction() {

    // Get handle
    libtorrent::torrent_handle torrentHandle = getTorrentHandleLastClicked();

    // Is it valid, i.e. was an actual match found?
    if(!torrentHandle.is_valid())
        std::cout << "no match found" << std::endl;
    else {

        // Turn on auto managing
        torrentHandle.auto_managed(true);

        // Start
        torrentHandle.resume();
    }
}

void MainWindow::removeMenuAction() {

    // Remove from info hash container
    libtorrent::torrent_handle torrentHandle = getTorrentHandleLastClicked();

    // Notify controller to remove torrent
    controller_->removeTorrent(torrentHandle);
}

libtorrent::torrent_handle MainWindow::getTorrentHandleLastClicked() {

    // Get row
    int row = lastIndexClicked.row();

    // Get info_hash
    libtorrent::sha1_hash & info_hash = infoHashInRow[row];

    // Get handle
    return controller_->getTorrentHandleFromInfoHash(info_hash);    // libtorrent::torrent_handle torrentHandle =
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

    // Notify controller
    controller_->close();
}

void MainWindow::closeEvent(QCloseEvent * event) {

    // Notify controller
    controller_->close();
}

void MainWindow::addTorrent(const libtorrent::sha1_hash & info_hash, const std::string & torrentName, int totalSize) {

    // Create row for model
    QList<QStandardItem *> modelRow;

    QString name(torrentName.c_str());
    modelRow.append(new QStandardItem(name));

    QString size = QString::number(totalSize);
    modelRow.append(new QStandardItem(size));

    QString status("Starting");
    modelRow.append(new QStandardItem(status));

    QString speed("");
    modelRow.append(new QStandardItem(speed));

    // Remember info has order
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

    // Find row where torrent is located
    int row = findRowFromInfoHash(torrentStatus.info_hash);

    if(row < 0) {
        std::cout << "no match info_hash found." << std::endl;
        return;
    }// else
     //   std::cout << "update for row: " << row << std::endl;

    // Change row in model row, by updating:
    // size (1) <-- if not set by info_hash based torrent
    // status (2)
    // speed (3)

    // Size, set it if information is present
    if(torrentStatus.torrent_file.get() != 0) {

        QString size = QString::number(torrentStatus.torrent_file->total_size());

        model->item(row,1)->setText(size);
    }

    // Status
    QString status;

    if(torrentStatus.paused)
        status = "Paused";
    else {

        // .progress reports the pogress of the relevant task, but I suspect it is being used
        // some what incorrectly in som eof the cases below.

        switch(torrentStatus.state) {

            case libtorrent::torrent_status::queued_for_checking:

                status = "Queued for checking";

                break;
            case libtorrent::torrent_status::checking_files:

                status = "Checking files";
                status += " " + QString::number(torrentStatus.progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::downloading_metadata:

                status = "Downloading metadata";
                status += " " + QString::number(torrentStatus.progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::downloading:

                status = "Downloading";
                status += " " + QString::number(torrentStatus.progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::finished:

                status = "Finished";
                break;
            case libtorrent::torrent_status::seeding:

                status = "Seeding";
                break;
            case libtorrent::torrent_status::allocating:

                status = "Allocating";
                status += " " + QString::number(torrentStatus.progress*100,'f',0).append("%");
                break;
            case libtorrent::torrent_status::checking_resume_data:

                status = "Checking resume data";
                status += " " + QString::number(torrentStatus.progress*100,'f',0).append("%");
                break;
        }

    }

    model->item(row,2)->setText(status);

    // Speed
    QString speed;

    speed.append(QString::number(torrentStatus.download_rate/1024))
         .append("Kb/s | ")
         .append(QString::number(torrentStatus.upload_rate/1024))
         .append("Kb/s");

    model->item(row, 3)->setText(speed);
}

void MainWindow::removeTorrent(const libtorrent::sha1_hash & info_hash) {

    // Try to get row
    int row = findRowFromInfoHash(info_hash);

    // Check that it is valid

    // Remove row from model
    model->removeRows(row, 1);

    // Remove from info hash container
    infoHashInRow.erase(std::remove(infoHashInRow.begin(),
                                    infoHashInRow.end(),
                                    info_hash),
                        infoHashInRow.end());
}

void MainWindow::addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec) {

    // Do something more clever, this is just a hacky fix for now
    std::cerr << "Failed to add torrent " << name.c_str() << " because: " << ec.message().c_str() << std::endl;
}

int MainWindow::findRowFromInfoHash(const libtorrent::sha1_hash & info_hash) {

    // Find torrent matching info_hash
    std::vector<libtorrent::sha1_hash>::iterator it = std::find(infoHashInRow.begin(),
                                                               infoHashInRow.end(),
                                                               info_hash);
    // Check if we hit end, could be due to no match
    if(it != infoHashInRow.end())
        return std::distance(infoHashInRow.begin(), it);
    else if (*it == infoHashInRow.back())
        return infoHashInRow.size() - 1;
    else
        return -1;
}
