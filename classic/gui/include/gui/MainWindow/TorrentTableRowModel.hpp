/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_TORRENTTABLEROWMODEL_HPP
#define JOYSTREAM_CLASSIC_GUI_TORRENTTABLEROWMODEL_HPP

#include <libtorrent/torrent_status.hpp>

#include <QObject>

class QStandardItem;
class BitcoinDisplaySettings;

namespace joystream {
namespace protocol_session {
    enum class SessionMode;
}
namespace classic {
namespace gui {

class TorrentTableRowModel : public QObject {

    Q_OBJECT

public:

    /**
     * @brief TorrentTableViewRow constructor
     * @note This object does not take ownership of the items provided
     */
    TorrentTableRowModel(QObject * parent,
                         QStandardItem * nameItem,
                         QStandardItem * sizeItem,
                         QStandardItem * stateItem,
                         QStandardItem * uploadSpeed,
                         QStandardItem * downloadSpeed,
                         QStandardItem * numberOfBuyerPeersItem,
                         QStandardItem * numberOfSellerPeersitem,
                         QStandardItem * sessionModeItem,
                         QStandardItem * balanceItem,
                         const BitcoinDisplaySettings * settings);

public slots:

    void setName(const std::string & name);
    void setSize(qint64 totalSize);
    void setState(libtorrent::torrent_status::state_t state, float progress);
    void setPaused(bool);
    void setUploadSpeed(int speed);
    void setDownloadSpeed(int speed);
    void setNumberOfBuyers(quint32 num);
    void setNumberOfSellers(quint32 num);
    void setSessionMode(protocol_session::SessionMode mode);
    void setBalance(qint64 balance);

    void unsetTorrentPluginPresence();

public:

    int row() const noexcept;

private:

    // View model pointers
    // Objects are owned by QStandardItemModel, not us
    QStandardItem * _nameItem,
                  * _sizeItem,
                  * _stateItem,
                  * _uploadSpeedItem,
                  * _downloadSpeedItem,
                  * _numberOfBuyerPeersItem,
                  * _numberOfSellerPeersitem,
                  * _sessionModeItem,
                  * _balanceItem;

    const BitcoinDisplaySettings * _settings;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_TORRENTTABLEROWMODEL_HPP

