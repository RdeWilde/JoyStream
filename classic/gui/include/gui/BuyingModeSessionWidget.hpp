/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 20 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_BUYINGMODESESSIONWIDGET_HPP
#define JOYSTREAM_CLASSIC_GUI_BUYINGMODESESSIONWIDGET_HPP

#include <libtorrent/socket.hpp>

#include <QWidget>
#include <QStandardItemModel>

#include <typeindex>

class QStandardItem;
class BitcoinDisplaySettings;

namespace Ui {
    class Form;
}

namespace Coin {
    class Transaction;
    class typesafeOutPoint;
}

namespace joystream {
namespace protocol_wire {
    class BuyerTerms;
}
namespace protocol_session {
    enum class BuyingState;
    class BuyingPolicy;
}
namespace protocol_statemachine {
    class AnnouncedModeAndTerms;
}
namespace core {
    class Connection;
    class Session;
}
namespace classic {
namespace gui {

class BuyingModeSessionWidget : public QWidget
{
    Q_OBJECT

public:

    class Connection;

    static BuyingModeSessionWidget * create(QWidget * parent,
                                            const BitcoinDisplaySettings * settings,
                                            const std::shared_ptr<core::Session> & session);

    explicit BuyingModeSessionWidget(QWidget * parent,
                                     const BitcoinDisplaySettings * settings);
    ~BuyingModeSessionWidget();

public slots:

    void setTerms(const protocol_wire::BuyerTerms & terms);
    void setPolicy(const protocol_session::BuyingPolicy & policy);
    void setState(const protocol_session::BuyingState & state);
    void setContractTx(const Coin::Transaction & tx);
    void addConnections(const std::weak_ptr<core::Connection> & connection);
    void removeConnections(const libtorrent::tcp::endpoint & endPoint);

    // add button for capturing clicked
    void updateTerms();

private:

    Ui::Form *ui;

    std::map<libtorrent::tcp::endpoint, Connection *> _connections;

    // Display settings for bitcoin
    const BitcoinDisplaySettings * _settings;

    // Torrent table view model
    QStandardItemModel _buyers;
};

class BuyingModeSessionWidget::Connection : public QObject {

    Q_OBJECT

public:

    static Connection * create(BuyingModeSessionWidget * parent,
                               const BitcoinDisplaySettings * settings,
                               QStandardItemModel * buyersStandardItemModel,
                               const std::shared_ptr<core::Connection> & c);

    Connection(BuyingModeSessionWidget * parent,
               const BitcoinDisplaySettings * settings,
               QStandardItem * hostItem,
               QStandardItem * stateItem,
               QStandardItem * fundsItem,
               QStandardItem * lockItem,
               QStandardItem * priceItem,
               QStandardItem * numberOfPayments,
               QStandardItem * balance);

    int row() const noexcept;

public slots:

    void setHost(const libtorrent::tcp::endpoint &);
    void setState(const std::type_index &);
    void setFunds(quint64);
    void setRefundLockTime(quint64);
    void setPrice(quint64);
    void setNumberOfPayments(quint64);
    void setBalance(qint64);
    void setAnnouncedModeAndTerms(const protocol_statemachine::AnnouncedModeAndTerms & announcedTerms);

signals:

    void hide(int row);

    void show(int row);

private:

    // Display settings for bitcoin
    const BitcoinDisplaySettings * _settings;

    // View model pointers
    // Objects are owned by QStandardItemModel, not us
    QStandardItem * _hostItem,
                  * _stateItem,
                  * _fundsItem,
                  * _lockItem,
                  * _priceItem,
                  * _numberOfPayments,
                  * _balance;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_BUYINGMODESESSIONWIDGET_HPP
