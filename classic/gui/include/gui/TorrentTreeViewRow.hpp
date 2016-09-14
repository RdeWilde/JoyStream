/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_TORRENTTREEVIEWROW_HPP
#define JOYSTREAM_CLASSIC_GUI_TORRENTTREEVIEWROW_HPP

#include <libtorrent/torrent_status.hpp>

#include <QtGlobal>

class QObject;
class QStandardItem;
class BitcoinDisplaySettings;

namespace joystream {
namespace protocol_session {
    enum class SessionMode;
}
namespace core {
    class Torrent;
    class Session;
    class TorrentPlugin;
}
namespace classic {
namespace gui {

class TorrentTreeViewRow {

public:

    /**
     * @brief TorrentTableViewRow constructor
     * @note This object does not take ownership of the items provided
     */
    TorrentTreeViewRow(const BitcoinDisplaySettings * settings,
                       QStandardItem * nameItem,
                       QStandardItem * sizeItem,
                       QStandardItem * stateItem,
                       QStandardItem * uploadSpeed,
                       QStandardItem * downloadSpeed,
                       QStandardItem * numberOfBuyerPeersItem,
                       QStandardItem * numberOfSellerPeersitem,
                       QStandardItem * sessionModeItem,
                       QStandardItem * balanceItem);

    TorrentTreeViewRow * create(const BitcoinDisplaySettings * settings);

    QStandardItem * nameItem() const noexcept;
    QStandardItem * sizeItem() const noexcept;
    QStandardItem * stateItem() const noexcept;
    QStandardItem * uploadSpeedItem() const noexcept;
    QStandardItem * downloadSpeedItem() const noexcept;
    QStandardItem * numberOfBuyerPeersItem() const noexcept;
    QStandardItem * numberOfSellerPeersitem() const noexcept;
    QStandardItem * sessionModeItem() const noexcept;
    QStandardItem * balanceItem() const noexcept;

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

private:

    // Display settings for bitcoin
    const BitcoinDisplaySettings * _settings;

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
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_TORRENTTREEVIEWROW_HPP

