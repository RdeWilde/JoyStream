#include "view/AddTorrentDialog.hpp"
#include "ui_addtorrentdialog.h"

#include <QFileDialog>

#include <libtorrent/magnet_uri.hpp> // libtorrent::parse_magnet_uri
#include <libtorrent/add_torrent_params.hpp> // parsing torrent magnet/file

AddTorrentDialog::AddTorrentDialog(Controller * controller, QLoggingCategory & category, const QString & resource, bool isTorrentFile)
    : ui(new Ui::AddTorrentDialog)
    , _controller(controller)
    , _category(category)
    , _resource(resource)
    , _isTorrentFile(isTorrentFile) {

    // Setup ui using QtCreator routine
    ui->setupUi(this);

    // Error code
    libtorrent::error_code ec;

    if(_isTorrentFile) { // From torrent file

        // Load torrent file
        _torrent_info = new libtorrent::torrent_info(_resource.toStdString().c_str(), ec);

        // Was torrent file valid?
        if(ec) {
            qCCritical(_category) << "Invalid torrent file: " << ec.message().c_str();
            return;
        }

    } else { // From magnet link

        // Get magnet link
        url = _resource.toStdString();

        // Parse link to get info_hash
        libtorrent::parse_magnet_uri(url, _params, ec);

        // Was magnet link malformed
        if(ec) {
            qCWarning(_category) << "Invalid magnet link: " << ec.message().c_str();
            return;
        }
    }
}

AddTorrentDialog::~AddTorrentDialog() {

    delete ui;

    // Delete torrent information we allocated in constructor
    if(_isTorrentFile)
        delete _torrent_info;
}

void AddTorrentDialog::on_AddTorrentDialog_accepted() {

    // Info hash
    libtorrent::sha1_hash info_hash;

    if(_isTorrentFile)
        info_hash = _torrent_info->info_hash();
    else
        info_hash = _params.info_hash;

    // Name
    std::string name;

    if(_isTorrentFile)
        name = _torrent_info->name();
    else
        name = _params.name;

    // Save path
    std::string save_path = this->ui->saveToFolderLineEdit->text().toStdString();

    // Resume data
    std::vector<char> resume_data;

    // Torrent plugin configuration
    TorrentPluginConfiguration torrentPluginConfiguration(PluginMode::NotDetermined, _enableBanningSets, _withPlugin);

    // Create configuration for adding torrent
    TorrentConfiguration torrentConfiguration(info_hash, name, save_path, resume_data, 0, *_torrent_info, torrentPluginConfiguration);

    // Add torrent
    _controller->addTorrent(torrentConfiguration);

    // Close window
    done(0);
}

void AddTorrentDialog::on_AddTorrentDialog_rejected() {

    // Close window
    done(0);
}

void AddTorrentDialog::on_saveToFolderPushButton_clicked() {

    // Show directory chooser
    QString path = QFileDialog::getExistingDirectory (this, tr("Directory"), "C:\\");

    // If a directory was chosen, then set the corresponding line edit
    if(path.isNull() == false)
        this->ui->saveToFolderLineEdit->setText(path);
}
