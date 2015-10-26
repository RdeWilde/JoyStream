/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/controller/Controller.hpp>
//#include "TorrentStatus.hpp"

/**
 * Controller::Torrent::Configuration
 */

#include <core/controller/exceptions/InvalidBitSwaprStateEntryException.hpp>
#include <core/extension/PluginMode.hpp>

Controller::Torrent::Configuration::Configuration() {
}

Controller::Torrent::Configuration::Configuration(const libtorrent::sha1_hash & infoHash,
                                           const std::string & name,
                                           const std::string & savePath,
                                           const std::vector<char> & resumeData,
                                           quint64 flags,
                                           //const libtorrent::torrent_info & torrentInfo
                                           const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile
                                           )
                                           //const TorrentPlugin::Configuration * torrentPluginConfiguration)
    :_infoHash(infoHash)
    ,_name(name)
    ,_savePath(savePath)
    ,_resumeData(resumeData)
    ,_flags(flags)
    ,_torrentFile(torrentFile) {
    //,_torrentPluginConfiguration(torrentPluginConfiguration) {
}

Controller::Torrent::Configuration::~Configuration() {

    //if(_torrentFile != NULL)
        //delete _torrentFile;
}

Controller::Torrent::Configuration Controller::Torrent::Configuration::fromTorrentFile(const QString & resource) {
    // Error code
    libtorrent::error_code ec;

    // Load torrent file
    libtorrent::torrent_info * torrentInfo = new libtorrent::torrent_info(resource.toStdString().c_str(), ec);

    // Was torrent file valid?
    if(ec) {
        qDebug() << "Invalid torrent file: " << ec.message().c_str();
        throw (std::runtime_error("Invalid Torrent File"));
    }

    // Resume data
    std::vector<char> resume_data;

    // Save Path
    std::string save_path = "";

    Controller::Torrent::Configuration configuration(torrentInfo->info_hash(),
                                                      torrentInfo->name(),
                                                      save_path,
                                                      resume_data,
                                                      libtorrent::add_torrent_params::flag_update_subscribe,
                                                      torrentInfo);

    return configuration;
}

Controller::Torrent::Configuration Controller::Torrent::Configuration::fromMagnetLink(const QString & resource) {
    // Error code
    libtorrent::error_code ec;

    // Magnet link url
    std::string url = resource.toStdString();

    // parse_magnet_uri
    libtorrent::add_torrent_params params;

    // Parse link to get info_hash
    libtorrent::parse_magnet_uri(url, params, ec);

    // Was magnet link malformed
    if(ec) {
        qDebug() << "Invalid magnet link: " << ec.message().c_str();
        throw (std::runtime_error("Invalid Magnet Link"));
    }

    // Resume data
    std::vector<char> resume_data;

    // Save Path
    std::string save_path = "";

    Controller::Torrent::Configuration configuration(params.info_hash,
                                                      params.name,
                                                      save_path,
                                                      resume_data,
                                                      libtorrent::add_torrent_params::flag_update_subscribe,
                                                      NULL);

    return configuration;
}

/*
TorrentConfiguration::TorrentConfiguration()
                    : _torrentInfo(NULL) {
}

TorrentConfiguration & TorrentConfiguration::operator=(const TorrentConfiguration & rhs) {

    // Copy members
    _infoHash = rhs.getInfoHash();
    _name = rhs.getName();
    _savePath = rhs.getSavePath();
    _resumeData = rhs.getConstResumeData();
    _flags = rhs.getFlags();

    // Delete if was set to object
    if(_torrentInfo != NULL)
        delete _torrentInfo;

    // Allocate copy for new object
    _torrentInfo = new libtorrent::torrent_info(*rhs.getTorrentInfo());

    _torrentPluginConfiguration = rhs.getTorrentPluginConfiguration();

    // Return self reference
    return *this;
}
*/

Controller::Torrent::Configuration::Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry) {
    //: _torrentInfo(libtorrent::sha1_hash()){

    /**
    // Check that info_hash is present
    if(dictionaryEntry.count("info_hash") == 1) {

        // Get entry
        libtorrent::entry infoHashEntry = dictionaryEntry.find("info_hash")->second;

        // Check that entry is of type entry::string_t
        if(infoHashEntry.type() == libtorrent::entry::string_t)
            _infoHash = libtorrent::sha1_hash(infoHashEntry.string()); // Why do we need to call constructor, why no conversion?
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "info_hash key is not of type entry::string_type.");

    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "info_hash key should have .count == 1.");

    // Check that name is present
    if(dictionaryEntry.count("name") == 1) {

        // Get entry
        libtorrent::entry nameEntry = dictionaryEntry.find("name")->second;

        // Check that entry is of type entry::string_t
        if(nameEntry.type() == libtorrent::entry::string_t)
            _name = nameEntry.string();
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "name key is not of type entry::string_type.");
    }
    else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "name key should have .count == 1.");

    // Check that save_path is present
    if(dictionaryEntry.count("save_path") == 1) {

        // Get entry
        libtorrent::entry savePathEntry = dictionaryEntry.find("save_path")->second;

        // Check that entry is of type entry::string_t
        if(savePathEntry.type() == libtorrent::entry::string_t)
            _savePath = savePathEntry.string();
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key is not of type entry::string_type.");
    }
    else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key should have .count == 1.");


    // Check that resume_data is present
    if(dictionaryEntry.count("resume_data") == 1) {

        // Get entry
        libtorrent::entry resumeDataEntry = dictionaryEntry.find("resume_data")->second;

        // Check that entry is of type entry::string_type
        if(resumeDataEntry.type() == libtorrent::entry::string_t) {

            // Use as entry::string_type
            libtorrent::entry::string_type resumeDataStringEntry = resumeDataEntry.string();

            // Populate resume_data_ vector
            for(std::string::const_iterator i = resumeDataStringEntry.begin(),
                    end(resumeDataStringEntry.end());i != end;i++)
                    _resumeData.push_back(*i);

        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key is not of type entry::string_type.");
    }
    else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key should have .count == 1.");

    // Check that flags is present
    if(dictionaryEntry.count("flags") == 1) {

        // Get entry
        libtorrent::entry flagsEntry = dictionaryEntry.find("flags")->second;

        // Check that entry is of type entry::int_t
        if(flagsEntry.type() == libtorrent::entry::int_t)
            _flags = flagsEntry.integer();
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "flags key is not of type entry::int_t.");
    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "flags key should have .count == 1.");

    // Check if torrentInfo is present
    // NOT IMPLEMENTED

    // Check if torrentPluginConfiguration is present
    if(dictionaryEntry.count("configuration") == 1) {

        // Get entry
        libtorrent::entry configurationEntry = dictionaryEntry.find("configuration")->second;

        // Check that entry is of type entry::dictionary_t
        if(configurationEntry.type() == libtorrent::entry::dictionary_t) {

            //_configuration = TorrentPlugin::Configuration(configurationEntry.dict());

            libtorrent::entry::dictionary_type configurationDictionaryEntry = configurationEntry.dict();

            // Figure out mode of plugin
            PluginMode mode;

            try {
                mode = TorrentPlugin::Configuration::pluginMode(configurationDictionaryEntry);
            } catch (InvalidBitSwaprStateEntryException & e) {

            }

            // Create corresponding
            switch(mode) {

                case PluginMode::Buyer:
                    _configuration = new BuyerTorrentPlugin::Configuration(configurationDictionaryEntry);
                    break;
                case PluginMode::Seller:
                    _configuration = new SellerTorrentPlugin::Configuration(configurationDictionaryEntry);
                    break;
                case PluginMode::Observer:
                    _configuration = NULL; // LATER
                    break;
            }


        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "configuration key is not of type entry::dictionary_t.");
    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "configuration key should have .count == 1.");

*/
}

void Controller::Torrent::Configuration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    /**
    // _infoHash
    dictionaryEntry["infoHash"] = libtorrent::entry::string_type(_infoHash.to_string());

    // _name
    dictionaryEntry["name"] = libtorrent::entry::string_type(_name);

    // _savePath
    dictionaryEntry["savePath"] = libtorrent::entry::string_type(_savePath);

    // _resumeData
    std::string resume_data_string;
    for(std::vector<char>::const_iterator i = _resumeData.begin(),
        end(_resumeData.end());i != end;i++)
            resume_data_string.append(&(*i));

    dictionaryEntry["resumeData"] = libtorrent::entry::string_type(resume_data_string);

    // _flags
    dictionaryEntry["flags"] = libtorrent::entry::integer_type(_flags);

    // _torrentInfo
    // NOT IMPLEMENTED
    //dictionaryEntry["torrentInfo"] = ;

    // _configuration
    libtorrent::entry::dictionary_type configurationDictionaryEntry;
    _configuration->toDictionaryEntry(configurationDictionaryEntry);
    dictionaryEntry["configuration"] = configurationDictionaryEntry;
    */
}

libtorrent::add_torrent_params Controller::Torrent::Configuration::toAddTorrentParams() const {

    // Create add_torrent_params for adding
    libtorrent::add_torrent_params params;

    params.info_hash = _infoHash;
    params.name = _name;
    params.save_path = _savePath;
    params.resume_data = _resumeData;// new std::vector<char>(_resumeData); // We do not own this pointer
    params.flags = _flags;
    params.ti = _torrentFile;

    //if(!_torrentInfo.info_hash().is_all_zeros())
    //    params.ti = boost::intrusive_ptr<libtorrent::torrent_info>(new libtorrent::torrent_info(_torrentInfo));

    //params.userdata = static_cast<void *>(_torrentPluginConfiguration);

    // Return parameters
    return params;
}

boost::intrusive_ptr<libtorrent::torrent_info> Controller::Torrent::Configuration::torrentFile() const {
    return _torrentFile;
}

/**
const TorrentPlugin::Configuration * Controller::Torrent::Configuration::torrentPluginConfiguration() const {
    return _torrentPluginConfiguration;
}

void Controller::Torrent::Configuration::setTorrentPluginConfiguration(const TorrentPlugin::Configuration *torrentPluginConfiguration) {
    _torrentPluginConfiguration = torrentPluginConfiguration;
}

libtorrent::torrent_info Controller::Torrent::Configuration::torrentInfo() const {
    return _torrentInfo;
}

void Controller::Torrent::Configuration::setTorrentInfo(libtorrent::torrent_info * torrentInfo) {
    _torrentInfo = torrentInfo;
}
*/

quint64 Controller::Torrent::Configuration::flags() const {
    return _flags;
}

void Controller::Torrent::Configuration::setFlags(quint64 flags) {
    _flags = flags;
}

std::vector<char> Controller::Torrent::Configuration::resumeData() const {
    return _resumeData;
}

void Controller::Torrent::Configuration::setResumeData(const std::vector<char> & resumeData) {
    _resumeData = resumeData;
}

std::string Controller::Torrent::Configuration::savePath() const {
    return _savePath;
}

void Controller::Torrent::Configuration::setSavePath(const std::string & savePath) {
    _savePath = savePath;
}

std::string Controller::Torrent::Configuration::name() const {
    return _name;
}

void Controller::Torrent::Configuration::setName(const std::string & name) {
    _name = name;
}

libtorrent::sha1_hash Controller::Torrent::Configuration::infoHash() const {
    return _infoHash;
}

void Controller::Torrent::Configuration::setInfoHash(const libtorrent::sha1_hash & infoHash) {
    _infoHash = infoHash;
}

/**
 * Controller::Torrent
 */

Controller::Torrent::Torrent(const libtorrent::sha1_hash & infoHash,
                             const std::string & name,
                             const std::string & savePath,
                             const std::vector<char> & resumeData,
                             quint64 flags,
                             //libtorrent::torrent_info * torrentInfo,
                             const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile,
                             Status event)
    : _infoHash(infoHash)
    , _name(name)
    , _savePath(savePath)
    , _resumeData(resumeData)
    , _flags(flags)
    //, _handle(handle)
    //, _torrentInfo(torrentInfo)
    , _status(event)
    , _pluginInstalled(PluginInstalled::None)
    , _model(infoHash,
             name,
             savePath,
             torrentFile) {
}

void Controller::Torrent::addPlugin(const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Seller;
    _model.addPlugin(status);
}

void Controller::Torrent::addPlugin(const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Buyer;
    _model.addPlugin(status);
}

libtorrent::sha1_hash Controller::Torrent::infoHash() const {
    return _infoHash;
}

std::string Controller::Torrent::name() const {
    return _name;
}

void Controller::Torrent::setName(const std::string & name) {
    _name = name;
}

std::string Controller::Torrent::savePath() const {
    return _savePath;
}

void Controller::Torrent::setSavePath(const std::string & savePath) {
    _savePath = savePath;
}

std::vector<char> Controller::Torrent::resumeData() const {
    return _resumeData;
}

void Controller::Torrent::setResumeData(const std::vector<char> & resumeData) {
    _resumeData = resumeData;
}

quint64 Controller::Torrent::flags() const {
    return _flags;
}

void Controller::Torrent::setFlags(quint64 flags) {
    _flags = flags;
}

/**
libtorrent::torrent_info * Controller::Torrent::torrentInfo() {
    return _torrentInfo;
}
*/

libtorrent::torrent_handle Controller::Torrent::handle() const {
    return _handle;
}

void Controller::Torrent::setHandle(const libtorrent::torrent_handle & handle) {
    _handle = handle;
}

Controller::Torrent::Status Controller::Torrent::status() const {
    return _status;
}

void Controller::Torrent::setStatus(Status event) {
    _status = event;
}

PluginInstalled Controller::Torrent::pluginInstalled() const {
    return _pluginInstalled;
}

TorrentViewModel * Controller::Torrent::model() {
    return &_model;
}

void Controller::Torrent::addStream(Stream * stream) {
    _streams.insert(stream);
}

void Controller::Torrent::removeStream(Stream * stream) {
    _streams.remove(stream);
}

void Controller::Torrent::pieceRead(const boost::shared_array<char> & buffer,
                                    int pieceIndex,
                                    int size) {

    // Iterate streams and notify them
    for(QSet<Stream *>::iterator i = _streams.begin(),
        end = _streams.end();
        i != end;i++)
        (*i)->pieceRead(buffer, pieceIndex, size);
}

void Controller::Torrent::pieceFinished(int piece) {

    // Iterate streams and notify them
    for(QSet<Stream *>::iterator i = _streams.begin(),
        end = _streams.end();
        i != end;i++)
        (*i)->pieceDownloaded(piece);
}

/**
 * Controller::Configuration
 */

#include <core/controller/exceptions/InvalidBitSwaprStateEntryException.hpp>
#include <libtorrent/session_settings.hpp>

Controller::Configuration::Configuration() {

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

Controller::Configuration::Configuration(const libtorrent::entry & libtorrentSessionSettingsEntry,
                                         const std::pair<int, int> & portRange,
                                         const std::vector<std::pair<std::string, int>> & dhtRouters,
                                         const QVector<Torrent::Configuration> & torrents)
                                :_libtorrentSessionSettingsEntry(libtorrentSessionSettingsEntry)
                                ,_portRange(portRange)
                                ,_dhtRouters(dhtRouters)
                                ,_torrents(torrents) {
}

Controller::Configuration::~Configuration() {
/**
    // Delete torrent configuration
    for(std::vector<TorrentConfiguration *>::iterator i = _torrentConfigurations.begin(),
            end(_torrentConfigurations.end()); i != end;i++)
            delete *i;
*/
}

Controller::Configuration::Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry) {

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

Controller::Configuration::Configuration(const char * fileName) {

    /*
    // Create dictionary entry
    libtorrent::entry controllerConfigurationEntry;

    // Save bencoded dictionary to file
    Utilities::loadBencodedEntry(fileName, controllerConfigurationEntry);

    // Convert to dictionary entry
    libtorrent::entry::dictionary_type controllerConfigurationDictionaryEntry = controllerConfigurationEntry.dict();

    // Use other constructor using this dictionary
    ControllerConfiguration::ControllerConfiguration(controllerConfigurationDictionaryEntry);
    */
}

void Controller::Configuration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) {

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

void Controller::Configuration::saveToFile(const char * fileName) {

    /*
    // Create dictionary entry
    libtorrent::entry::dictionary_type controllerConfigurationDictionaryEntry;

    // Save controller configururation in entry
    toDictionaryEntry(controllerConfigurationDictionaryEntry);

    // Save bencoded dictionary to file
    Utilities::saveBencodedEntry(fileName, controllerConfigurationDictionaryEntry);
    */
}

/*
void ControllerConfiguration::insertTorrentConfiguration(const TorrentConfiguration * torrentConfiguration) {
    _torrentConfigurations.push_back(torrentConfiguration);
}
*/

libtorrent::entry Controller::Configuration::getLibtorrentSessionSettingsEntry() const {
    return _libtorrentSessionSettingsEntry;
}

std::pair<int, int> Controller::Configuration::getPortRange() const {
    return _portRange;
}

std::vector<std::pair<std::string, int>> Controller::Configuration::getDhtRouters() const {
    return _dhtRouters;
}

QVector<Controller::Torrent::Configuration> Controller::Configuration::torrents() const {
    return _torrents;
}

void Controller::Configuration::setTorrents(const QVector<Torrent::Configuration> & torrents) {
    _torrents = torrents;
}

/*
QMap<libtorrent::sha1_hash, QPair<Controller::Torrent::Configuration, SellerTorrentPlugin::Configuration> > Controller::Configuration::sellers() const {
    return _sellers;
}

void Controller::Configuration::setSellers(const QMap<libtorrent::sha1_hash, QPair<Controller::Torrent::Configuration, SellerTorrentPlugin::Configuration> > &sellers) {
    _sellers = sellers;
}

QMap<libtorrent::sha1_hash, QPair<Controller::Torrent::Configuration, BuyerTorrentPlugin::Configuration> > Controller::Configuration::buyers() const {
    return _buyers;
}

void Controller::Configuration::setBuyers(const QMap<libtorrent::sha1_hash, QPair<Torrent::Configuration, BuyerTorrentPlugin::Configuration> > &buyers) {
    _buyers = buyers;
}
*/

/*
std::vector<TorrentConfiguration *>::const_iterator ControllerConfiguration::getBeginTorrentConfigurationsIterator() const {
    return _torrentConfigurations.begin();
}

std::vector<TorrentConfiguration *>::const_iterator ControllerConfiguration::getEndTorrentConfigurationsIterator() const {
    return _torrentConfigurations.end();
}
*/

void Controller::Configuration::setLibtorrentSessionSettingsEntry(const libtorrent::entry & libtorrentSessionSettingsEntry) {
    _libtorrentSessionSettingsEntry = libtorrentSessionSettingsEntry;
}

/**
 * Controller
 */

#include <core/controller/exceptions/ListenOnException.hpp>
#include <core/controller/Stream.hpp>
#include <core/extension/Alert/StartedSellerTorrentPlugin.hpp>
#include <core/extension/Alert/StartedBuyerTorrentPlugin.hpp>
#include <core/extension/Alert/BuyerTorrentPluginStatusAlert.hpp>
#include <core/extension/Alert/SellerTorrentPluginStatusAlert.hpp>
#include <core/extension/Alert/PluginStatusAlert.hpp>
#include <core/extension/Alert/SellerPeerAddedAlert.hpp>
#include <core/extension/Alert/BuyerPeerAddedAlert.hpp>
#include <core/extension/Alert/SellerPeerPluginRemovedAlert.hpp>
#include <core/extension/Alert/BuyerPeerPluginRemovedAlert.hpp>
#include <core/extension/Request/StartSellerTorrentPlugin.hpp>
#include <core/extension/Request/StartBuyerTorrentPlugin.hpp>
#include <core/extension/Request/StartObserverTorrentPlugin.hpp>
#include <core/extension/Request/ChangeDownloadLocation.hpp>
#include <wallet/Manager.hpp>

#include <libtorrent/alert_types.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/peer_connection.hpp>

#include <QObject>
#include <QElapsedTimer>
#include <QtGlobal>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QThread>
#include <QNetworkAccessManager>

#ifndef Q_MOC_RUN
#include <boost/bind.hpp>
#endif Q_MOC_RUN

// Register types for signal and slots: LATER CHECK WHICH ONE OF THESE ARE ACTUALLY REQUIRED
Q_DECLARE_METATYPE(libtorrent::sha1_hash)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(libtorrent::error_code)
Q_DECLARE_METATYPE(std::vector<libtorrent::torrent_status>)
Q_DECLARE_METATYPE(libtorrent::torrent_status)

Q_DECLARE_METATYPE(Coin::Transaction) // Probably should not be here

// Register type for QMetaObject::invokeMethod
Q_DECLARE_METATYPE(const libtorrent::alert*)

Controller::Controller(const Configuration & configuration, Wallet::Manager * wallet, QNetworkAccessManager * manager, QLoggingCategory & category)
    : _state(State::normal)
    , _session(new libtorrent::session(libtorrent::fingerprint(CORE_EXTENSION_FINGERPRINT, CORE_VERSION_MAJOR, CORE_VERSION_MINOR, 0, 0),
                   libtorrent::session::add_default_plugins + libtorrent::session::start_default_features,
                   libtorrent::alert::error_notification +
                   libtorrent::alert::tracker_notification +
                   libtorrent::alert::debug_notification +
                   libtorrent::alert::status_notification +
                   libtorrent::alert::progress_notification +
                   libtorrent::alert::performance_warning +
                   libtorrent::alert::stats_notification))
    , _wallet(wallet) // add autosave to configuration later?? does user even need to control that?
    , _category(category)
    , _manager(manager)
    , _plugin(new Plugin(wallet, _category))
    , _portRange(configuration.getPortRange()) {
    //, _server(9999, this) {

    qCDebug(_category) << "Libtorrent session started on port" << QString::number(_session->listen_port());

    // Register types for signal and slots
    qRegisterMetaType<libtorrent::sha1_hash>();
    qRegisterMetaType<std::string>();
    qRegisterMetaType<libtorrent::error_code>();
    qRegisterMetaType<std::vector<libtorrent::torrent_status>>();
    qRegisterMetaType<libtorrent::torrent_status>();
    qRegisterMetaType<Coin::Transaction>(); // Probably should not be here

    // Register type for QMetaObject::invokeMethod
    qRegisterMetaType<const libtorrent::alert*>();

    // Set libtorrent to call processAlert when alert is created
    _session->set_alert_dispatch(boost::bind(&Controller::libtorrent_alert_dispatcher_callback, this, _1));

    /**
    // Connect streaming server signals
    QObject::connect(&_server,
                     SIGNAL(streamStarted(const Stream*)),
                     this,
                     SLOT(registerStream(const Stream*)));

    QObject::connect(&_server,
                     SIGNAL(streamCreationError(QAbstractSocket::SocketError socketError)),
                     this,
                     SLOT(handleFailedStreamCreation(QAbstractSocket::SocketError socketError)));
    */

    // Connect server signals to corresponding controller slots
    QObject::connect(&_streamingServer,
                     SIGNAL(newConnection()),
                     this,
                     SLOT(handleConnection()));

    QObject::connect(&_streamingServer,
                     SIGNAL(acceptError(QAbstractSocket::SocketError)),
                     this,
                     SLOT(handleAcceptError(QAbstractSocket::SocketError)));

    // Start listening
    bool success = _streamingServer.listen(QHostAddress::Any); // auto selects port by removing port arg

    if(success)
        qCDebug(_category) << "Started streaming server on port:" << _streamingServer.serverPort();
    else
        qCDebug(_category) << "Could not start streaming server on port:" << _streamingServer.serverPort();

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

    // Create session, which immediately starts server
    // ======================================
    _session = new libtorrent::session(settings);
    */

    qCDebug(_category) << "Libtorrent session started";

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

    // Add plugin extension
    _session->add_extension(boost::shared_ptr<libtorrent::plugin>(_plugin));

    // Start timer which calls session.post_torrent_updates at regular intervals
    _statusUpdateTimer.setInterval(CORE_CONTROLLER_POST_TORRENT_UPDATES_DELAY);

    QObject::connect(&_statusUpdateTimer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(callPostTorrentUpdates()));

    _statusUpdateTimer.start();

    // Add all torrents, but this ust be AFTER session.listen_on(),
    // because otherwise adding to session won't work.
    QVector<Torrent::Configuration> torrents = configuration.torrents();

    for(QVector<Torrent::Configuration>::const_iterator i = torrents.begin(),
            end(torrents.end());i != end; ++i) {

        /**
        // Try to add torrent, without prompting user
        if(!addTorrent(*i, false)) {
            qCCritical(_category) << "Unable to add torrent configuration to session";
            return;
        }
        */
    }
}

Controller::~Controller() {

    // Delete all torrents
    for(QMap<libtorrent::sha1_hash, Torrent *>::const_iterator
        i = _torrents.constBegin(),
        end = _torrents.constEnd();
        i != end;i++)
        delete i.value();

    // Delete session
    delete _session;
}

void Controller::callPostTorrentUpdates() {
    _session->post_torrent_updates();
}

void Controller::handleConnection() {

    // Create handler for each pending connection
    // socket is owned by _server
    while(QTcpSocket * socket = _streamingServer.nextPendingConnection()) {

        qDebug(_category) << "New connection opened.";

        // Create stream
        Stream * stream = new Stream(socket, this);
    }
}

void Controller::handleAcceptError(QAbstractSocket::SocketError socketError) {

    qDebug(_category) << "Failed to accept connection.";
}

/**
void Controller::registerStream(const Stream * handler) {

    // Make sure to handle stream path announcement signal synchronously
    QObject::connect(handler,
                     SIGNAL(requestedPathAnnounced(const Stream*,QByteArray)),
                     this,
                     SLOT(registerRequestedPathOnStream(const Stream*,QByteArray)),
                     Qt::DirectConnection);
}

void Controller::handleFailedStreamCreation(QAbstractSocket::SocketError socketError) {
    qCDebug(_category) << "handleFailedStreamCreation";
}

void Controller::registerRequestedPathOnStream(const Stream * stream, const QByteArray & requestedPath) const {

    // Turn into info hash
    libtorrent::sha1_hash infoHash(requestedPath.toStdString());

    if(_torrents.contains(infoHash)) {

        const TorrentViewModel * viewModel = _torrents[infoHash]->model();

        // Connect stream signals to view model slots
        QObject::connect(stream,
                         SIGNAL(rangeRequested(int, int)),
                         viewModel,
                         SLOT(getRange(int,int)));

        QObject::connect(stream,
                         SIGNAL(startRequested(int)),
                         viewModel,
                         SLOT(getStart(int)));

        QObject::connect(stream,
                         SIGNAL(errorOccured(Stream::Error)),
                         viewModel,
                         SLOT(errorOccured(Stream::Error)));

        // Connect view model signals to stream slots
        QObject::connect(viewModel,
                         SIGNAL(dataRangeRead(QString,int,int,int,QVector<Stream::Piece>,int,int)),
                         stream,
                         SLOT(sendDataRange(QString,int,int,int,QVector<Stream::Piece>,int,int)));

        QObject::connect(viewModel,
                         SIGNAL(receivedInvalidRange(int)),
                         stream,
                         SLOT(invalidRangeRequested(int)));


    } else {

        qCDebug(_category) << "Requested path does not correspond to any presently active torrent.";

        Q_ASSERT(false); // <== no clean way to deal with this, as this slot is called synchronously
    }
}

void Controller::readPiece(int piece) {
    //_session->

    _session->get_torrents()

    // We must ask plugin sto download from a givne position in response
    // to a range reset

    // and the nonly read pieces ones we actually know they have been
    // downloaded...
}
*/

/*
void Controller::addPeerPlugin(libtorrent::sha1_hash info_hash, libtorrent::tcp::endpoint endPoint) {
    view.addPeerPlugin(info_hash, endPoint);
}

void Controller::updatePeerPluginStatus(PeerPluginStatus status) {
    view.updatePeerPluginStatus(status);
}

void Controller::removePeerPlugin(libtorrent::sha1_hash info_hash, libtorrent::tcp::endpoint endPoint) {
    view.removePeerPlugin(info_hash, endPoint);
}
*/

void Controller::libtorrent_alert_dispatcher_callback(const std::auto_ptr<libtorrent::alert> & alertAutoPtr) {

    // Grab alert pointer and release the auto pointer, this way the alert is not automatically
    // deleted when alertAutoPtr goes out of scope.
    // **Registering auto_ptr with MOC is not worth trying**
    //const libtorrent::alert * a = alertAutoPtr.release();

    const libtorrent::alert * a = alertAutoPtr.get();

    std::auto_ptr<libtorrent::alert> stdAutoPtr = a->clone();

    libtorrent::alert * freedPointer = stdAutoPtr.release();

    // Tell bitswapr thread to run processAlert later with given alert as argument
    QMetaObject::invokeMethod(this, "processAlert", Q_ARG(const libtorrent::alert*, freedPointer));
}

void Controller::libtorrent_entry_point_alert_notification() {
    QMetaObject::invokeMethod(this, "processAlertQueue");
}

/**
void Controller::processAlertQueue() {

    // Populate vector with alerts
    std::vector<libtorrent::alert *> alerts;
    _session->pop_alerts(&alerts);

    // NOTE on owernship of alerts (alert.hpp)
    // Alerts returned by pop_alerts() are only valid until the next call to
    // pop_alerts(). You may not copy an alert object to access it after the next
    // call to pop_alerts(). Internal members of alerts also become invalid once
    // pop_alerts() is called again.

    for(std::vector<libtorrent::alert *>::const_iterator i = alerts.begin(),
        end = alerts.end();
        i != end;
        i++) {

        // Get alert
        libtorrent::alert * alert = *i;

        qCDebug(_category) << "Processing alert" << QString::fromStdString(alert->message());

        // Process
        processAlert(alert);
    }

}
*/

void Controller::processAlert(const libtorrent::alert * a) {

    // Check that alert has been stuck in event queue and corresponds to recenty
    // removed torrent.

    // if(something)
    //    something;

    // In each case, tell bitswapr thread to run the given method
    /**
     * Alerts from libtorrent core
     */
    if(libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a))
        processMetadataReceivedAlert(p);
    else if(libtorrent::metadata_failed_alert const * p = libtorrent::alert_cast<libtorrent::metadata_failed_alert>(a))
        processMetadataFailedAlert(p);
    else if(libtorrent::listen_failed_alert const * p = libtorrent::alert_cast<libtorrent::listen_failed_alert>(a))
        processListenFailedAlert(p);
    else if(libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a))
        processAddTorrentAlert(p);
    else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a))
        processTorrentFinishedAlert(p);
    else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a))
        processTorrentPausedAlert(p);
    else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a))
        processStatusUpdateAlert(p);
    else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a))
        processTorrentRemovedAlert(p);
    else if(libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a))
        processSaveResumeDataAlert(p);
    else if(libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a))
        processSaveResumeDataFailedAlert(p);
    else if(libtorrent::torrent_checked_alert const * p = libtorrent::alert_cast<libtorrent::torrent_checked_alert>(a))
        processTorrentCheckedAlert(p);
    else if(libtorrent::read_piece_alert const * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a))
        processReadPieceAlert(p);
    else if(libtorrent::piece_finished_alert const * p = libtorrent::alert_cast<libtorrent::piece_finished_alert>(a))
        processPieceFinishedAlert(p);
    else if(PluginStatusAlert const * p = libtorrent::alert_cast<PluginStatusAlert>(a))
        processPluginStatusAlert(p);
    /**
     * Alerts from plugin
     */
    else if(const StartedSellerTorrentPlugin * p = libtorrent::alert_cast<StartedSellerTorrentPlugin>(a))
        processStartedSellerTorrentPlugin(p);
    else if(const StartedBuyerTorrentPlugin * p = libtorrent::alert_cast<StartedBuyerTorrentPlugin>(a))
        processStartedBuyerTorrentPlugin(p);
    else if(const SellerTorrentPluginStatusAlert * p = libtorrent::alert_cast<SellerTorrentPluginStatusAlert>(a))
        processSellerTorrentPluginStatusAlert(p);
    else if(const BuyerTorrentPluginStatusAlert * p = libtorrent::alert_cast<BuyerTorrentPluginStatusAlert>(a))
        processBuyerTorrentPluginStatusAlert(p);
    else if(const SellerPeerAddedAlert * p = libtorrent::alert_cast<SellerPeerAddedAlert>(a))
        processSellerPeerAddedAlert(p);
    else if(const BuyerPeerAddedAlert * p = libtorrent::alert_cast<BuyerPeerAddedAlert>(a))
        processBuyerPeerAddedAlert(p);
    else if(const SellerPeerPluginRemovedAlert * p = libtorrent::alert_cast<SellerPeerPluginRemovedAlert>(a))
        processSellerPeerPluginRemovedAlert(p);
    else if(const BuyerPeerPluginRemovedAlert * p = libtorrent::alert_cast<BuyerPeerPluginRemovedAlert>(a))
        processBuyerPeerPluginRemovedAlert(p);
    //else if(const TorrentPluginStartedAlert * p = libtorrent::alert_cast<TorrentPluginStartedAlert>(a))
    //    processTorrentPluginStartedAlert(p);

    // Delete alert ** DELETE WHEN GOING TO NEW RELEASE ***
    // ===============================================
    delete a;
}

void Controller::sellerPeerPluginRemoved(const libtorrent::sha1_hash & infoHash, const libtorrent::tcp::endpoint & endPoint) {

}

/**
// NB!: Alreayd assumes session is paused and does not check sourceForLastResumeDataCall and numberOfOutstandingResumeDataCalls before starting,
// assumes they equal NONE,0 respectively.
*/

int Controller::makeResumeDataCallsForAllTorrents() {

    // Get all handles (should be same torrents as int addTorrentParameters, but we dont check this here
    std::vector<libtorrent::torrent_handle> handles = _session->get_torrents();

    // Keeps track of how many calls were made
    int resumeCallsMade = 0;

    // Iterate all torrents, and try to save
    for (std::vector<libtorrent::torrent_handle>::iterator i = handles.begin(),
         end(handles.end());i != end; ++i) {

        // Get handle
        libtorrent::torrent_handle & h = *i;

        // Get torrent info hash
        libtorrent::sha1_hash infoHash = h.info_hash();

        Q_ASSERT(_torrents.contains(infoHash));

        // Grab torrent;
        Torrent * torrent = _torrents[infoHash];

        // Dont save data if
        if (torrent->status() != Torrent::Status::nothing // are in wrong state
            || !h.is_valid() // dont have valid handle
            || !h.need_save_resume_data() // dont need to
            || !h.status().has_metadata) // or dont have metadata
            continue;

        // Save resume data
        h.save_resume_data();

        // Count call
        resumeCallsMade++;

        // Change expected event of torrent
        _torrents[infoHash]->setStatus(Torrent::Status::asked_for_resume_data);
    }

    return resumeCallsMade;
}

void Controller::processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p) {

    // Get handle
    libtorrent::torrent_handle torrentHandle = p->handle;

    // Dont save data if we dont need to or can
    if (!torrentHandle.need_save_resume_data() || !torrentHandle.status().has_metadata)
        return;

    // Set state
    //_sourceForLastResumeDataCall = TORRENT_PAUSE;
    //_numberOfOutstandingResumeDataCalls = 1;

    // Save resume data
    torrentHandle.save_resume_data();

    // Get info hash
    libtorrent::sha1_hash info_hash = torrentHandle.info_hash();

    // Grab torrent and set next event to be arrival of save_resume_data alert
    Q_ASSERT(_torrents.contains(info_hash));

    _torrents[info_hash]->setStatus(Torrent::Status::asked_for_resume_data);
}

void Controller::processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p) {

    /*
     * NOTICE: Docs say p->handle may be invalid at this time,
     * so we must use p->info_hash instead.
     */

    // Get torrent info hash
    libtorrent::sha1_hash info_hash = p->info_hash;

    // Remove from view
    torrentRemoved(info_hash);

    qCDebug(_category) << "Found match and removed it.";
}

void Controller::processMetadataReceivedAlert(libtorrent::metadata_received_alert const * p) {

    // Get handle for torrent
    libtorrent::torrent_handle h = p->handle;

    // Process if handle is valid
    if (h.is_valid()) {

        // get torrent info
        //boost::shared_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();
        boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();

        //const libtorrent::torrent_info & torrentInfo = h.get_torrent_info();

        // USE THIS INFORMATION FOR SOMETHING LATER

        // Put in model later
        qCDebug(_category) << "Metadata saved.";

    } else
        qCDebug(_category) << "Invalid handle for received metadata.";
}

void Controller::processMetadataFailedAlert(libtorrent::metadata_failed_alert const * p) {
    // WHAT DO WE DO HERE?
    qCDebug(_category) << "Invalid metadata received.";
    throw std::runtime_error("Invalid metadata");
}

void Controller::processListenFailedAlert(libtorrent::listen_failed_alert const * p) {
    throw std::runtime_error("Failed to start listening"); // p->listen_interface()
}

void Controller::processAddTorrentAlert(libtorrent::add_torrent_alert const * p) {

    Q_ASSERT(_state == State::normal);
    Q_ASSERT(_torrents.contains(p->params.info_hash));

    // Get torrent
    Torrent * torrent = _torrents[p->params.info_hash];

    Q_ASSERT(torrent->status() == Torrent::Status::being_async_added_to_session);

    // Check if there was an error
    if (p->error) {

        qCDebug(_category) << "Adding torrent failed, must be removed.";

        /**
         * Remove torrent here, so that it does not hanga around and cause problems.
         */

        // old name arg: p->params.ti.get() != 0 ? p->params.ti->name() : name = p->params.name
        emit failedToAddTorrent(p->params.name, p->params.info_hash, p->error);

    } else {

        qCDebug(_category) << "Adding torrent succeeded.";

        /*
		h.set_max_connections(max_connections_per_torrent);
		h.set_max_uploads(-1);
		h.set_upload_limit(torrent_upload_limit);
		h.set_download_limit(torrent_download_limit);
		h.use_interface(outgoing_interface.c_str());
        */

        // Give copy of handle
        torrent->setHandle(p->handle);

        // Update expected event on torrent
        torrent->setStatus(Torrent::Status::torrent_checked);

        // Send notification signal
        emit addedTorrent(torrent->model());
	}
}

void Controller::processTorrentFinishedAlert(libtorrent::torrent_finished_alert const * p) {

    /*
    p->handle.set_max_connections(max_connections_per_torrent / 2);

    // write resume data for the finished torrent
    // the alert handler for save_resume_data_alert
    // will save it to disk
    torrent_handle h = p->handle;
    h.save_resume_data();
    ++num_outstanding_resume_data;
    */
}

void Controller::processStatusUpdateAlert(libtorrent::state_update_alert const * p) {
    update(p->status);
}

void Controller::processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p) {

    // Get info hash for torrent
    libtorrent::sha1_hash info_hash = p->handle.info_hash();

    // Grab torrent
    Q_ASSERT(_torrents.contains(info_hash));
    Torrent * torrent = _torrents[info_hash];

    // Check that alert was expected
    Q_ASSERT(torrent->status() == Torrent::Status::asked_for_resume_data);

    // Reset expected event
    torrent->setStatus(Torrent::Status::nothing);

    // Create resume data buffer
    std::vector<char> resumeData;

    // Write new content to it
    bencode(std::back_inserter(resumeData), *(p->resume_data));

    // Save resume data in torrent
    torrent->setResumeData(resumeData);

    // If this is part of closing client, then
    if(_state == State::waiting_for_resume_data_while_closing) {

        // Check if all there are any pending resume data requests
        for(QMap<libtorrent::sha1_hash, Torrent *>::const_iterator
            i = _torrents.begin(),
            end = _torrents.end(); i != end;i++) {

            // End processing if alert is expected
            if((i.value())->status() == Torrent::Status::asked_for_resume_data)
                return;
        }

        // Close client
        finalize_close();
    }
}

void Controller::processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p) {

    qCCritical(_category) << "Failed to generate resume data for some reason.";
}

void Controller::processTorrentCheckedAlert(libtorrent::torrent_checked_alert const * p) {

    // Get handle for torrent
    libtorrent::torrent_handle h = p->handle;

    // Process if handle is valid
    if(h.is_valid()) {

        // Get info hash of torrent
        libtorrent::sha1_hash infoHash = h.info_hash();

        //qCCritical(_category) << QString::fromStdString(h.info_hash().to_string()); // infoHash.to_string()
        //qCCritical(_category) << QString::fromStdString(_torrents.first()->infoHash().to_string());

        // Make sure the torrent exists
        Q_ASSERT(_torrents.contains(infoHash));

        // Grab torrent
        Torrent * torrent = _torrents[infoHash];

        // Assert that torrent_checked_alert was expected
        Q_ASSERT(torrent->status() == Torrent::Status::torrent_checked);

        /**
        // if a configuration was saved, i.e. started from disk, then we just use it
        if(_pendingConfigurations.contains(info_hash)) {

            // Remove torrent plugin configuration
            const TorrentPlugin::Configuration * configuration = _pendingConfigurations.take(info_hash);

            // Send configuration to plugin
            _plugin->submitPluginRequest(new StartTorrentPlugin(info_hash, configuration));

            // Reset event
            torrent.setEvent(Torrent::ExpectedEvent::nothing);

        } */

        // if a configuration was saved, i.e. started from disk, then we just use it
        if(_pendingSellerTorrentPluginConfigurations.contains(infoHash)) {

            // Remove torrent plugin configuration
            const SellerTorrentPlugin::Configuration configuration = _pendingSellerTorrentPluginConfigurations.take(infoHash);

            // Send configuration to plugin
            _plugin->submitPluginRequest(new StartSellerTorrentPlugin(infoHash, configuration));

            // Reset event
            torrent->setStatus(Torrent::Status::nothing);

        } else if(_pendingBuyerTorrentPluginConfigurationAndUtxos.contains(infoHash)) {

            // Remove torrent plugin configuration and utxo
            QPair<BuyerTorrentPlugin::Configuration, Coin::UnspentP2PKHOutput> p = _pendingBuyerTorrentPluginConfigurationAndUtxos.take(infoHash);

            // Send configuration to plugin
            _plugin->submitPluginRequest(new StartBuyerTorrentPlugin(infoHash, p.first, p.second));

            // Reset event
            torrent->setStatus(Torrent::Status::nothing);

        } /** else if(_pendingObserverTorrentPluginConfigurations.contains(info_hash)) {

            // Remove torrent plugin configuration
            const ObserverTorrentPlugin::Configuration configuration = _pendingObserverTorrentPluginConfigurations.take(info_hash);

            // Send configuration to plugin
            _plugin->submitPluginRequest(new StartObserverTorrentPlugin(info_hash, configuration));

            // Reset event
            torrent.setEvent(Torrent::ExpectedEvent::nothing);

        }*/ else {

            // Get torrent information
            //boost::shared_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();
            boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();
            const libtorrent::torrent_info * torrentInfoPtr = torrentInfoIntrusivePtr.get();
            libtorrent::torrent_info torrentInfo = *torrentInfoPtr; //h.get_torrent_info();

            // Get torrent status
            libtorrent::torrent_status torrentStatus = h.status();

            // No longer used: Expect user to set configurations
            //torrent->setStatus(Torrent::Status::torrent_plugin_configuration_from_user);

            // Send signal
            emit torrentCheckedButHasNoPlugin(torrentInfo, torrentStatus);
        }

    } else {

        qCDebug(_category) << "Invalid handle for checked torrent.";
        Q_ASSERT(false);
    }
}

void Controller::processReadPieceAlert(const libtorrent::read_piece_alert * p) {

    // Get info hash for torrent from which this read piece comes from
    const libtorrent::sha1_hash infoHash = p->handle.info_hash();

    Q_ASSERT(_torrents.contains(infoHash));

    /**
    // Notify torrent view model
    _torrents[infoHash]->model()->pieceRead(p->ec,
                                            p->buffer,
                                            p->piece,
                                            p->size);
    */

    if(p->ec) {
        qCDebug(_category) << "There was some sort of error in reading a piece: " << QString::fromStdString(p->ec.message());
    } else {

        // Notify torrent
        _torrents[infoHash]->pieceRead(p->buffer, p->piece, p->size);
    }
}

void Controller::processPieceFinishedAlert(const libtorrent::piece_finished_alert * p) {

    // Get info hash for torrent from which this read piece comes from
    const libtorrent::sha1_hash infoHash = p->handle.info_hash();

    Q_ASSERT(_torrents.contains(infoHash));

    // Notify torrent
    _torrents[infoHash]->pieceFinished(p->piece_index);
}

void Controller::processStartedSellerTorrentPlugin(const StartedSellerTorrentPlugin * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    // Get torrent
    Torrent * torrent = _torrents[p->infoHash()];

    Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::None);

    // Update information about plugin installed on torrent
    torrent->addPlugin(p->status());

    // Send signal
    // *** DISCONTINUED, THESE ARE INSTALLED AS SIGNALS ON TORRENT VIEW MODELS
    //emit startedSellerTorrentPlugin(torrent->model()->sellerTorrentPluginViewModel());
}

void Controller::processStartedBuyerTorrentPlugin(const StartedBuyerTorrentPlugin * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    Torrent * torrent = _torrents[p->infoHash()];

    Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::None);

    // Update information about plugin installed on torrent
    torrent->addPlugin(p->status());

    // Send signal
    // *** DISCONTINUED, THESE ARE INSTALLED AS SIGNALS ON TORRENT VIEW MODELS
    //emit startedBuyerTorrentPlugin(torrent->model()->buyerTorrentPluginViewModel());
}

void Controller::processBuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    // Get view model for torrent
    TorrentViewModel * model = _torrents[p->infoHash()]->model();

    Q_ASSERT(model->pluginInstalled() == PluginInstalled::Buyer);

    // Update
    model->update(p->status());
}

void Controller::processSellerTorrentPluginStatusAlert(const SellerTorrentPluginStatusAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    // Get view model for torrent
    TorrentViewModel * model = _torrents[p->infoHash()]->model();

    Q_ASSERT(model->pluginInstalled() == PluginInstalled::Seller);

    // Update
    model->update(p->status());
}

void Controller::processPluginStatusAlert(const PluginStatusAlert * p) {

    //
    emit pluginStatusUpdate(p->status());
}

void Controller::processSellerPeerAddedAlert(const SellerPeerAddedAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    Torrent * torrent = _torrents[p->infoHash()];

    torrent->model()->addPeer(p->endPoint(), p->status());
}

void Controller::processBuyerPeerAddedAlert(const BuyerPeerAddedAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    Torrent * torrent = _torrents[p->infoHash()];

    torrent->model()->addPeer(p->endPoint(), p->status());
}

void Controller::processSellerPeerPluginRemovedAlert(const SellerPeerPluginRemovedAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    // Grab torrent
    Torrent * torrent = _torrents[p->infoHash()];

    Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::Seller);

    // Notify view model
    torrent->model()->removePeer(p->endPoint());
}

void Controller::processBuyerPeerPluginRemovedAlert(const BuyerPeerPluginRemovedAlert * p) {

    Q_ASSERT(_torrents.contains(p->infoHash()));

    // Grab torrent
    Torrent * torrent = _torrents[p->infoHash()];

    Q_ASSERT(torrent->pluginInstalled() == PluginInstalled::Buyer);

    // Notify view model
    torrent->model()->removePeer(p->endPoint());
}

void Controller::update(const std::vector<libtorrent::torrent_status> & statuses) {

    for(std::vector<libtorrent::torrent_status>::const_iterator
        i = statuses.begin(),
        end = statuses.end(); i != end;i++)
        update(*i);
}

void Controller::update(const libtorrent::torrent_status & status) {

    Q_ASSERT(_torrents.contains(status.info_hash));

    // Get view model
    TorrentViewModel * model = _torrents[status.info_hash]->model();

    // Update status
    model->update(status);
}

void Controller::removeTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle torrentHandle = _session->find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        Q_ASSERT(false);

    // Remove from session
    // Session will send us torrent_removed_alert alert when torrent has been removed
    // at which point we can remove torrent from model in alert handler
    _session->remove_torrent(torrentHandle);
}

void Controller::pauseTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle torrentHandle = _session->find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        Q_ASSERT(false);

    // Turn off auto managing
    torrentHandle.auto_managed(false);

    // Pause
    // We save resume data when the pause alert is issued by libtorrent
    torrentHandle.pause(libtorrent::torrent_handle::graceful_pause);
}

void Controller::startTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle torrentHandle = _session->find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        Q_ASSERT(false);

    // Turn on auto managing
    torrentHandle.auto_managed(true);

    // Start
    torrentHandle.resume();
}

bool Controller::addTorrent(const Torrent::Configuration & configuration) {
                            //, bool promptUserForTorrentPluginConfiguration) {

    // Convert to add torrent parameters
    libtorrent::add_torrent_params params = configuration.toAddTorrentParams();

    // Create save_path on disk, return if it does not exist
    if(!(QDir()).mkpath(params.save_path.c_str())) {
        qCCritical(_category) << "Could not create save_path: " << params.save_path.c_str();
        return false;
    }

    // Get info hash
    libtorrent::sha1_hash info_hash = configuration.infoHash();

    // Create torrent
    Torrent * torrent = new Torrent(info_hash,
                                    configuration.name(),
                                    configuration.name(),
                                    configuration.resumeData(),
                                    configuration.flags(),
                                    configuration.torrentFile(), //.torrentInfo(),
                                    //libtorrent::torrent_handle(), // proper handle is set when torrent has been added
                                    Torrent::Status::being_async_added_to_session);

    // Warn user if torrent has already been added
    if(_torrents.contains(info_hash)) {
        qCCritical(_category) << "Torrent with given info_hash has already been added.";
        return false;
    }

    // !_torrents.contains(info_hash) =>
    Q_ASSERT(!_pendingSellerTorrentPluginConfigurations.contains(info_hash));
    Q_ASSERT(!_pendingBuyerTorrentPluginConfigurationAndUtxos.contains(info_hash));
    //Q_ASSERT(!_pendingObserverTorrentPluginConfigurations.contains(info_hash));

    /**
     * Setup signals and slots
     */

    // Connect view model signals to controller slots
    const TorrentViewModel * viewModel = torrent->model();

    QObject::connect(viewModel,
                     SIGNAL(pause(libtorrent::sha1_hash)),
                     this,
                     SLOT(pauseTorrent(libtorrent::sha1_hash)));

    QObject::connect(viewModel,
                     SIGNAL(start(libtorrent::sha1_hash)),
                     this,
                     SLOT(startTorrent(libtorrent::sha1_hash)));

    QObject::connect(viewModel,
                     SIGNAL(remove(libtorrent::sha1_hash)),
                     this,
                     SLOT(removeTorrent(libtorrent::sha1_hash)));

    // NOTE:
    // The remaining signals model
    // * pluginInstalledChanged(PluginInstalled pluginInstalled)
    // * torrentStatusChanged(const libtorrent::torrent_status & status)
    // should be wired up by controller users upon
    // capturing addedTorrent(const TorrentViewModel *) signal.

    // Save torrent
    _torrents[info_hash] = torrent;

    // Add torrent to session
    _session->async_add_torrent(params);

    // Indicate that we added to session
    return true;
}

bool Controller::addTorrent(const Torrent::Configuration & configuration, const SellerTorrentPlugin::Configuration & pluginConfiguration) {

    // Try to add torrent
    if(!addTorrent(configuration))
        return false;

    // Save plugin configuration
    _pendingSellerTorrentPluginConfigurations[configuration.infoHash()] = pluginConfiguration;

    return true;
}

bool Controller::addTorrent(const Torrent::Configuration & configuration, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo) {

    // Try to add torrent
    if(!addTorrent(configuration))
        return false;

    // Save plugin configuration and utxo
    _pendingBuyerTorrentPluginConfigurationAndUtxos[configuration.infoHash()] = QPair<BuyerTorrentPlugin::Configuration, Coin::UnspentP2PKHOutput>(pluginConfiguration, utxo);

    return true;
}

/**
void Controller::startTorrentPlugin(const libtorrent::sha1_hash & info_hash, const TorrentPlugin::Configuration * configuration) {

    // Check that torrent exists with given info hash
    if(!_torrents.contains(info_hash)) {
        qCCritical(_category) << "No torrent registered with given info hash.";
        return;
    }

    // Grab torrent
    Torrent & torrent = _torrents[info_hash];

    Q_ASSERT(torrent->status() != Torrent::Status::being_async_added_to_session &&
            torrent->status() != Torrent::Status::torrent_added_but_not_checked);

    // Send configuration to plugin
    _plugin->submitPluginRequest(new StartTorrentPlugin(info_hash, configuration));

    // Reset event
    torrent.setEvent(Torrent::ExpectedEvent::nothing);
}
*/

void Controller::startSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPlugin::Configuration & pluginConfiguration) {

    Q_ASSERT(_torrents.contains(info_hash));

    // Grab torrent
    Torrent * torrent = _torrents[info_hash];

    Q_ASSERT(torrent->status() != Torrent::Status::being_async_added_to_session &&
            torrent->status() != Torrent::Status::torrent_added_but_not_checked);

    // Reset event
    torrent->setStatus(Torrent::Status::nothing);

    // Send configuration to plugin
    _plugin->submitPluginRequest(new StartSellerTorrentPlugin(info_hash, pluginConfiguration));
}

void Controller::startBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo) {

    Q_ASSERT(_torrents.contains(info_hash));

    // Grab torrent
    Torrent * torrent = _torrents[info_hash];

    Q_ASSERT(torrent->status() != Torrent::Status::being_async_added_to_session &&
            torrent->status() != Torrent::Status::torrent_added_but_not_checked);

    // Reset event
    torrent->setStatus(Torrent::Status::nothing);

    // Send configuration to plugin
    _plugin->submitPluginRequest(new StartBuyerTorrentPlugin(info_hash, pluginConfiguration, utxo));
}


void Controller::saveStateToFile(const char * file) {

    // NOT DONE!

    /**

	// Save present state of session in entry
	libtorrent::entry libtorrentSessionSettingsEntry;
    _session->save_state(libtorrentSessionSettingsEntry);

    // Torrent configurations
    std::vector<TorrentConfiguration *> torrentConfigurations;

    // Grab torrent handles
    std::vector<libtorrent::torrent_handle> torrents = _session->get_torrents();

    for(std::vector<libtorrent::torrent_handle>::iterator i = torrents.begin(),
            end(torrents.end()); i != end;i++) {

        // Check if torrent handle is valid, if not, we can't save it
        if((*i).is_valid()) {

            // We need to have torrent plugin configurations already available,
            // which is done asynch, perhaps make a parameter of this method
            // which is called by a callback routine.

            // Create torrent configuration
            TorrentConfiguration * torrentConfiguration = new TorrentConfiguration();

            // Add to vector of configurations
            torrentConfigurations.push_back(torrentConfiguration);

        } else
            qCCritical(_category) << "Could not recover handle for torrent for saving, skipping it.";
    }

    // DHT routers
    std::vector<std::pair<std::string, int>> dhtRouters;

    // Create controller configuration
    ControllerConfiguration controllerConfiguration(libtorrentSessionSettingsEntry
                                                    ,_portRange
                                                    ,torrentConfigurations
                                                    ,dhtRouters);

	// Save to file
    controllerConfiguration.saveToFile(file);
    */
}

Wallet::Manager * Controller::wallet() {
    return _wallet;
}

const TorrentViewModel * Controller::torrentViewModel(const libtorrent::sha1_hash & infoHash) const {

    if(!_torrents.contains(infoHash))
        return NULL;
    else
        return _torrents[infoHash]->model();
}

void Controller::begin_close() {

    // Note that controller is being stopped,
    // this prevents any processing of Qt events
    // from libtorrent or view pending in event queue.
    //stoppingController = true;

    // Pause all torrents
    _session->pause();

    /**
     * DO NOT SAVE STATE TO DISK,
     * RATHER EMIT CONFIGURATION WITH CLOSING EVENT OR SOMETHING
     */
    // Save state of controller (includes full libtorrent state) to parameter file
    //QString file = QDir::current().absolutePath () + QDir::separator() + PARAMETER_FILE_NAME;
    //saveStateToFile(file.toStdString().c_str());

    // Save resume data for all
    int numberOutStanding = makeResumeDataCallsForAllTorrents();

    // If there are no outstanding, then just close right away
    if(numberOutStanding == 0)
        finalize_close();
    else {

        // Update controller state
        _state = State::waiting_for_resume_data_while_closing;

        qCDebug(_category) << "Attempting to generate resume data for " << numberOutStanding << " torrents.";
    }
}

/**
libtorrent::torrent_handle Controller::getTorrentHandle(const libtorrent::sha1_hash & infoHash) const {

    if(_torrents.contains(infoHash))
        return _torrents[infoHash]->handle();
    else
        return libtorrent::torrent_handle();
}
*/

libtorrent::torrent_handle Controller::registerStream(Stream * stream) {

    // Get info hash of torrent requested in stream
    libtorrent::sha1_hash infoHash = stream->infoHash();

    // If no torrent exist with given info hash, just return default handle
    if(!_torrents.contains(infoHash))
        return libtorrent::torrent_handle();

     // Get torrent
     Torrent * torrent = _torrents[infoHash];

     // Add stream
     torrent->addStream(stream);

     // Get handle
     libtorrent::torrent_handle h = torrent->handle();

     Q_ASSERT(h.is_valid());

     return h;
}

void Controller::unRegisterStream(Stream * stream) {

    libtorrent::sha1_hash infoHash = stream->infoHash();

    if(_torrents.contains(infoHash))
        _torrents[infoHash]->removeStream(stream);
    else
        qCDebug(_category) << "Failed unregistering stream, torrent does not exist.";
}

void Controller::unRegisterStream(Stream * stream, Stream::Error error) {

    unRegisterStream(stream);

    qCDebug(_category) << "Stream unregistered due to some error.";

    //emit some sort of signal about error
}

void Controller::changeDownloadingLocationFromThisPiece(const libtorrent::sha1_hash & infoHash, int pieceIndex) {

    // Check that torrent exists
    if(!_torrents.contains(infoHash)) {

        qCDebug(_category) << "Changing download location requested for torrent which does not exist.";
        return;
    }

    // Check that
    if(_torrents[infoHash]->pluginInstalled() != PluginInstalled::Buyer) {

        qCDebug(_category) << "Changing download location requested for with plugin which does not have a buyer torrent plugin installed on torrent.";
        return;
    }

    // Ask torrent to relocate
    _plugin->submitTorrentPluginRequest(new ChangeDownloadLocation(infoHash, pieceIndex));
}

quint16 Controller::getServerPort() const {
   return _streamingServer.serverPort();
}

void Controller::finalize_close() {

    qCDebug(_category) << "finalize_close() run.";

    // Stop timer
    _statusUpdateTimer.stop();

    // Tell runner that controller is done
    emit closed();
}
