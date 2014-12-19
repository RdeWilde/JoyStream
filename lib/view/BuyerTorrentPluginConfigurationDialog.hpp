#ifndef BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP
#define BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP

#include <QDialog>

#include <libtorrent/peer_id.hpp> // sha1_hash

namespace Ui {
class BuyerTorrentPluginConfigurationDialog;
}

class Controller;

class BuyerTorrentPluginConfigurationDialog : public QDialog
{
    Q_OBJECT

public:

    //explicit BuyerTorrentPluginConfigurationDialog(QWidget *parent = 0);

    BuyerTorrentPluginConfigurationDialog(Controller * controller, const libtorrent::sha1_hash & infoHash);

    // Destructor
    ~BuyerTorrentPluginConfigurationDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::BuyerTorrentPluginConfigurationDialog *ui;

    Controller * _controller;

    libtorrent::sha1_hash _infoHash;
};

#endif // BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP
