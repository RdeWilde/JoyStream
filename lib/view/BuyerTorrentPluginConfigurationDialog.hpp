#ifndef BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP
#define BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP

#include <QDialog>

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/torrent_info.hpp>

namespace Ui {
class BuyerTorrentPluginConfigurationDialog;
}

class Controller;
class Wallet;

class BuyerTorrentPluginConfigurationDialog : public QDialog
{
    Q_OBJECT

public:

    //explicit BuyerTorrentPluginConfigurationDialog(QWidget *parent = 0);

    BuyerTorrentPluginConfigurationDialog(Controller * controller, Wallet * wallet, const libtorrent::torrent_info & torrentInfo);

    // Destructor
    ~BuyerTorrentPluginConfigurationDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_maxPriceLineEdit_textChanged(const QString &arg1);

private:
    Ui::BuyerTorrentPluginConfigurationDialog *ui;

    Controller * _controller;

    Wallet * _wallet;

    //libtorrent::sha1_hash _infoHash;

    libtorrent::torrent_info _torrentInfo;
};

#endif // BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP
