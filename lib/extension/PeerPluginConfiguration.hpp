#ifndef PEER_PLUGIN_CONFIGURATION_HPP
#define PEER_PLUGIN_CONFIGURATION_HPP

#include "BEPSupportStatus.hpp"
#include "Message/ExtendedMessageIdMapping.hpp"
#include "PeerPluginState.hpp"
#include "PeerPluginId.hpp"
#include "PluginMode.hpp"

/**
 * This class has been disabled for the time
 * being, as persisting peer plugin state is
 * way to complex to deal with at the moment.
 */
class PeerPluginConfiguration {

public:

    // Constructor from members
    PeerPluginConfiguration(const libtorrent::tcp::endpoint & endPoint
                            ,PluginMode pluginMode
                            ,const ExtendedMessageIdMapping & clientMapping
                            ,const ExtendedMessageIdMapping & peerMapping
                            ,const BEPSupportStatus & getPeerBEP10SupportedStatus
                            ,const BEPSupportStatus & peerBEP43SupportedStatus
                            ,const PeerPluginState & peerPluginState
                            ,const PeerPluginId & peerPluginId);

    // Constructor from members, when mappings are not known
    PeerPluginConfiguration(const libtorrent::tcp::endpoint & endPoint
                            ,PluginMode pluginMode
                            ,const BEPSupportStatus & getPeerBEP10SupportedStatus
                            ,const BEPSupportStatus & peerBEP43SupportedStatus
                            ,const PeerPluginState & peerPluginState);

    // Constructor from dictionary
    PeerPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    // Getters & Setters
    ExtendedMessageIdMapping & getPeerMapping();
    void setPeerMapping(const ExtendedMessageIdMapping & peerMapping);

    ExtendedMessageIdMapping & getClientMapping();
    void setClientMapping(const ExtendedMessageIdMapping & clientMapping);

    BEPSupportStatus & getPeerBEP10SupportedStatus();
    void setPeerBEP10SupportedStatus(const BEPSupportStatus & peerBEP10SupportedStatus);

    BEPSupportStatus & getPeerBEP43SupportedStatus();
    void setPeerBEP43SupportedStatus(const BEPSupportStatus & peerBEP43SupportedStatus);

    PeerPluginState & getPeerPluginState();
    void setPeerPluginState(const PeerPluginState & peerPluginState);

    PeerPluginId & getPeerPluginId();
    void setPeerPluginId(const PeerPluginId & peerPluginId);

protected:

    // Endpoint
    libtorrent::tcp::endpoint _endPoint;

    // Mode of plugin. All peers have same mode.
    PluginMode _pluginMode;

    // Mapping from messages to BEP10 ID of peer
    ExtendedMessageIdMapping _clientMapping, _peerMapping;

    // Indicates whether peer supports
    BEPSupportStatus _peerBEP10SupportedStatus, // BEP10
                        _peerBEP43SupportedStatus; // BEP43

    // State of peer plugin
    PeerPluginState _peerPluginState;

    // Id of this peer plugin
    PeerPluginId _peerPluginId; // assess later, is the redundancy worth it
};

#endif // PEER_PLUGIN_CONFIGURATION_HPP
