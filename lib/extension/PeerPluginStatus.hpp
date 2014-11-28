#ifndef PEER_PLUGIN_STATUS_HPP
#define PEER_PLUGIN_STATUS_HPP

#include <QMetaType> // Q_DECLARE_METATYPE

class PeerPluginStatus
{
public:
    PeerPluginStatus();

    int balance;
};

Q_DECLARE_METATYPE(PeerPluginStatus)

#endif // PEER_PLUGIN_STATUS_HPP
