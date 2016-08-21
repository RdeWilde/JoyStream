/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_TORRENTTABLEVIEWROW_HPP
#define JOYSTREAM_CLASSIC_GUI_TORRENTTABLEVIEWROW_HPP

#include <libtorrent/torrent_status.hpp>

#include <QObject>

#include <memory>

class QStandardItemModel;
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

class TorrentTableViewRow : public QObject {

    Q_OBJECT

public:

    static TorrentTableViewRow * create(QStandardItemModel * model,
                                       const std::shared_ptr<core::Torrent> & torrentModel,
                                       const BitcoinDisplaySettings * settings);

    TorrentTableViewRow(QObject * parent,
                       const BitcoinDisplaySettings * settings,
                       QStandardItem * nameItem,
                       QStandardItem * sizeItem,
                       QStandardItem * stateItem,
                       QStandardItem * uploadSpeed,
                       QStandardItem * downloadSpeed,
                       QStandardItem * numberOfBuyerPeersItem,
                       QStandardItem * numberOfSellerPeersitem,
                       QStandardItem * sessionModeItem,
                       QStandardItem * balanceItem);

public slots:

    /// Set field

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

    /// Plugin events

    void setTorrentPluginPresent(const std::weak_ptr<core::TorrentPlugin> &);
    void setTorrentPluginAbsent();

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

    // Updates the peer counts
    void updateConnectionCounts(const core::Session * session);

    // Utility routine for getting counts on number of different peer types
    static void numberOf(const core::Session * session,
                         unsigned int & numberOfBuyers,
                         unsigned int & numberOfSellers,
                         unsigned int & numberOfObservers,
                         unsigned int & numberOfUnAnnounced);

    // Updates the balance
    void updateBalance(const core::Session * session);

    // Utility routine for figuring out the balance in the corresponding mode
    static int64_t getBalanceInBuyingMode(const core::Session * session);
    static int64_t getBalanceInSellingMode(const core::Session * session);

};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_TORRENTTABLEVIEWROW_HPP

