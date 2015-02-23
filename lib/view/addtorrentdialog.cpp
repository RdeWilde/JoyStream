#include "view/AddTorrentDialog.hpp"
#include "ui_addtorrentdialog.h"
//#include "controller/TorrentConfiguration.hpp"

#include <QFileDialog>

#include <libtorrent/magnet_uri.hpp> // libtorrent::parse_magnet_uri
#include <libtorrent/add_torrent_params.hpp> // parsing torrent magnet/file

AddTorrentDialog::AddTorrentDialog(Controller * controller, QLoggingCategory & category, const QString & resource, bool isTorrentFile)
    : ui(new Ui::AddTorrentDialog)
    , _controller(controller)
    , _category(category)
    , _resource(resource)
    , _isTorrentFile(isTorrentFile)
    , _torrentInfo(NULL) {

    // Setup ui using QtCreator routine
    ui->setupUi(this);

    // Error code
    libtorrent::error_code ec;

    if(_isTorrentFile) { // From torrent file

        // Load torrent file
        _torrentInfo = new libtorrent::torrent_info(_resource.toStdString().c_str(), ec);

        // Was torrent file valid?
        if(ec) {
            qCCritical(_category) << "Invalid torrent file: " << ec.message().c_str();
            return;
        }

    } else { // From magnet link

        // Get magnet link
        _url = _resource.toStdString();

        // Parse link to get info_hash
        libtorrent::parse_magnet_uri(_url, _params, ec);

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
        delete _torrentInfo;
}

void AddTorrentDialog::on_AddTorrentDialog_accepted() {

    // Info hash
    libtorrent::sha1_hash info_hash;

    if(_isTorrentFile)
        info_hash = _torrentInfo->info_hash();
    else
        info_hash = _params.info_hash;

    // Name
    std::string name;

    if(_isTorrentFile)
        name = _torrentInfo->name();
    else
        name = _params.name;

    // Save path
    std::string save_path = this->ui->saveToFolderLineEdit->text().toStdString();

    // Resume data
    std::vector<char> resume_data;

    // Create configuration for adding torrent
    libtorrent::torrent_info * t;

    // make copy
    if(_torrentInfo != NULL)
        t = new libtorrent::torrent_info(*_torrentInfo);

    Controller::Torrent::Configuration configuration(info_hash,
                                                      name,
                                                      save_path,
                                                      resume_data,
                                                      libtorrent::add_torrent_params::flag_update_subscribe,
                                                      t,
                                                      NULL);

    // Add torrent, and make sure user later supplies torrent plugin configuration
    _controller->addTorrent(configuration, true);

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
