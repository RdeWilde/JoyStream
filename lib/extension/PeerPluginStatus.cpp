#include "PeerPluginStatus.hpp"

PeerPluginStatus::PeerPluginStatus(PeerPlugin::State peerPluginStatus, int balance)
    : peerPluginStatus_(peerPluginStatus)
    , balance_(balance)
{
}
