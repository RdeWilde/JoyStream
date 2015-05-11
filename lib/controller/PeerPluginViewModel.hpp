#ifndef PEER_PLUGIN_VIEW_MODEL_HPP
#define PEER_PLUGIN_VIEW_MODEL_HPP

#include <libtorrent/socket.hpp> // libtorrent::tcp::endpoint

class PeerPlugin;
class QStandardItemModel;
class QStandardItem;
class PeerPluginStatus;
class QString;
//enum class PeerState;


/**
 * This class is likely not productive, remove?
 */

class PeerPluginViewModel {

public:

    // Default constructor
    PeerPluginViewModel();

    // Constructor from members
    PeerPluginViewModel(const libtorrent::tcp::endpoint & endPoint);

    // Update routines
    //void update(PeerPluginStatus status);
    void updateHost(const QString & host);
    //void updateState(PeerState);
    void updateBalance(int balance);
    void updateProgress();

    // Getters
    libtorrent::tcp::endpoint endPoint();

private:

    // TCP/IP endpoint
    libtorrent::tcp::endpoint _endPoint;

    // Model items, these cannot be by value since the QStandardItemModel
    // takes ownership of objects and deletes them.
    QStandardItem * _hostItem,
                  * _stateItem,
                  * _balanceItem,
                  * _progressItem;
};

//#include <QMetaType>
//Q_DECLARE_METATYPE(PeerPluginViewModel *) // QStandardItem::setData(QVariant::fromValue(this))

#endif // PEER_PLUGIN_VIEW_MODEL_HPP
