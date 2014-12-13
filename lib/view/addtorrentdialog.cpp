#include "view/addtorrentdialog.hpp"
#include "ui_addtorrentdialog.h"

#include <QFileDialog>

#include <libtorrent/add_torrent_params.hpp> // parsing torrent magnet/file

AddTorrentDialog::AddTorrentDialog(Controller * controller, const QString & resource, bool isTorrentFile, bool withPlugin)
    : ui(new Ui::AddTorrentDialog)
    , _controller(controller)
    , _resources(resources)
    , _isTorrentFile(isTorrentFile)
    , _withPlugin(withPlugin)
{
    // Setup ui using QtCreator routine
    ui->setupUi(this);

    // From torrent file
    if(_isTorrentFile) {

        // Setup parameters
        libtorrent::add_torrent_params params;

        // Load torrent file
        libtorrent::error_code ec;
        boost::intrusive_ptr<libtorrent::torrent_info> torrentInfoPointer = new libtorrent::torrent_info(torrentFile.toStdString().c_str(), ec);
        if(ec) {
            qCCritical(_category) << "Invalid torrent file: " << ec.message().c_str();
            return;
        }

        // Set torrent info in parameters
        params.ti = torrentInfoPointer;

    } else { // From magnet link

        // Setup parameters
        libtorrent::add_torrent_params params;
        params.url = magnetLink.toStdString();

        // Parse link to get info_hash, so that resume data can be loaded
        libtorrent::error_code ec;
        libtorrent::parse_magnet_uri(magnetLink.toStdString(), params, ec);

        // Exit if link is malformed
        if(ec) {
            qCWarning(_category) << "Malformed magnet link: " << ec.message().c_str();
            return;
        }


        /*
         *
         //* If info_hash is not set, we try and set it.
         //* This would typically be the case if torrent was added through torrent
         //* file rather than magnet link. The primary reason for this constraint is because searching
         //* addTorrentParameters is based on info_hashes,
         if(params.info_hash.is_all_zeros()) {

             // Is torrent info set, use it
             if(params.ti.get() != 0 && !params.ti->info_hash().is_all_zeros()) {
                 libtorrent::sha1_hash info_hash = params.ti->info_hash();
                 params.info_hash = info_hash;
             } else {
                 // Throw exception in future
                 qCDebug(_category) << "no valid info_hash set.";
                 return;
             }
         }
         */
    }
}

AddTorrentDialog::~AddTorrentDialog() {
    delete ui;
}

void AddTorrentDialog::on_AddTorrentDialog_accepted() {

    // save_path
    QString save_path = this->ui->saveToFolderLineEdit->text();

    // Create plugin
    TorrentPluginConfiguration * torrentPluginConfiguration = NULL;

    if(_withPlugin) {

        if()
            torrentPluginConfiguration = new SellerTorrentPluginConfiguration();
        else
            torrentPluginConfiguration = new BuyerTorrentPluginConfiguration();

        //if(newTorrent->bytes_left() > 0
   }

   // Create configuration for adding torrent
   TorrentConfiguration torrentConfiguration();

    // Add torrent
    _controller->addTorrent(torrentConfiguration);

    // Close window
    closeWindow();
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
