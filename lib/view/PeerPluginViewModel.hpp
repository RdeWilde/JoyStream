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

    // Default constructor so type can be added to container by value
    PeerPluginViewModel();

    // Destructor, is called from TorrentViewModel destructor
    ~PeerPluginViewModel();

    // Assignment operator required to put in std::map
    PeerPluginViewModel & operator=(const PeerPluginViewModel& rhs);

    void update(PeerPluginStatus status);
    void updateHost(const QString & host);
    void updateState(PeerPluginState state);
    void updateBalance(int balance);
    void updateProgress(); // some representation of what it has given me?

    // Getters
    const boost::asio::ip::tcp::endpoint & getEndPoint() const;
    QStandardItemModel * getPeerPluginsTableViewModel() const;
    QStandardItem * getHostItem() const;
    QStandardItem * getStateItem() const;
    QStandardItem * getBalanceItem() const;
    QStandardItem * getProgressItem() const;

private:

    // TCP/IP endpoint
    boost::asio::ip::tcp::endpoint endPoint_;

    // View model for peer plugins table
    QStandardItemModel * peerPluginsTableViewModel_;

    // peerPluginsTableViewModel_ items
    QStandardItem * hostItem,
                  * stateItem,
                  * balanceItem,
                  * progressItem;

    // Sets data of all items to be Qvairant of this object
    void setItemData();
};

Q_DECLARE_METATYPE(PeerPluginViewModel *)

#endif // PEER_PLUGIN_VIEW_MODEL_HPP
