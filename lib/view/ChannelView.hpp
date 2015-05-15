#ifndef CHANNEL_VIEW_HPP
#define CHANNEL_VIEW_HPP

#include "extension/PaymentChannel/Payor.hpp"

#include <QObject>
#include <QMenu>
#include <QAction>

class QStandardItem;
class QStandardItemModel;
class ChannelViewModel;

class ChannelView : public QObject
{
    Q_OBJECT
public:

    // Constructor
    ChannelView(QObject * parent,
                const ChannelViewModel * channelViewModel,
                QStandardItemModel * model);

    // Text conversion routines
    static QString stateToString(Payor::Channel::State state);
    static QString fundsToString(quint64 funds);
    static QString refundLockTimeToString(quint32 refundLockTime);
    static QString priceToString(quint64 price);
    static QString numberOfPaymentsMadeToString(quint64 numberOfPaymentsMade);
    static QString balanceToString(quint64 balance);

public slots:

    // Update channel status fields
    void updateState(Payor::Channel::State state);
    void updateFunds(quint64 funds);
    void updateRefundLockTime(quint32 refundLockTime);
    void updatePrice(quint64 price);
    void updateNumberOfPaymentsMade(quint64 numberOfPaymentsMade);
    void updateBalance(quint64 balance);

signals:

private:

    // Index of channel in payor
    // Is required to send signals based on user interaction
    quint32 _index;

    // View model pointers
    // Objects are owned by QStandardItemModel passed to ctr
    QStandardItem * _stateItem,
                  * _fundsItem,
                  * _refundLockTimeItem,
                  * _priceItem,
                  * _numberOfPaymentMadeItem,
                  * _balanceItem;

    // Context menu
    //QMenu _torrentTableContextMenu;

    // Context menu actions
    //QAction _pause;
};

#endif // CHANNEL_VIEW_HPP
