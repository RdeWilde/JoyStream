/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_SELLINGMODESESSIONWIDGET_HPP
#define JOYSTREAM_CLASSIC_GUI_SELLINGMODESESSIONWIDGET_HPP

#include <libtorrent/socket.hpp>

#include <QWidget>
#include <QStandardItemModel>

#include <typeindex>

class QStandardItem;
class BitcoinDisplaySettings;

namespace Ui {
    class SellingModeSessionWidget;
}

namespace Coin {
    class typesafeOutPoint;
}

namespace joystream {
namespace protocol_wire {
    class SellerTerms;
}
namespace protocol_session {
    class SellingPolicy;
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

class SellingModeSessionWidget : public QWidget
{
    Q_OBJECT

public:

    class Connection;

    static SellingModeSessionWidget * create(QWidget * parent,
                                             const BitcoinDisplaySettings * settings,
                                             const std::shared_ptr<core::Session> & session);

    explicit SellingModeSessionWidget(QWidget * parent,
                                      const BitcoinDisplaySettings * settings);

    ~SellingModeSessionWidget();

public slots:

    void setTerms(const protocol_wire::SellerTerms & terms);
    void setPolicy(const protocol_session::SellingPolicy & policy);
    void addConnections(const std::weak_ptr<core::Connection> & connection);
    void removeConnections(const libtorrent::tcp::endpoint & endPoint);

private:

    Ui::SellingModeSessionWidget *ui;

    std::map<libtorrent::tcp::endpoint, Connection *> _connections;

    // Display settings for bitcoin
    const BitcoinDisplaySettings * _settings;

    QStandardItemModel _sellers;

};

class SellingModeSessionWidget::Connection : public QObject {

    Q_OBJECT

public:

    static Connection * create(SellingModeSessionWidget * parent,
                               const BitcoinDisplaySettings * settings,
                               QStandardItemModel * buyersStandardItemModel,
                               const std::shared_ptr<core::Connection> & c);

    Connection(SellingModeSessionWidget * parent,
               const BitcoinDisplaySettings * settings,
               QStandardItem * hostItem,
               QStandardItem * stateItem,
               QStandardItem * anchorItem,
               QStandardItem * fundsItem,
               QStandardItem * lockItem,
               QStandardItem * priceItem,
               QStandardItem * numberOfPayments,
               QStandardItem * balance);

    int row() const noexcept;

public slots:

    void setHost(const libtorrent::tcp::endpoint &);
    void setState(const std::type_index &);
    void setAnchor(const Coin::typesafeOutPoint &);
    void setFunds(quint64);
    void setRefundLockTime(quint64);
    void setPrice(quint64);
    void setNumberOfPayments(quint64);
    void setBalance(qint64);
    void setAnnouncedModeAndTerms(const protocol_statemachine::AnnouncedModeAndTerms & annoucnedTerms);

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
                  * _anchorItem,
                  * _fundsItem,
                  * _lockItem,
                  * _priceItem,
                  * _numberOfPayments,
                  * _balance;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_SELLINGMODESESSIONWIDGET_HPP
