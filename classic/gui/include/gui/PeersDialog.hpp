/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 23 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_PEERSDIALOG_HPP
#define JOYSTREAM_CLASSIC_GUI_PEERSDIALOG_HPP

#include <libtorrent/socket_io.hpp>

#include <QDialog>
#include <QStandardItemModel>

#include <boost/variant.hpp>
#include <boost/optional.hpp> // <optional> available in c++17

namespace Ui {
class PeersDialog;
}

class BitcoinDisplaySettings;

namespace joystream {
namespace core {
    class Torrent;
    class Peer;
    class TorrentPlugin;
    class Connection;
    class PeerPlugin;
    class Buying;
    class Selling;
}
namespace protocol_wire {
    class BuyerTerms;
    class SellerTerms;
}
namespace protocol_statemachine {
    class AnnouncedModeAndTerms;
    enum class ModeAnnounced;
}
namespace extension {
    enum class BEPSupportStatus;
}
namespace classic {
namespace gui {

class NormalPeerRow : public QObject {

    Q_OBJECT

public:

    static NormalPeerRow * create(QStandardItemModel * model,
                                  const std::shared_ptr<core::Peer> & peer);

    NormalPeerRow(QStandardItem * host,
                  QStandardItem * clientName,
                  QStandardItem * BEPSupportStatus);

    void set(const std::shared_ptr<core::PeerPlugin> & peerPlugin);

public slots:

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    void setClientName(const std::string & clientName);

    void setBEPSupport(const extension::BEPSupportStatus & status);

public:

    int row() const noexcept;

private:

    QStandardItem * _hostItem,
                  * _clientNameItem,
                  * _BEPSupportStatusItem;
};

class BuyerConnectionRow : public QObject {

    Q_OBJECT

public:

    static BuyerConnectionRow * create(QStandardItemModel * model,
                                       const libtorrent::tcp::endpoint & endPoint,
                                       const protocol_wire::BuyerTerms & terms,
                                       const BitcoinDisplaySettings * settings);

    BuyerConnectionRow(QStandardItem * hostItem,
                       QStandardItem * maxPriceItem,
                       QStandardItem * maxLockItem,
                       QStandardItem * minNumberOfSellersItem,
                       QStandardItem * maxContractFeePerKbItem,
                       const BitcoinDisplaySettings * settings);

public slots:

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    void setTerms(const protocol_wire::BuyerTerms & terms);

public:

    int row() const noexcept;

private:

    QStandardItem * _hostItem,
                  * _maxPriceItem,
                  * _maxLockItem,
                  * _minNumberOfSellersItem,
                  * _maxContractFeePerKbItem;

    const BitcoinDisplaySettings * _settings;
};

class SellerConnectionRow : public QObject {

    Q_OBJECT

public:

    static SellerConnectionRow * create(QStandardItemModel * model,
                                        const libtorrent::tcp::endpoint & endPoint,
                                        const protocol_wire::SellerTerms & terms,
                                        const BitcoinDisplaySettings * settings);

    SellerConnectionRow(QStandardItem * hostItem,
                        QStandardItem * minPriceItem,
                        QStandardItem * minLockItem,
                        QStandardItem * maxSellersItem,
                        QStandardItem * minContractFeePerKbItem,
                        QStandardItem * settlementFeeItem,
                        const BitcoinDisplaySettings * settings);

public slots:

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    void setTerms(const protocol_wire::SellerTerms & terms);

public:

    int row() const noexcept;

private:

    QStandardItem * _hostItem,
                  * _minPriceItem,
                  * _minLockItem,
                  * _maxSellersItem,
                  * _minContractFeePerKbItem,
                  * _settlementFeeItem;

    const BitcoinDisplaySettings * _settings;
};

class ObserverConnectionRow : public QObject {

    Q_OBJECT

public:

    static ObserverConnectionRow * create(QStandardItemModel * model,
                                          const libtorrent::tcp::endpoint & endPoint,
                                          const BitcoinDisplaySettings * settings);

    ObserverConnectionRow(QStandardItem * hostItem,
                          const BitcoinDisplaySettings * settings);

public slots:

    void setHost(const libtorrent::tcp::endpoint & endPoint);

public:

    int row() const noexcept;

private:

    QStandardItem * _hostItem;

    const BitcoinDisplaySettings * _settings;

};

class PeerRows : public QObject {

public:

    typedef boost::variant<BuyerConnectionRow *,
                           SellerConnectionRow * ,
                           ObserverConnectionRow *> ConnectionRow;

    static PeerRows * create(QStandardItemModel * anyPeerTableModel,
                             QStandardItemModel * buyerConnectionTableModel,
                             QStandardItemModel * sellerConnectionTableModel,
                             QStandardItemModel * observerConnectionTableModel,
                             NormalPeerRow * normalPeerRow,
                             const libtorrent::tcp::endpoint & endPoint,
                             const BitcoinDisplaySettings * settings);

    // Takes ownership of normalPeerRow
    PeerRows(QStandardItemModel * anyPeerTableModel,
             QStandardItemModel * buyerConnectionTableModel,
             QStandardItemModel * sellerConnectionTableModel,
             QStandardItemModel * observerConnectionTableModel,
             NormalPeerRow * normalPeerRow,
             boost::optional<ConnectionRow> & connectionRow,
             const libtorrent::tcp::endpoint & endPoint,
             const BitcoinDisplaySettings * settings);

    ~PeerRows();

    void set(const std::shared_ptr<core::Connection> & connection);

    NormalPeerRow * normalPeerRow() const noexcept;

    void dropConnectionRow();

public slots:

    void setAnouncedModesAndterms(const protocol_statemachine::AnnouncedModeAndTerms & announed);

private:

    ConnectionRow createConnectionRow(const libtorrent::tcp::endpoint & endPoint,
                                      const protocol_statemachine::AnnouncedModeAndTerms & announed);



    QStandardItemModel * _anyPeerTableModel,
                       * _buyerConnectionTableModel,
                       * _sellerConnectionTableModel,
                       * _observerConnectionTableModel;

    NormalPeerRow * _normalPeerRow;

    // Substitute with std::optional when available
    boost::optional<ConnectionRow> _connectionRow;

    libtorrent::tcp::endpoint _endPoint;

    const BitcoinDisplaySettings * _settings;
};

class PeersDialog : public QDialog
{
    Q_OBJECT

public:

    explicit PeersDialog(QWidget * parent,
                         const BitcoinDisplaySettings * settings,
                         const std::shared_ptr<core::Torrent> torrent);

    ~PeersDialog();

public slots:

    void addPeer(const std::weak_ptr<core::Peer> & peer);
    void removePeer(const libtorrent::tcp::endpoint & endPoint);

    void addPeerPlugin(const std::weak_ptr<core::PeerPlugin> & peerPlugin);
    void removePeerPlugin(const libtorrent::tcp::endpoint & endPoint);

    void addConnection(const std::weak_ptr<core::Connection> & connection);
    void removeConnection(const libtorrent::tcp::endpoint & endPoint);

private:

    Ui::PeersDialog * ui;

    const BitcoinDisplaySettings * _settings;

    QStandardItemModel _anyPeerTableModel,
                       _buyerConnectionTableModel,
                       _sellerConnectionTableModel,
                       _observerConnectionTableModel;

    std::map<libtorrent::tcp::endpoint, PeerRows *> _endPointToPeerRows;

    std::weak_ptr<core::Torrent> _torrent;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_PEERSDIALOG_HPP
