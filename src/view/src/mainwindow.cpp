#include "view/include/mainwindow.h"
#include "view/include/addtorrentdialog.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QMessageBox>
#include <QFileDialog>
#include <QString>

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::MainWindow(Controller * controller) :
    ui(new Ui::MainWindow),
    controller_(controller)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // Choose torrent file
    QString torrentFileName = QFileDialog::getOpenFileName(this, tr("Open Torrent"), "/home/jana", tr("Torrent file (*.torrent)"));

    // Show window
    AddTorrentDialog * addTorrentDialog = new AddTorrentDialog(controller_, torrentFileName);
    addTorrentDialog->exec();
}
