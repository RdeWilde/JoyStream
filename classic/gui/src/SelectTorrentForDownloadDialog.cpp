#include <gui/SelectTorrentForDownloadDialog.hpp>
#include "ui_SelectTorrentForDownloadDialog.h"

namespace joystream {
namespace classic {
namespace gui {

SelectTorrentForDownloadDialog::SelectTorrentForDownloadDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectTorrentForDownloadDialog)
    , _selected(Selected::None)
{
    ui->setupUi(this);
}

SelectTorrentForDownloadDialog::~SelectTorrentForDownloadDialog()
{
    delete ui;
}

SelectTorrentForDownloadDialog::Selected SelectTorrentForDownloadDialog::selected() const noexcept {
    return _selected;
}

void joystream::classic::gui::SelectTorrentForDownloadDialog::on_TorrentFilePushButton_clicked() {
    _selected = Selected::TorrentFile;
    done(1);
}

void joystream::classic::gui::SelectTorrentForDownloadDialog::on_MagnetLinkPushButton_clicked()
{
    _selected = Selected::MagnetLink;
    done(1);
}

}
}
}

