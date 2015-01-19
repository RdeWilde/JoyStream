#ifndef PEER_PLUGIN_VIEW_MODEL_HPP
#define PEER_PLUGIN_VIEW_MODEL_HPP

#include <libtorrent/socket.hpp> // libtorrent::tcp::endpoint

class PeerPlugin;
class QStandardItemModel;
class QStandardItem;
class PeerPluginStatus;
class QString;
enum class PeerStatess PeerPluginViewModel
{
public:

    static const char * columnTitles[];
    static const int numberOfColumns;

    // Constructor
    PeerPluginViewModel(const libtorrent::tcp::endpoint & endPoint, QStandardItemModel & peerPluginsTableViewModel);

    // Update routines
    void update(PeerPluginStatus status);
    void updateHost(const QString & host);
    void updateState(PeerState);
    void updateBalance(int balance);
    void updateProgress();

    // Getters
    const libtorrent::tcp::endpoint & getEndPoint() const;

private:

    // TCP/IP endpoint
    libtorrent::tcp::endpoint endPoint_;

    // View model for peer plugins table. Is pointer since it is shared
    // among objects of this type.
    QStandardItemModel & peerPluginsTableViewModel_;

    // Model items, these cannot be by value since the QStandardItemModel
    // takes ownership of objects and deletes them.
    QStandardItem * hostItem,
                  * stateItem,
                  * balanceItem,
                  * progressItem;
};

#include <QMetaType>
Q_DECLARE_METATYPE(PeerPluginViewModel *) // QStandardItem::setData(QVariant::fromValue(this))

#endif // PEER_PLUGIN_VIEW_MODEL_HPP
