#include "view/include/addtorrentdialog.h"
#include "ui_addtorrentdialog.h"

#include <QFileDialog>

AddTorrentDialog::AddTorrentDialog(Controller * controller, libtorrent::add_torrent_params & params) : //, AddTorrentDialog::torrentType type) :
    ui(new Ui::AddTorrentDialog),
    controller_(controller),
    params_(params) {
    ui->setupUi(this);
}

AddTorrentDialog::~AddTorrentDialog() {
    delete ui;
}

void AddTorrentDialog::on_AddTorrentDialog_accepted() {

    // save_path
    QString save_path = this->ui->saveToFolderLineEdit->text();

    /*
    // Append directory seperator if not there
    QChar s = QDir::separator();
    if(!save_path.endsWith(s))
        save_path.append(s);

    // Extend with torrent name if present, checks torrent_info
    // which is set when using torrent files, and checks .name
    // which is set when using magnet links.
    std::string torrentName;
    if(params_.ti.get() != 0) // check if pointer has been set
        torrentName = params_.ti->name();
    else
        torrentName = params_.name;

    if(torrentName.empty())
        std::cerr << "No torrent name found." << std::endl;

    // Append torrent whatever torrent name found
    save_path.append(torrentName.c_str());
    */

    // Update parameters
    params_.save_path = save_path.toStdString();

    // Add torrent
    controller_->addTorrent(params_);

    // Close window
    closeWindow();

    //std::cout << "done on_AddTorrentDialog_accepted()" << std::endl;
}

void AddTorrentDialog::on_AddTorrentDialog_rejected() {
    // Close window
    closeWindow();
}

void AddTorrentDialog::on_saveToFolderPushButton_clicked() {
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
