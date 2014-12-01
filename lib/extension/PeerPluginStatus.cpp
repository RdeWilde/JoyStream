#include "PeerPluginStatus.hpp"

PeerPluginStatus::PeerPluginStatus()
    : peerPlugin_(0)
    , peerPluginState_(PeerPluginState::started) // not REALLY best state encoding for default
    , balance_(0)
{
}

PeerPluginStatus::PeerPluginStatus(const PeerPlugin * peerPlugin, PeerPluginState peerPluginState, int balance)
    : peerPlugin_(peerPlugin)
    , peerPluginState_(peerPluginState)
    , balance_(balance)
{
}

