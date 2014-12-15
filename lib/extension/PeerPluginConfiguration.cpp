#include "PeerPluginConfiguration.hpp"

PeerPluginConfiguration::PeerPluginConfiguration(const ExtendedMessageIdMapping & clientMapping, const ExtendedMessageIdMapping & peerMapping, const BEPSupportStatus & peerBEP10SupportedStatus, const BEPSupportStatus & peerBEP43SupportedStatus, const PeerPluginState & peerPluginState, const PeerPluginId & peerPluginId)
    : _clientMapping(clientMapping)
    , _peerMapping(peerMapping)
    , _peerBEP10SupportedStatus(peerBEP10SupportedStatus)
    , _peerBEP43SupportedStatus(peerBEP43SupportedStatus)
    , _peerPluginState(peerPluginState)
    , _peerPluginId(peerPluginId) {
}

PeerPluginConfiguration::PeerPluginConfiguration(const BEPSupportStatus & peerBEP10SupportedStatus, const BEPSupportStatus & peerBEP43SupportedStatus, const PeerPluginState & peerPluginState)
    : _peerBEP10SupportedStatus(peerBEP10SupportedStatus)
    , _peerBEP43SupportedStatus(peerBEP43SupportedStatus)
    , _peerPluginState(peerPluginState) {
}

PeerPluginConfiguration::PeerPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {
    // IMPLEMENT LATER
}
ExtendedMessageIdMapping PeerPluginConfiguration::peerMapping() const
{
    return _peerMapping;
}

void PeerPluginConfiguration::setPeerMapping(const ExtendedMessageIdMapping &peerMapping)
{
    _peerMapping = peerMapping;
}
ExtendedMessageIdMapping PeerPluginConfiguration::clientMapping() const
{
    return _clientMapping;
}

void PeerPluginConfiguration::setClientMapping(const ExtendedMessageIdMapping &clientMapping)
{
    _clientMapping = clientMapping;
}
BEPSupportStatus PeerPluginConfiguration::peerBEP10SupportedStatus() const
{
    return _peerBEP10SupportedStatus;
}

void PeerPluginConfiguration::setPeerBEP10SupportedStatus(const BEPSupportStatus &peerBEP10SupportedStatus)
{
    _peerBEP10SupportedStatus = peerBEP10SupportedStatus;
}
BEPSupportStatus PeerPluginConfiguration::peerBEP43SupportedStatus() const
{
    return _peerBEP43SupportedStatus;
}

void PeerPluginConfiguration::setPeerBEP43SupportedStatus(const BEPSupportStatus &peerBEP43SupportedStatus)
{
    _peerBEP43SupportedStatus = peerBEP43SupportedStatus;
}
PeerPluginState PeerPluginConfiguration::peerPluginState() const
{
    return _peerPluginState;
}

void PeerPluginConfiguration::setPeerPluginState(const PeerPluginState &peerPluginState)
{
    _peerPluginState = peerPluginState;
}
PeerPluginId PeerPluginConfiguration::peerPluginId() const
{
    return _peerPluginId;
}

void PeerPluginConfiguration::setPeerPluginId(const PeerPluginId &peerPluginId)
{
    _peerPluginId = peerPluginId;
}






