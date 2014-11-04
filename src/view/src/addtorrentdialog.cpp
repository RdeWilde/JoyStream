#include "view/include/addtorrentdialog.h"
#include "ui_addtorrentdialog.h"

#include <QFileDialog>
#include <QDir>

#include <iostream>

#ifndef Q_MOC_RUN
#include <boost/intrusive_ptr.hpp>
#endif Q_MOC_RUN

//#include <libtorrent/entry.hpp>
//#include <libtorrent/bencode.hpp>
//#include <libtorrent/session.hpp>
#include <libtorrent/torrent_info.hpp>

AddTorrentDialog::AddTorrentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTorrentDialog)
{
    ui->setupUi(this);
}

AddTorrentDialog::AddTorrentDialog(Controller * controller, const QString & torrentFileName) :
    ui(new Ui::AddTorrentDialog),
    controller_(controller),
    torrentFileName_(torrentFileName)

{
    ui->setupUi(this);
}

AddTorrentDialog::~AddTorrentDialog()
{
    delete ui;
}

void AddTorrentDialog::on_AddTorrentDialog_accepted()
{
    // Add torrent
    addTorrent(torrentFileName_, ui->saveToFolderLineEdit->text());

    // Close window
    closeWindow();
}

void AddTorrentDialog::addTorrent(const QString & torrentFileName, const QString & downloadFolder) {

    std::string torrentFileNameString = torrentFileName.toStdString();

    // Load torrent file
    boost::intrusive_ptr<libtorrent::torrent_info> t;
    libtorrent::error_code ec;
    t = new libtorrent::torrent_info(torrentFileNameString.c_str(), ec);
    if(ec) {
        std::cerr << torrentFileNameString.c_str() << ": " << ec.message().c_str() << std::endl;
        return;
    }

    // Create directory for torrent if it does not exist
    QDir downloadFolderQDir(downloadFolder);
    QString torrentNameQString(t->name().c_str());

    if(!downloadFolderQDir.exists()) {
        std::cerr << "Directory " << downloadFolder.toStdString() << " does not exist." << std::endl;
        return;
    } else if(!downloadFolderQDir.exists(torrentNameQString)) {
        std::cout << "Creating directory: " << t->name() << std::endl;
        downloadFolderQDir.mkdir(torrentNameQString);
    }

    // Go into torrent folder
    QDir torrentFolder(downloadFolder);
    torrentFolder.cd(torrentNameQString);

    // Load resume data if it exists
    std::vector<char> * resume_data = NULL;
    if(torrentFolder.exists("resume.file")) {

        // Allocate space for resume data
        //resume_data = new std::vector<char>();

        //

    }

    // Create torrent parameters
    libtorrent::add_torrent_params params;

    params.ti = t;
    params.save_path = torrentFolder.path().toStdString();
    params.resume_data = resume_data;
    //params.storage_mode = (storage_mode_t)allocation_mode; //  disabled_storage_constructor;
    //params.flags |= add_torrent_params::flag_paused; //  |= add_torrent_params::flag_seed_mode;
    //params.flags &= ~add_torrent_params::flag_duplicate_is_error;
    //params.flags |= add_torrent_params::flag_auto_managed; // |= add_torrent_params::flag_share_mode;
    //params.userdata = (void*)strdup(torrent.c_str());

    // Ask controller to add torrent
    controller_->submitAddTorrentViewRequest(params);
}

void AddTorrentDialog::on_AddTorrentDialog_rejected()
{
    // Close window
    closeWindow();
}

void AddTorrentDialog::on_saveToFolderPushButton_clicked()
{
    // Show directory chooser
    QString path = QFileDialog::getExistingDirectory (this, tr("Directory"), "C:\\");

    // If a directory was chosen, then set the corresponding line edit
    if(path.isNull() == false)
        this->ui->saveToFolderLineEdit->setText(path);
}

void AddTorrentDialog::closeWindow() {

    // Delete window resources
    //delete this;
}
