#include "view/include/mainwindow.h"
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
    controller_(controller) {

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //QMessageBox::information(NULL, "Hello World!", "Hi!");

    // Choose torrent file
    QString fileNameQString = QFileDialog::getOpenFileName(this, tr("Open Torrent"), "/home/jana", tr("Torrent file (*.torrent)"));

    std::string fileNameString = fileNameQString.toStdString();

    std::cout << fileNameString << std::endl;

    /*
    // Add torrent file
    libtorrent::add_torrent_params params;
    controller_->submitAddTorrentViewRequest(params);
    */
}
