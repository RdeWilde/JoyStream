#ifndef PAYMENT_CHANNEL_VIEW_MODEL_HPP
#define PAYMENT_CHANNEL_VIEW_MODEL_HPP

#include <QMetaType> // Q_DECLARE_METATYPE

class PeerPlugin;
class QStandardItemModel;
class QStandardItem;
class PeerPluginStatus;
class QString;
enum class PeerPluginState;

class PaymentChannelViewModel
{
public:

    // Constructor
    PaymentChannelViewModel(PeerPlugin * peerPlugin, QStandardItemModel * paymentChannelsTableViewModel);

    // Destructor, is called from TorrentViewModel destructor
    ~PaymentChannelViewModel();

    void update(PeerPluginStatus status);
    void updateHost(const QString & host);
    void updateState(PeerPluginState state);
    void updateBalance(int balance);
    void updateProgress(); // some representation of what it has given me?

private:

    // View model for payment channels table
    QStandardItemModel * paymentChannelsTableViewModel_;

    // paymentChannelsTableViewModel_ items
    QStandardItem * hostItem,
                  * stateItem,
                  * balanceItem,
                  * progressItem;
};

Q_DECLARE_METATYPE(PaymentChannelViewModel *)

#endif // PAYMENT_CHANNEL_VIEW_MODEL_HPP
