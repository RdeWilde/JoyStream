#ifndef PEER_PLUGIN_STATUS_HPP
#define PEER_PLUGIN_STATUS_HPP

#include <QMetaType> // Q_DECLARE_METATYPE

#include "PeerPluginState.hpp"

class PeerPluginStatus
{
public:

    // Default constructor required for Qt MOC
    PeerPluginStatus();

    // Constructor
    PeerPluginStatus(const PeerPlugin * peerPlugin, PeerPluginState peerPluginState, int balance);

    const PeerPluginState * peerPlugin_;

    PeerPluginState peerPluginState_;

    int balance_;
};

Q_DECLARE_METATYPE(PeerPluginStatus)

#endif // PEER_PLUGIN_STATUS_HPP
