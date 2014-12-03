#ifndef PEER_PLUGIN_VIEW_MODEL_HPP
#define PEER_PLUGIN_VIEW_MODEL_HPP

#include <QMetaType> // Q_DECLARE_METATYPE
#include <boost/asio/ip/tcp.hpp>

class PeerPlugin;
class QStandardItemModel;
class QStandardItem;
class PeerPluginStatus;
class QString;
enum class PeerPluginState;

class PeerPluginViewModel
{
public:

    // Constructor
    PeerPluginViewModel(const boost::asio::ip::tcp::endpoint & endPoint, QStandardItemModel * peerPluginsTableViewModel);

    // Destructor, is called from TorrentViewModel destructor
    ~PeerPluginViewModel();

    void update(PeerPluginStatus status);
    void updateHost(const QString & host);
    void updateState(PeerPluginState state);
    void updateBalance(int balance);
    void updateProgress(); // some representation of what it has given me?

private:

    // TCP/IP endpoint
    const boost::asio::ip::tcp::endpoint endPoint_;

    // View model for peer plugins table
    QStandardItemModel * peerPluginsTableViewModel_;

    // peerPluginsTableViewModel_ items
    QStandardItem * hostItem,
                  * stateItem,
                  * balanceItem,
                  * progressItem;
};

Q_DECLARE_METATYPE(PeerPluginViewModel *)

#endif // PEER_PLUGIN_VIEW_MODEL_HPP
