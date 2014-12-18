#include "PeerPluginConfiguration.hpp"

PeerPluginConfiguration::PeerPluginConfiguration(const libtorrent::tcp::endpoint & endPoint
                                                 ,PluginMode pluginMode
                                                 ,const ExtendedMessageIdMapping & clientMapping
                                                 ,const ExtendedMessageIdMapping & peerMapping
                                                 ,const BEPSupportStatus & peerBEP10SupportedStatus
                                                 ,const BEPSupportStatus & peerBEP43SupportedStatus
                                                 ,const PeerPluginState & peerPluginState
                                                 ,const PeerPluginId & peerPluginId)
        : _endPoint(endPoint)
        ,_pluginMode(pluginMode)
        ,_clientMapping(clientMapping)
        ,_peerMapping(peerMapping)
        ,_peerBEP10SupportedStatus(peerBEP10SupportedStatus)
        ,_peerBEP43SupportedStatus(peerBEP43SupportedStatus)
        ,_peerPluginState(peerPluginState)
        ,_peerPluginId(peerPluginId) {
}

PeerPluginConfiguration::PeerPluginConfiguration(const libtorrent::tcp::endpoint & endPoint
                                                 ,PluginMode pluginMode
                                                 ,const BEPSupportStatus & peerBEP10SupportedStatus
                                                 ,const BEPSupportStatus & peerBEP43SupportedStatus
                                                 ,const PeerPluginState & peerPluginState)
        : _endPoint(endPoint)
        ,_pluginMode(pluginMode)
        ,_peerBEP10SupportedStatus(peerBEP10SupportedStatus)
        ,_peerBEP43SupportedStatus(peerBEP43SupportedStatus)
        ,_peerPluginState(peerPluginState) {
}

PeerPluginConfiguration::PeerPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {
    // IMPLEMENT LATER
}

ExtendedMessageIdMapping & PeerPluginConfiguration::getPeerMapping() {
    return _peerMapping;
}

void PeerPluginConfiguration::setPeerMapping(const ExtendedMessageIdMapping & peerMapping) {
    _peerMapping = peerMapping;
}

ExtendedMessageIdMapping & PeerPluginConfiguration::getClientMapping() {
    return _clientMapping;
}

void PeerPluginConfiguration::setClientMapping(const ExtendedMessageIdMapping & clientMapping) {
    _clientMapping = clientMapping;
}

BEPSupportStatus & PeerPluginConfiguration::getPeerBEP10SupportedStatus() {
    return _peerBEP10SupportedStatus;
}

void PeerPluginConfiguration::setPeerBEP10SupportedStatus(const BEPSupportStatus & peerBEP10SupportedStatus) {
    _peerBEP10SupportedStatus = peerBEP10SupportedStatus;
}

BEPSupportStatus & PeerPluginConfiguration::getPeerBEP43SupportedStatus() {
    return _peerBEP43SupportedStatus;
}

void PeerPluginConfiguration::setPeerBEP43SupportedStatus(const BEPSupportStatus &peerBEP43SupportedStatus) {
    _peerBEP43SupportedStatus = peerBEP43SupportedStatus;
}

PeerPluginState & PeerPluginConfiguration::getPeerPluginState() {
    return _peerPluginState;
}

void PeerPluginConfiguration::setPeerPluginState(const PeerPluginState & peerPluginState) {
    _peerPluginState = peerPluginState;
}

PeerPluginId & PeerPluginConfiguration::getPeerPluginId() {
    return _peerPluginId;
}

void PeerPluginConfiguration::setPeerPluginId(const PeerPluginId & peerPluginId) {
    _peerPluginId = peerPluginId;
}


