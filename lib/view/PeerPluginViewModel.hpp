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

    static const char * columnTitles[];
    static const int numberOfColumns;

    // Constructor
    PeerPluginViewModel(const boost::asio::ip::tcp::endpoint & endPoint, QStandardItemModel & peerPluginsTableViewModel);

    // Update routines
    void update(PeerPluginStatus status);
    void updateHost(const QString & host);
    void updateState(PeerPluginState state);
    void updateBalance(int balance);
    void updateProgress();

    // Getters
    const boost::asio::ip::tcp::endpoint & getEndPoint() const;

private:

    // TCP/IP endpoint
    boost::asio::ip::tcp::endpoint endPoint_;

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

Q_DECLARE_METATYPE(PeerPluginViewModel *)

#endif // PEER_PLUGIN_VIEW_MODEL_HPP
