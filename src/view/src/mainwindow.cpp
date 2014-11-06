#include "view/include/mainwindow.h"
#include "view/include/addtorrentdialog.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QInputDialog>

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>

MainWindow::MainWindow(Controller * controller) :
    ui(new Ui::MainWindow),
    controller_(controller),
    model(new QStandardItemModel(0, 4, this)) //0 Rows and 4 Columns
{
    ui->setupUi(this);

    // Setup tableView
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Size")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Status")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Speed")));
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;

    // Delete everything in model
    delete model;
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

    // Tell controller
    controller_->addTorrentFromMagnetLink(magnetLink);
}

void MainWindow::on_closePushButton_clicked()
{

}

void MainWindow::addTorrent(const libtorrent::sha1_hash & info_hash, const std::string & torrentName, int totalSize) {

    /*
     * change model to avoid the whole hash map mess
     */

    // Create row for model
    QList<QStandardItem *> modelRow;

    QString name(torrentName.c_str());
    QStandardItem * nameQStandardItem = new QStandardItem(name);
    modelRow.append(nameQStandardItem);

    QString size = QString::number(totalSize);
    modelRow.append(new QStandardItem(size));

    QString status("Starting");
    modelRow.append(new QStandardItem(status));

    QString speed("");
    modelRow.append(new QStandardItem(speed));

    // Add to name model item to to make it recoverable from info_hash
    rowMap.insert(std::make_pair(info_hash, nameQStandardItem));

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

    // Find model item corresponding to torrent
    QStandardItem * firstItem = rowMap.at(torrentStatus.info_hash);

    // Find row and column of model item
    QModelIndex index = model->indexFromItem(firstItem);
    int row = index.row();

    // Change row in model row, by updating: status (2) and speed (3)
    QString status;
    std::cout << torrentStatus.progress*100 << std::endl;
    status.append(QString::number(torrentStatus.progress*100)).append("%");
    model->item(row,2)->setText(status);

    QString speed;
    speed.append(QString::number(torrentStatus.download_rate))
            .append(QDir::separator())
            .append(QString::number(torrentStatus.upload_rate));
    model->item(row, 3)->setText(speed);
}

void MainWindow::removeTorrent(const libtorrent::sha1_hash & info_hash) {

}

void MainWindow::addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec) {

    // Do something more clever, this is just a hacky fix for now
    std::cerr << "Failed to add torrent " << name.c_str() << " because: " << ec.message().c_str() << std::endl;
}
