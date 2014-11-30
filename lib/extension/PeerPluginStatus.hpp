#ifndef PEER_PLUGIN_STATUS_HPP
#define PEER_PLUGIN_STATUS_HPP

#include <QMetaType> // Q_DECLARE_METATYPE

//#include "PeerPlugin.hpp" // for PeerPlugin::State field

// Proiblem is circular dependency, not enum!!
// Proiblem is circular dependency, not enum!!
// Proiblem is circular dependency, not enum!!
// Proiblem is circular dependency, not enum!!


class PeerPlugin;

class PeerPluginStatus
{
public:

    PeerPlugin * x;

    /*
    // Default constructor required for Qt MOC
    PeerPluginStatus();

    // Constructor
    PeerPluginStatus(State peerPluginStatus, int balance);

    PeerPlugin::State peerPluginStatus_;

    int balance_;*/
};

Q_DECLARE_METATYPE(PeerPluginStatus)

#endif // PEER_PLUGIN_STATUS_HPP
