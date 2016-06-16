/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 15 2016
 */

#include <core/Configuration.hpp>
#include <core/Exception.hpp>

#include <libtorrent/create_torrent.hpp>

#include <QJsonObject>

namespace joystream {
namespace core {
namespace configuration {

template <class T>
QJsonValue read<T>(const QJsonObject & json, const QString & key) {

    if(!json.contains(key))
        throw exception::MissingJSONKey(key.toStdString());

    QJsonValue v = json[key];

    if(!v.type() != T)
        throw exception::InvalidJSONValueType(key, T, v.type());

    return v;
}

//// Torrent

Torrent::Torrent() {}

Torrent::Torrent(const libtorrent::add_torrent_params & params)
    : _params(params) {}

/**
libtorrent::torrent_info Torrent::fromMagnetUri(const std::string & uri) {

    // Error code
    libtorrent::error_code ec;

    // parse_magnet_uri
    libtorrent::add_torrent_params params;

    // Parse link, wich recovers following fields into add_torrent_params
    // (1) std::vector<std::string> trackers
    // (2) std::vector<std::string> url_seeds
    // (3) std::vector<tcp::endpoint> peers
    // (4) std::vector<std::pair<std::string, int> > dht_nodes
    // (5) sha1_hash info_hash [will always be available if there was no error]
    // (6) std::string name
    libtorrent::parse_magnet_uri(uri, params, ec);

    // Throw error if URI could not be parsed
    if(ec) {

        std::stringstream s;
        s << "Could not decode magnet URI:" << ec.message().c_str();

        throw std::runtime_error(s.str());
    }

    // Create torrent_info object based on the parsed values
    libtorrent::torrent_info torrentInfo(params.info_hash);

    for(auto t : params.trackers)
        torrentInfo.add_tracker(t);

    for(auto u : params.url_seeds)
        torrentInfo.add_url_seed(u);

    // torrentInfo has no peers containers
    //for(auto p : params.peers)
    //    /torrentInfo.add

    for(auto n : params.dht_nodes)
        torrentInfo.add_node(n);

    // There si apparently no way to set the name of a torrent

    return torrentInfo;
}

libtorrent::torrent_info Torrent::fromTorrentFile(const std::string & file) {

    // Error code
    libtorrent::error_code ec;

    // Load torrent file
    libtorrent::torrent_info torrentFile(file, ec);

    // Throw error if file could not be parsed
    if(ec) {

        std::stringstream s;
        s << "Could not decode torrent file: " << ec.message().c_str();

        throw std::runtime_error(s.str());
    }

    return torrentFile;
}
*/

QJsonObject Torrent::toJSON() const {

    QJsonObject object;

    object["params"] = Torrent::toJSON(_params);

    return object;
}

Torrent Torrent::fromJSON(const QJsonObject & json) {

    QJsonObject paramsObject = read<QJsonValue::Type::Object>(json, "params");
    libtorrent::add_torrent_params params = Torrent::fromJSON(paramsObject);

    return Torrent(params);
}

QJsonObject Torrent::toJSON(const libtorrent::add_torrent_params & params) {

    QJsonObject object;

    object["name"] = QString::fromStdString(params.name);
    object["save_path"] = QString::fromStdString(params.save_path);

    QString resumeData;
    for(char c : params.resume_data)
        resumeData += c;

    object["resume_data"] = resumeData;
    object["ti"] = bencode(*params.ti);

    return object;
}

libtorrent::add_torrent_params Torrent::fromJSON(const QJsonObject & json) {

    // ti
    QString tiString = read<QJsonValue::Type::String>(json, "ti").toString();

    libtorrent::torrent_info ti = deBencode(tiString);

    // name
    QString name = read<QJsonValue::Type::String>(json, "name").toString();

    // save_path
    QString save_path = read<QJsonValue::Type::String>(json, "save_path").toString();

    // resume_data
    QString resume_data = read<QJsonValue::Type::String>(json, "resume_data").toString();

    std::vector<char> resumeData;
    for(auto x : resume_data)
        resumeData.push_back((char)x);

    // flags
    double flags = read<QJsonValue::Type::Double>(json, "flags").toDouble();

    // Create and return torrent
    libtorrent::add_torrent_params params;
    params.ti = std::boost<libtorrent::torrent_info>(new libtorrent::torrent_info(ti));
    params.name = name.toStdString();
    params.save_path = save_path.toStdString();
    params.resume_data = resumeData;
    params.flags = flags;

    return params;
}

QString Torrent::bencode(const libtorrent::torrent_info & ti) {

    // Create entry
    libtorrent::entry e = libtorrent::create_torrent(ti).generate();

    // Bencode
    std::vector<char> buffer;
    libtorrent::bencode(std::back_inserter(buffer), e);

    // Serialize buffer into QString
    QString bencoded;
    for(char c: buffer)
        bencoded += c;

    return bencoded;
}

libtorrent::torrent_info Torrent::deBencode(const QString & bencoding) {

    QByteArray data = bencoding.toUtf8();

    return libtorrent::torrent_info(data.data(), data.length());
}

//// Controller

Controller::Controller() {

    ////////////////////////////////////////////////////
    // Create settings for sesion
    // this has all been hard coded last moment due to new
    // libtorrent changes.
    ////////////////////////////////////////////////////

    /**libtorrent::settings_pack settings;


    settings.set_int(libtorrent::settings_pack::active_loaded_limit, 20);
    settings.set_int(libtorrent::settings_pack::choking_algorithm, libtorrent::settings_pack::rate_based_choker);
    //settings.set_int(libtorrent::settings_pack::half_open_limit, atoi(arg))
    settings.set_bool(libtorrent::settings_pack::allow_multiple_connections_per_ip, true);
    //settings.set_bool(libtorrent::settings_pack::use_disk_read_ahead, false);
    //settings.set_int(libtorrent::settings_pack::suggest_mode, libtorrent::settings_pack::suggest_read_cache);
    //settings.set_int(libtorrent::settings_pack::peer_timeout, atoi(arg));
    //settings.set_bool(libtorrent::settings_pack::announce_to_all_tiers, true);
    //settings.set_int(libtorrent::settings_pack::hashing_threads, atoi(arg));
    //settings.set_int(libtorrent::settings_pack::download_rate_limit, atoi(arg) * 1000);
    //settings.set_int(libtorrent::settings_pack::upload_rate_limit, atoi(arg) * 1000);
    //settings.set_int(libtorrent::settings_pack::unchoke_slots_limit, atoi(arg));
    //settings.set_int(libtorrent::settings_pack::urlseed_wait_retry, atoi(arg));
    settings.set_bool(libtorrent::settings_pack::enable_dht, true);
    //settings.set_int(libtorrent::settings_pack::listen_queue_size, atoi(arg));
    //settings.set_int(libtorrent::settings_pack::out_enc_policy, libtorrent::settings_pack::pe_forced);
    //settings.set_int(libtorrent::settings_pack::in_enc_policy, libtorrent::settings_pack::pe_forced);
    //settings.set_int(libtorrent::settings_pack::allowed_enc_level, libtorrent::settings_pack::pe_rc4);
    //settings.set_bool(libtorrent::settings_pack::prefer_rc4, true);
    //settings.set_int(libtorrent::settings_pack::max_peerlist_size, atoi(arg));
    //settings.set_int(libtorrent::settings_pack::max_paused_peerlist_size, atoi(arg) / 2);
    //settings.set_int(libtorrent::settings_pack::connections_limit, atoi(arg));
    //settings.set_str(libtorrent::settings_pack::i2p_hostname, arg);
    //settings.set_int(libtorrent::settings_pack::i2p_port, 7656);
    //settings.set_int(libtorrent::settings_pack::proxy_type, libtorrent::settings_pack::i2p_proxy);
    //settings.set_int(libtorrent::settings_pack::cache_size, atoi(arg));
    //settings.set_bool(libtorrent::settings_pack::use_read_cache, atoi(arg) > 0);
    //settings.set_int(libtorrent::settings_pack::cache_buffer_chunk_size, atoi(arg) / 100);
    //settings.set_int(libtorrent::settings_pack::allowed_fast_set_size, atoi(arg));
    //settings.set_int(libtorrent::settings_pack::read_cache_line_size, atoi(arg));
    //settings.set_int(libtorrent::settings_pack::mixed_mode_algorithm, libtorrent::settings_pack::prefer_tcp);
    //settings.set_bool(libtorrent::settings_pack::enable_outgoing_tcp, true);
    //settings.set_bool(libtorrent::settings_pack::enable_incoming_tcp, true);
    //settings.set_str(libtorrent::settings_pack::proxy_hostname, arg);
    //settings.set_int(libtorrent::settings_pack::proxy_port, atoi(port));

    //if (settings.get_int(libtorrent::settings_pack::proxy_type) == libtorrent::settings_pack::none)
    //        settings.set_int(libtorrent::settings_pack::proxy_type, libtorrent::settings_pack::socks5);

    //settings.set_str(libtorrent::settings_pack::proxy_username, arg);
    //settings.set_str(libtorrent::settings_pack::proxy_password, pw);
    //settings.set_int(libtorrent::settings_pack::proxy_type, settings_pack::socks5_pw);
    //settings.set_str(libtorrent::settings_pack::outgoing_interfaces, arg);
    settings.set_bool(libtorrent::settings_pack::enable_upnp, true);
    settings.set_bool(libtorrent::settings_pack::enable_natpmp, true);
    settings.set_bool(libtorrent::settings_pack::enable_lsd, true);
    //settings.set_str(libtorrent::settings_pack::mmap_cache, arg);
    //settings.set_bool(libtorrent::settings_pack::contiguous_recv_buffer, false);
    //settings.set_int(libtorrent::settings_pack::active_downloads, atoi(arg));
    //settings.set_int(libtorrent::settings_pack::active_limit, atoi(arg) * 2);
    //settings.set_int(libtorrent::settings_pack::active_seeds, atoi(arg));
    //settings.set_int(libtorrent::settings_pack::active_limit, atoi(arg) * 2);


    // setup default values
    libtorrent::high_performance_seed(settings);

    // Listening settings
    //settings.set_str(libtorrent::settings_pack::listen_interfaces, "0.0.0.0:6881");

    //settings.set_str(libtorrent::settings_pack::user_agent, CLIENT_FINGERPRINT); //  + JOYSTREAM_VERSION_MAJOR + JOYSTREAM_VERSION_MINOR
    //settings.set_str(libtorrent::settings_pack::user_agent, "client_test/" LIBTORRENT_VERSION);

    settings.set_int(libtorrent::settings_pack::alert_mask,
                     //libtorrent::alert::all_categories
                     libtorrent::alert::error_notification +
                     libtorrent::alert::tracker_notification +
                     libtorrent::alert::debug_notification +
                     libtorrent::alert::status_notification +
                     libtorrent::alert::progress_notification +
                     libtorrent::alert::performance_warning +
                     libtorrent::alert::stats_notification
                     );
    */

    std::clog << "Libtorrent session started";

    // Set session settings - these acrobatics with going back and forth seem to indicate that I may have done it incorrectly
    std::vector<char> buffer;
    libtorrent::bencode(std::back_inserter(buffer), configuration.getLibtorrentSessionSettingsEntry());
    libtorrent::lazy_entry settingsLazyEntry;
    libtorrent::error_code lazyBdecodeEc;
    libtorrent::lazy_bdecode(&buffer[0], &buffer[0] + buffer.size(), settingsLazyEntry, lazyBdecodeEc);
    _session->load_state(settingsLazyEntry);

    // Add DHT routing nodes
    // ======================================
    const std::vector<std::pair<std::string, int>> & dhtRouters = configuration.getDhtRouters();
    for(std::vector<std::pair<std::string, int>>::const_iterator i = dhtRouters.begin(),
            end(dhtRouters.end());i != end; ++i)
        _session->add_dht_router(*i); // Add router to session

    /**
    // Start dht node?
    libtorrent::dht_settings dht;
    //dht.privacy_lookups = true;
    _session->set_dht_settings(dht);

    settings.set_bool(libtorrent::settings_pack::use_dht_as_fallback, false); // use as main?

    _session->add_dht_router(std::make_pair(std::string("router.bittorrent.com"), 6881));
    _session->add_dht_router(std::make_pair(std::string("router.utorrent.com"), 6881));
    _session->add_dht_router(std::make_pair(std::string("router.bitcomet.com"), 6881));

    // Setup alert processing callback
    _session->set_alert_notify(boost::bind(&Controller::libtorrent_entry_point_alert_notification, this));
    */


    // Setup session settings
    libtorrent::session_settings sessionSettings;

    /* session_settings:
    *	This holds most of the session-wide settings in libtorrent.
    *	Pass this to session::set_settings() to change the settings,
    *	initialize it from session::get_settings() to get the current settings.
    *	http://libtorrent.org/reference-Settings.html#session_settings
    */

    // Set session settings

    /* connections_limit:
     * sets a global limit on the number of connections opened.
     * The number of connections is set to a hard minimum of at least two per torrent,
     * so if you set a too low connections limit,
     * and open too many torrents, the limit will not be met.
     */
    //sessionSettings.connections_limit =

    /* user_agent:
    *	the client identification to the tracker. The recommended format of
    *	this string is: "ClientName/ClientVersion libtorrent/libtorrentVersion".
    *	This name will not only be used when making HTTP requests, but also when
    *	sending extended headers to peers that support that extension.
    */
    sessionSettings.user_agent = CORE_EXTENSION_FINGERPRINT + std::string("/") + std::to_string(CORE_VERSION_MAJOR) + std::string(".") + std::to_string(CORE_VERSION_MINOR);

    /* choking_algorithm:
    *	Specifies which algorithm to use to determine which peers to unchoke.
    *	This setting replaces the deprecated settings auto_up_slots and
    *	auto_upload_slots_rate_based.
    */
    sessionSettings.choking_algorithm = libtorrent::session_settings::auto_expand_choker;

    /* disk_cache_algorithm
    *	tells the disk I/O thread which cache flush algorithm to use.
    *	This is specified by the disk_cache_algo_t enum.
    */
    sessionSettings.disk_cache_algorithm = libtorrent::session_settings::avoid_readback;

    /* volatile_read_cache
    *	if this is set to true, read cache blocks that are hit by peer read requests
    *	are removed from the disk cache to free up more space. This is useful if you
    *	don't expect the disk cache to create any cache hits from other peers than
    *	the one who triggered the cache line to be read into the cache in the first place.
    */
    sessionSettings.volatile_read_cache = true; // <-- worth taking a closer look at

    /* half_open_limit
    *	sets the maximum number of half-open connections libtorrent
    *	will have when connecting to peers. A half-open connection is
    *	one where connect() has been called, but the connection still
    *	hasn't been established (nor failed). Windows XP Service Pack 2
    *	sets a default, system wide, limit of the number of half-open
    *	connections to 10. So, this limit can be used to work nicer
    *	together with other network applications on that system.
    *	The default is to have no limit, and passing -1 as the limit,
    *	means to have no limit. When limiting the number of simultaneous
    *	connection attempts, peers will be put in a queue waiting for their
    *	turn to get connected.
    */
    // s_s.half_open_limit = -1;

    /* allow_multiple_connections_per_ip
    *	determines if connections from the same IP address as existing
    *	connections should be rejected or not. Multiple connections from
    *	the same IP address is not allowed by default, to prevent abusive
    *	behavior by peers. It may be useful to allow such connections in
    *	cases where simulations are run on the same machie, and all peers
    *	in a swarm has the same IP address.
    **/

    // THIS SHOULD REALLY ONLY BE TRUE WHEN WE ARE DOING TEST ON THE SAME MACHINE
    // NOT IN GENERAL
    sessionSettings.allow_multiple_connections_per_ip = true;


    // the maximum times we try to connect to a peer before stop connecting
    // again. If a peer succeeds, its failcounter is reset. If a peer is
    // retrieved from a peer source (other than DHT) the failcount is
    // decremented by one, allowing another try.
    // DEFAULT = 3
    sessionSettings.max_failcount = 3;

    // the number of seconds to wait to reconnect to a peer. this time is
    // multiplied with the failcount.
    // DEFAULT = 60
    sessionSettings.min_reconnect_time = 3;

    /* use_disk_read_ahead
    *	defaults to true and will attempt to optimize disk reads by giving
    *	the operating system heads up of disk read requests as they are
    *	queued in the disk job queue. This gives a significant performance
    *	boost for seeding.
    */
    sessionSettings.use_disk_read_ahead = true;

    /* disable_hash_checks
    *	controls if downloaded pieces are verified against the piece hashes
    *	in the torrent file or not. The default is false, i.e. to verify all
    *	downloaded data. It may be useful to turn this off for performance
    *	profiling and simulation scenarios. Do not disable the hash check
    *	for regular bittorrent clients.
    */
    sessionSettings.disable_hash_checks = false;

    /* peer_timeout
    *	the number of seconds to wait for any activity on the peer wire before
    *	closing the connectiong due to time out. This defaults to 120 seconds,
    *	since that's what's specified in the protocol specification. After half
    *	the time out, a keep alive message is sent.
    */
    //sessionSettings.peer_timeout = 120;

    /* announce_to_all_tiers
    *	controls how multi tracker torrents are treated. When this is set to true,
    *	one tracker from each tier is announced to. This is the uTorrent behavior.
    *	This is false by default in order to comply with the multi-tracker specification.
    */
    //sessionSettings.announce_to_all_tiers = false;

    /* download_rate_limit
    *	sets the session-global limits of upload and download rate limits, in bytes
    *	per second. The local rates refer to peers on the local network. By default
    *	peers on the local network are not rate limited. These rate limits are only
    *	used for local peers (peers within the same subnet as the client itself) and
    *	it is only used when session_settings::ignore_limits_on_local_network is set
    *	to true (which it is by default). These rate limits default to unthrottled,
    *	but can be useful in case you want to treat local peers preferentially, but
    *	not quite unthrottled. A value of 0 means unlimited.
    */
    sessionSettings.download_rate_limit = 0;// kbyte/s * 1000 = bytes/s;
    sessionSettings.upload_rate_limit = 0;// kbyte/s * 1000 = byte/s;
    sessionSettings.ignore_limits_on_local_network = false;

    /* unchoke_slots_limit
    *	the max number of unchoked peers in the session. The number of unchoke slots
    *	may be ignored depending on what choking_algorithm is set to.
    *	A value of -1 means infinite.
    */
    sessionSettings.unchoke_slots_limit = 4; // <-- value suggested by spec, but may be disregarded if choking_algorithm != fixed_slots_choker

    /* max_peerlist_size
    *	the maximum number of peers in the list of known peers. These peers are not
    *	necessarily connected, so this number should be much greater than the maximum
    *	number of connected peers. Peers are evicted from the cache when the list grows
    *	passed 90% of this limit, and once the size hits the limit, peers are no longer
    *	added to the list. If this limit is set to 0, there is no limit on how many peers
    *	we'll keep in the peer list.
    */
    sessionSettings.max_peerlist_size = 0;

    /* cache_size
    *	the disk write and read cache. It is specified in units of 16 KiB blocks.
    *	Buffers that are part of a peer's send or receive buffer also count against
    *	this limit. Send and receive buffers will never be denied to be allocated,
    *	but they will cause the actual cached blocks to be flushed or evicted.
    *	If this is set to -1, the cache size is automatically set to the amount
    *	of physical RAM available in the machine divided by 8. If the amount of
    *	physical RAM cannot be determined, it's set to 1024 (= 16 MiB).
    *
    *	Disk buffers are allocated using a pool allocator, the number of blocks that
    *	are allocated at a time when the pool needs to grow can be specified in
    *	cache_buffer_chunk_size. This defaults to 16 blocks.
    *	Lower numbers saves memory at the expense of more heap allocations.
    *	It must be at least 1.
    */
    sessionSettings.cache_size = -1; // check these values later.
    sessionSettings.use_read_cache = sessionSettings.cache_size > 0;
    sessionSettings.cache_buffer_chunk_size = sessionSettings.cache_size / 100;
    //sessionSettings.read_cache_line_size =

    /* allow_reordered_disk_operations
    *	if this is true, disk read operations may be re-ordered based on their
    *	physical disk read offset. This greatly improves throughput when uploading
    *	to many peers. This assumes a traditional hard drive with a read head and
    *	spinning platters. If your storage medium is a solid state drive,
    *	this optimization doesn't give you an benefits
    */
    sessionSettings.allow_reordered_disk_operations = true;

    /* mixed_mode_algorithm
    *	determines how to treat TCP connections when there are uTP connections.
    *	Since uTP is designed to yield to TCP, there's an inherent problem when
    *	using swarms that have both TCP and uTP connections. If nothing is done,
    *	uTP connections would often be starved out for bandwidth by the TCP connections.
    *	This mode is prefer_tcp. The peer_proportional mode simply looks at the current
    *	throughput and rate limits all TCP connections to their proportional share based
    *	on how many of the connections are TCP. This works best if uTP connections are
    *	not rate limited by the global rate limiter, see rate_limit_utp.
    *	see bandwidth_mixed_algo_t for options.
    */
    sessionSettings.mixed_mode_algorithm = libtorrent::session_settings::prefer_tcp;

    /* active management fields
    *	determines how the DHT is used. If this is true, the DHT will only be used for
    *	torrents where all trackers in its tracker list has failed. Either by an explicit
    *	error message or a time out. This is false by default, which means the DHT is used
    *	by default regardless of if the trackers fail or not.
    */
    //sessionSettings.active_downloads = atoi(arg);
    //sessionSettings.active_seeds = atoi(arg);

    /* use_dht_as_fallback
    *	determines how the DHT is used. If this is true, the DHT will only be used for torrents
    *	where all trackers in its tracker list has failed. Either by an explicit error message
    *	or a time out. This is false by default, which means the DHT is used by default regardless
    *	of if the trackers fail or not.
    */
    //sessionSettings.use_dht_as_fallback = false;

    // Set dht settings
    libtorrent::dht_settings dhtSettings;

    // Set encryption settings
    //libtorrent::pe_settings peerEncryptionSettings;

    // Set proxy settings
    //libtorrent::proxy_settings proxySettings;

    // Create dummy session so that settings can be set
    // and a full session settings entry can be extracted
    libtorrent::session dummySession;
    dummySession.set_settings(sessionSettings);
    dummySession.set_dht_settings(dhtSettings);
    //dummySession.set_pe_settings(peerEncryptionSettings);
    //dummySession.set_proxy(proxySettings);
    dummySession.save_state(_libtorrentSessionSettingsEntry);

    // Set port range
    _portRange = std::make_pair(6881, 6889);

    // Set dht routers
    _dhtRouters.push_back(std::make_pair(std::string("router.bittorrent.com"), 6881));
    _dhtRouters.push_back(std::make_pair(std::string("router.utorrent.com"), 6881));
    _dhtRouters.push_back(std::make_pair(std::string("router.bitcomet.com"), 6881));
}

Controller::Controller(const libtorrent::entry & libtorrentSessionSettingsEntry,
                       const std::pair<int, int> & portRange,
                       const std::vector<std::pair<std::string, int> > & dhtRouters,
                       const std::vector<TorrentConfiguration> & torrents)
                                :_libtorrentSessionSettingsEntry(libtorrentSessionSettingsEntry)
                                ,_portRange(portRange)
                                ,_dhtRouters(dhtRouters)
                                ,_torrents(torrents) {}

Controller Controller::fromJSON(const QJsonObject & json) {

    /*
    // Check that libtorrentSettings key is present, and then parse
    if(dictionaryEntry.count("libtorrentSettings") == 1)
        _libtorrentSessionSettingsEntry = dictionaryEntry.find("libtorrentSettings")->second;
    else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "There is not exactly one libtorrentSettings key.");

    // Check that portRange key is present, and then parse
    if(dictionaryEntry.count("portRange") == 1) {

        const libtorrent::entry & portRangeEntry = dictionaryEntry.find("portRange")->second;

        // Check that entry is of type entry::list_type
        if(portRangeEntry.type() == libtorrent::entry::data_type::list_t) {

            // Use as list
            const libtorrent::entry::list_type & portRangeListEntry = portRangeEntry.list();

            // Check that list is of length two
            if(portRangeListEntry.size() == 2) {

                // Get two elements
                libtorrent::entry::list_type::const_iterator i = portRangeListEntry.begin();

                const libtorrent::entry & firstListElement = *i;
                i++;
                const libtorrent::entry & secondListElement = *i;

                // Check that both are integers
                if(firstListElement.type() == libtorrent::entry::data_type::int_t &&
                    secondListElement.type() == libtorrent::entry::data_type::int_t) {

                    // Use as integers
                    const libtorrent::entry::integer_type firstInteger = firstListElement.integer();
                    const libtorrent::entry::integer_type secondInteger = secondListElement.integer();

                    // Add to portRange
                    _portRange = std::make_pair(static_cast<int>(firstInteger), static_cast<int>(secondInteger));

                } else
                    throw InvalidBitSwaprStateEntryException(dictionaryEntry, "elements in portRange entry are not of type entry::int_type.");
            } else
                throw InvalidBitSwaprStateEntryException(dictionaryEntry, "portRange entry is not a list of length 2.");
        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "portRange key is not of type entry::list_type.");
    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "libtorrentSettings key should have .count == 1.");

    // Check that dhtRouters key is present, and then parse
    if(dictionaryEntry.count("dhtRouters") == 1) {

        const libtorrent::entry & dhtRoutersEntry = dictionaryEntry.find("dhtRouters")->second;

        // Check that entry is of type entry::list_typ
        if(dhtRoutersEntry.type() == libtorrent::entry::data_type::list_t) {

            // Use as list
            const libtorrent::entry::list_type & dhtRoutersListEntry = dhtRoutersEntry.list();

            // Iterate list and parse dht routing pairs
            for(libtorrent::entry::list_type::const_iterator i = dhtRoutersListEntry.begin(),
                    end(dhtRoutersListEntry.end()); i != end;i++) {

                // Get list entry element
                const libtorrent::entry & dhtRoutersListElementEntry = *i;

                // Check that it is a list
                if(dhtRoutersListElementEntry.type() == libtorrent::entry::data_type::list_t) {

                    // Use as list
                    const libtorrent::entry::list_type & dhtRoutersListElementListEntry = dhtRoutersListElementEntry.list();

                    // Check that list is of length two
                    if(dhtRoutersListElementListEntry.size() == 2) {

                        // Get two elements
                        libtorrent::entry::list_type::const_iterator i = dhtRoutersListElementListEntry.begin();

                        const libtorrent::entry hostNameEntry = *i;
                        i++;
                        const libtorrent::entry portEntry = *i;

                        // Check that both are integers
                        if(hostNameEntry.type() == libtorrent::entry::data_type::string_t &&
                            portEntry.type() == libtorrent::entry::data_type::int_t) {

                            // Get in correct types
                            libtorrent::entry::string_type hostNameStringEntry = hostNameEntry.string();
                            libtorrent::entry::integer_type portIntegerEntry = portEntry.integer();

                            // Add to dhtRouters
                            _dhtRouters.push_back(std::make_pair(hostNameStringEntry.c_str(), (int)portIntegerEntry));

                        } else
                            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "elements in dhtRouters entry are not of type <entry::string_type, entry::int_type>.");
                    } else
                        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "dhtRouters entry is not a list of length 2.");
                } else
                    throw InvalidBitSwaprStateEntryException(dictionaryEntry, "dhtRouters list element entry is not of type entry::list_type.");
            }

        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "dhtRouters entry is not of type entry::list_type.");
    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "dhtRouters key should have .count == 1.");

    // Check that addTorrentParameters key is present, and then parse
    if(dictionaryEntry.count("persistentTorrentStates") == 1) {

        const libtorrent::entry & persistentTorrentStatesEntry = dictionaryEntry.find("persistentTorrentStates")->second;

        // Check that entry is of type entry::list_typ
        if(persistentTorrentStatesEntry.type() == libtorrent::entry::data_type::list_t) {

            // Use as list
            const libtorrent::entry::dictionary_type & persistentTorrentStatesDictionaryEntry = persistentTorrentStatesEntry.dict();

            // Iterate list and parse add_torrent_params structs
            for(libtorrent::entry::dictionary_type::const_iterator i = persistentTorrentStatesDictionaryEntry.begin(),
                    end(persistentTorrentStatesDictionaryEntry.end()); i != end;i++) {

                // Get key and value
                const libtorrent::sha1_hash info_hash(i->first);
                const libtorrent::entry & persistentTorrentStateEntry = i->second;

                // Check that it is a dictionary
                if(persistentTorrentStateEntry.type() == libtorrent::entry::data_type::dictionary_t) {

                    // Use as dictionary
                    const libtorrent::entry::dictionary_type & persistentTorrentStateDictionaryEntry = persistentTorrentStateEntry.dict();

                    // Add to torrentAddTorrentParameters
                    //_torrentConfigurations[info_hash] = TorrentConfiguration(persistentTorrentStateDictionaryEntry); // new
                    _torrentConfigurations.push_back(new TorrentConfiguration(persistentTorrentStateDictionaryEntry));

                } else
                    throw InvalidBitSwaprStateEntryException(dictionaryEntry, "persistentTorrentStates has value that is not of type entry::dict_type.");
            }

        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "persistentTorrentStates is not of type entry::dict_type.");
    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "persistentTorrentStates key should have .count == 1.");

   */
}

QJsonObject Controller::toJSON() {

    /*
    // Add "libtorrentSettings" key
    dictionaryEntry["libtorrentSettings"] = _libtorrentSessionSettingsEntry;

    // Add "portRange" key
    libtorrent::entry::list_type portRangeListEntry;
    portRangeListEntry.push_back(_portRange.first);
    portRangeListEntry.push_back(_portRange.second);

    dictionaryEntry["portRange"] = portRangeListEntry;

    // Add "dhtRouters" key
    libtorrent::entry::list_type dhtRoutersListEntry;

    for(std::vector<std::pair<std::string, int>>::const_iterator i = _dhtRouters.begin(),
        end(_dhtRouters.end());i != end; i++) {

        // Create entry list for dht pair
        libtorrent::entry::list_type routerEntry;

        routerEntry.push_back((*i).first);
        routerEntry.push_back((*i).second);

        // Add to list entry
        dhtRoutersListEntry.push_back(routerEntry);
    }

    dictionaryEntry["dhtRouters"] = dhtRoutersListEntry;

    // Add "torrentConfigurations" key
    libtorrent::entry::list_type torrentConfigurationsListEntry;

    for(std::vector<TorrentConfiguration *>::const_iterator i = _torrentConfigurations.begin(),
        end(_torrentConfigurations.end()); i != end; i++) {

        // Write to dictionary
        libtorrent::entry::dictionary_type dictionaryEntry;
        (*i)->toDictionaryEntry(dictionaryEntry);

        // Add to list
        torrentConfigurationsListEntry.push_back(dictionaryEntry);
    }

    dictionaryEntry["torrentConfigurations"] = torrentConfigurationsListEntry;

    // Add "walletFile" key
    walletFile...
    */
}

Controller Controller::fromFile(const std::string & file) {

    /*
    // Create dictionary entry
    libtorrent::entry ControllerEntry;

    // Save bencoded dictionary to file
    Utilities::loadBencodedEntry(fileName, ControllerEntry);

    // Convert to dictionary entry
    libtorrent::entry::dictionary_type ControllerDictionaryEntry = ControllerEntry.dict();

    // Use other constructor using this dictionary
    ControllerController::Controller(ControllerDictionaryEntry);
    */

    return Controller();
}

void Controller::toFile(const std::string & file) {

    /*
    // Create dictionary entry
    libtorrent::entry::dictionary_type ControllerDictionaryEntry;

    // Save controller configururation in entry
    toDictionaryEntry(ControllerDictionaryEntry);

    // Save bencoded dictionary to file
    Utilities::saveBencodedEntry(fileName, ControllerDictionaryEntry);
    */
}



}
}
}
