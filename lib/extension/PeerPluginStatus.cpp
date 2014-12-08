#include "PeerPluginStatus.hpp"

PeerPluginStatus::PeerPluginStatus()
    : peerPluginId_()
    , peerPluginState_(PeerPluginState::started) // not REALLY best state encoding for default
    , balance_(0)
{
}

PeerPluginStatus::PeerPluginStatus(const PeerPluginId & peerPluginId, PeerPluginState peerPluginState, int balance)
    : peerPluginId_(peerPluginId)
    , peerPluginState_(peerPluginState)
    , balance_(balance)
{
}
