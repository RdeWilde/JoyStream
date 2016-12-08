/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/Node.hpp>
#include <core/Torrent.hpp>
#include <core/Exception.hpp>
#include <core/TorrentIdentifier.hpp>
#include <core/detail/detail.hpp>
#include <libtorrent/error_code.hpp>
#include <functional>

Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)
Q_DECLARE_METATYPE(libtorrent::sha1_hash)

typedef std::map<libtorrent::sha1_hash, joystream::extension::status::TorrentPlugin> TorrentPluginStatuses;
Q_DECLARE_METATYPE(TorrentPluginStatuses)

namespace joystream {
namespace core {

void Node::registerMetaTypes() {

    qRegisterMetaType<libtorrent::tcp::endpoint>();
    qRegisterMetaType<libtorrent::sha1_hash>();
    Torrent::registerMetaTypes();
}

Node::Node(libtorrent::session * session,
           const boost::shared_ptr<extension::Plugin> & plugin)
    : _pimpl(session,
             plugin,
             std::bind(&Node::pimplStartedListeningHandler, this, std::placeholders::_1),
             std::bind(&Node::pimplTorrentAdded, this, std::placeholders::_1),
             std::bind(&Node::pimplTorrentRemoved, this, std::placeholders::_1),
             std::bind(&Node::pimplTorrentPluginStatusUpdate, this, std::placeholders::_1)) {
}

Node * Node::create() {

    // Create session instance
    libtorrent::session * session = new libtorrent::session(Node::session_settings(false),
                                                            // start_default_features: DHT, local service discovery, UPnP and NAT-PMP.
                                                            libtorrent::session_handle::session_flags_t::start_default_features |
                                                            // add_default_plugins: ut_pex, ut_metadata, lt_tex, smart_ban and possibly others.
                                                            libtorrent::session_handle::session_flags_t::add_default_plugins);

    // Check if session was actually started properly
    libtorrent::error_code ec;

    if(ec)
        throw exception::FailedToStartNodeException(ec);

    /// Setup DHT

    // Generate DHT settings
    libtorrent::dht_settings dht_settings;

    // Apply DHT settings to session
    session->set_dht_settings(dht_settings);

    // Add DHT routers
    session->add_dht_router(std::make_pair(std::string("router.bittorrent.com"), 6881));
    session->add_dht_router(std::make_pair(std::string("router.utorrent.com"), 6881));
    session->add_dht_router(std::make_pair(std::string("router.bitcomet.com"), 6881));

    // Enable DHT node, now that routers have been added
    session->apply_settings(Node::session_settings(true));

    /// Setup plugin

    // Create and install plugin
    boost::shared_ptr<extension::Plugin> plugin(new extension::Plugin(CORE_MINIMUM_EXTENDED_MESSAGE_ID));

    // Add plugin extension
    session->add_extension(boost::static_pointer_cast<libtorrent::plugin>(plugin));

    // Create actual node
    Node * node = new Node(session, plugin);

    // Set alerts notification callback in session
    // NB: This delayed setup of this notifcation handler w.r.t. creation of session
    // can lead to a race condition where the inition 0->1 event is missed, and so
    // the node misses all alerts until the session reaches its limit and dumps all alerts.
    // To avoid this, we make one initial call to processAlertQueue() at the end of the constructor.
    session->set_alert_notify([node]() { node->libtorrent_alert_notification_entry_point(); });

    // Resolve potential race condition described above
    node->processAlertQueue();

    return node;
}

void Node::pause(const NodePaused & nodePaused) {
    _pimpl.pause([nodePaused]() { nodePaused(); });
}

void Node::addTorrent(const boost::optional<uint> & uploadLimit,
                      const boost::optional<uint> & downloadLimit,
                      const std::string & name,
                      const std::vector<char> & resumeData,
                      const std::string & savePath,
                      bool paused,
                      const TorrentIdentifier & torrentReference,
                      const AddedTorrent & addedTorrent) {

    // Check that torrent not already present
    libtorrent::sha1_hash infoHash = torrentReference.infoHash();

    if(_pimpl._torrents.count(infoHash) != 0)
        throw exception::TorrentAlreadyExists(infoHash);

    // Create save_path on disk, return if it does not exist
    //if(!(QDir()).mkpath(params.save_path.c_str())) {
    //    qCCritical(_category) << "Could not create save_path: " << params.save_path.c_str();
    //    return false;
    // }

    // Convert to add torrent parameters
    libtorrent::add_torrent_params params = Node::toAddTorrentParams(uploadLimit,
                                                                     downloadLimit,
                                                                     name,
                                                                     resumeData,
                                                                     savePath,
                                                                     paused,
                                                                     torrentReference);

    _pimpl.addTorrent(params, addedTorrent);
}

void Node::removeTorrent(const libtorrent::sha1_hash & info_hash, const RemovedTorrent & handler) {
    _pimpl.removeTorrent(info_hash, handler);
}

void Node::postTorrentStatusUpdates() const noexcept {
    _pimpl.updateTorrentStatus();
}

void Node::postTorrentPluginStatusUpdates() const noexcept {
    _pimpl.postTorrentPluginStatusUpdates();
}

void Node::pimplStartedListeningHandler(const libtorrent::tcp::endpoint & endPoint) {
    emit startedListening(endPoint);
}

void Node::pimplTorrentAdded(core::Torrent * torrent) {
    emit addedTorrent(torrent);
}

void Node::pimplTorrentRemoved(const libtorrent::sha1_hash & info_hash) {
    emit removedTorrent(info_hash);
}

void Node::pimplTorrentPluginStatusUpdate(const std::map<libtorrent::sha1_hash, extension::status::TorrentPlugin> & status) {
    emit torrentPluginStatusUpdate(status);
}

void Node::libtorrent_alert_notification_entry_point() {

    // Process alert queue on main qt thread, not on libtorrent thread
    QMetaObject::invokeMethod(this, "processAlertQueue");
}

void Node::processAlertQueue() {
    _pimpl.processAlertQueue();
}

void Node::port() const noexcept {
    _pimpl.port();
}

std::map<libtorrent::sha1_hash, Torrent *> Node::torrents() const noexcept {
    return detail::getRawMap<libtorrent::sha1_hash, Torrent>(_pimpl._torrents);
}

libtorrent::settings_pack Node::session_settings(bool enableDHT) noexcept {

    // Initialize with default values
    libtorrent::settings_pack pack;

    // Setup alert filtering
    int ignoredAlerts =
                        // Enables alerts on events in the DHT node. For incoming searches or bootstrapping being done etc.
                        libtorrent::alert::dht_notification +

                        // Enables alerts for when blocks are requested and completed. Also when pieces are completed.
                        libtorrent::alert::progress_notification +

                        // Enables stats_alert approximately once every second, for every active torrent.
                        // These alerts contain all statistics counters for the interval since the lasts stats alert.
                        libtorrent::alert::stats_notification +

                        // Enables debug logging alerts. These are available unless libtorrent was
                        // built with logging disabled (TORRENT_DISABLE_LOGGING).
                        // The alerts being posted are log_alert and are session wide.
                        libtorrent::alert::session_log_notification +

                        // Enables debug logging alerts for torrents. These are available unless libtorrent was
                        // built with logging disabled (TORRENT_DISABLE_LOGGING). The alerts being posted are
                        // torrent_log_alert and are torrent wide debug events.
                        libtorrent::alert::torrent_log_notification +

                        // Enables debug logging alerts for peers. These are available unless libtorrent was
                        // built with logging disabled (TORRENT_DISABLE_LOGGING). The alerts being posted are
                        // peer_log_alert and low-level peer events and messages.
                        libtorrent::alert::peer_log_notification +

                        // Enables dht_log_alert, debug logging for the DHT
                        libtorrent::alert::dht_log_notification +

                        // Enables verbose logging from the piece picker
                        libtorrent::alert::picker_log_notification;

    pack.set_int(libtorrent::settings_pack::alert_mask, libtorrent::alert::all_categories & ~ ignoredAlerts);

    // Enable all default extensions, and possibly DHT.
    pack.set_bool(libtorrent::settings_pack::enable_upnp, true);
    pack.set_bool(libtorrent::settings_pack::enable_natpmp, true);
    pack.set_bool(libtorrent::settings_pack::enable_lsd, true);
    pack.set_bool(libtorrent::settings_pack::enable_dht, enableDHT);

    // This is the client identification to the tracker.
    // The recommended format of this string is: "ClientName/ClientVersion libtorrent/libtorrentVersion".
    // This name will not only be used when making HTTP requests, but also when sending BEP10 extended handshake
    // if handshake_client_version is left blank.
    // default: "libtorrent/" LIBTORRENT_VERSION
    pack.set_str(libtorrent::settings_pack::user_agent, std::string(CORE_USER_AGENT_NAME) +
                                                        std::string("/") +
                                                        std::to_string(CORE_VERSION_MAJOR) +
                                                        std::string(".") +
                                                        std::to_string(CORE_VERSION_MINOR));

    // Client name and version identifier sent to peers in the BEP10 handshake message.
    // If this is an empty string, the user_agent is used instead.
    // default: <user_agent>
    //pack.set_str(libtorrent::settings_pack::handshake_client_version, std::string(CORE_USER_AGENT_NAME) + CORE_VERSION_MAJOR + "." + CORE_VERSION_MINOR);

    // Fingerprint for the client.
    // It will be used as the prefix to the peer_id.
    // If this is 20 bytes (or longer) it will be used as the peer-id
    // There are two encoding styles, we use Azureus style, which is most popular:
    // '-', two characters for client id, four ascii digits for version number, '-', followed by random numbers.
    // For example: '-AZ2060-'...
    // default: "-LT1100-"
    std::string peerIdString = libtorrent::fingerprint(CORE_PEER_ID, CORE_VERSION_MAJOR, CORE_VERSION_MINOR, 0, 0).to_string();

    pack.set_str(libtorrent::settings_pack::peer_fingerprint, peerIdString);

    // Determines if connections from the same IP address as existing
    // connections should be rejected or not. Multiple connections from
    // the same IP address is not allowed by default, to prevent abusive behavior by peers.
    // It may be useful to allow such connections in cases where simulations
    // are run on the same machie, and all peers in a swarm has the same IP address.
    pack.set_bool(libtorrent::settings_pack::allow_multiple_connections_per_ip, true);


    return pack;
}

libtorrent::add_torrent_params Node::toAddTorrentParams(const boost::optional<uint> & uploadLimit,
                                                        const boost::optional<uint> & downloadLimit,
                                                        const std::string & name,
                                                        const std::vector<char> & resumeData,
                                                        const std::string & savePath,
                                                        bool pause,
                                                        const TorrentIdentifier & torrentIdentifier) noexcept {

    // Initialize with default values
    libtorrent::add_torrent_params params;

    if(uploadLimit.is_initialized())
        params.upload_limit = uploadLimit.value();

    if(downloadLimit.is_initialized())
        params.download_limit = downloadLimit.value();

    params.name = name;
    params.resume_data = resumeData;
    params.save_path = savePath;

    // NB: initial flags set in params is
    // default_flags = flag_pinned | flag_update_subscribe | flag_auto_managed | flag_paused | flag_apply_ip_filter

    //  bitwise OR or in a flag to set it:  params.flags |= flag_x
    //  bitwise AND the inverse of a flag to clear it:  params.flags &= ~flags_x;

    // If we wish to start unpaused, unset flag_paused which is set by default
    if(!pause)
        params.flags &= ~libtorrent::add_torrent_params::flags_t::flag_paused;

    // Remove auto-managing
    params.flags &= ~libtorrent::add_torrent_params::flags_t::flag_auto_managed;

    // Torrent refernce (usual, and safer, visitor pattern is a bit too heavy)
    switch(torrentIdentifier.type()) {

        case TorrentIdentifier::Type::Hash: params.info_hash = torrentIdentifier.infoHash(); break;

        case TorrentIdentifier::Type::MagnetLink: params.url = torrentIdentifier.magnetLink().toURI(); break;

        case TorrentIdentifier::Type::TorrentFile: params.ti = torrentIdentifier.torrentFile(); break;

        default:
            assert(false);
    }

    return params;
}

}
}
