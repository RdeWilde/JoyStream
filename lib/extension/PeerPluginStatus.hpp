#ifndef PEER_PLUGIN_STATUS_HPP
#define PEER_PLUGIN_STATUS_HPP

#include "PeerPluginState.hpp"
#include "PeerPluginId.hpp"

class PeerPluginStatus
{
public:

    // Default constructor required for Qt MOC
    PeerPluginStatus();

    // Constructor
    PeerPluginStatus(const PeerPluginId & peerPluginId, PeerPluginState peerPluginState, int balance);

    // Source of status
    PeerPluginId peerPluginId_;

    // State of plugin
    PeerPluginState peerPluginState_;

    // Balance in favour of client
    int balance_;
};

#include <QMetaType>
Q_DECLARE_METATYPE(PeerPluginStatus)

#endif // PEER_PLUGIN_STATUS_HPP
