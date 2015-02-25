#ifndef SELLER_TORRENT_PLUGIN_CONFIGURATION_DIALOG_HPP
#define SELLER_TORRENT_PLUGIN_CONFIGURATION_DIALOG_HPP

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/torrent_info.hpp> // torrent_info
#include <QDialog>

class Controller;

namespace Ui {
class SellerTorrentPluginConfigurationDialog;
}

class SellerTorrentPluginConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    //explicit SellerTorrentPluginConfigurationDialog(QWidget *parent = 0);

    SellerTorrentPluginConfigurationDialog(Controller * controller, const libtorrent::torrent_info & torrentInfo);

    ~SellerTorrentPluginConfigurationDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SellerTorrentPluginConfigurationDialog *ui;

    Controller * _controller;

    // Hash of torrent
    //libtorrent::sha1_hash _infoHash;

    // Torrent file
    libtorrent::torrent_info _torrentInfo;
};

#endif // SELLER_TORRENT_PLUGIN_CONFIGURATION_DIALOG_HPP
