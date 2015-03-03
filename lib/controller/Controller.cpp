
#include "Controller.hpp"
//#include "TorrentStatus.hpp"

/**
 * Controller::Torrent::Configuration
 */

#include "Exceptions/InvalidBitSwaprStateEntryException.hpp"
#include "extension/PluginMode.hpp"

Controller::Torrent::Configuration::Configuration() {

}

Controller::Torrent::Configuration::Configuration(const libtorrent::sha1_hash & infoHash,
                                           const std::string & name,
                                           const std::string & savePath,
                                           const std::vector<char> & resumeData,
                                           quint64 flags,
                                           libtorrent::torrent_info * torrentInfo,
                                           const TorrentPlugin::Configuration * torrentPluginConfiguration)
    :_infoHash(infoHash)
    ,_name(name)
    ,_savePath(savePath)
    ,_resumeData(resumeData)
    ,_flags(flags)
    ,_torrentInfo(torrentInfo)
    ,_torrentPluginConfiguration(torrentPluginConfiguration) {
}

Controller::Torrent::Configuration::~Configuration() {

    /*
    // Delete if was set to object
    if(_torrentInfo != NULL)
        delete _torrentInfo;
    */
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
    params.resume_data = _resumeData; new std::vector<char>(_resumeData); // We do not own this pointer
    params.flags = _flags;

    if(_torrentInfo != NULL)
        params.ti = boost::intrusive_ptr<libtorrent::torrent_info>(_torrentInfo);

    //params.userdata = static_cast<void *>(_torrentPluginConfiguration);

    // Return parameters
    return params;
}

const TorrentPlugin::Configuration * Controller::Torrent::Configuration::torrentPluginConfiguration() const {
    return _torrentPluginConfiguration;
}

void Controller::Torrent::Configuration::setTorrentPluginConfiguration(const TorrentPlugin::Configuration *torrentPluginConfiguration) {
    _torrentPluginConfiguration = torrentPluginConfiguration;
}

libtorrent::torrent_info * Controller::Torrent::Configuration::torrentInfo() const {
    return _torrentInfo;
}

void Controller::Torrent::Configuration::setTorrentInfo(libtorrent::torrent_info * torrentInfo) {
    _torrentInfo = torrentInfo;
}

quint64 Controller::Torrent::Configuration::flags() const {
    return _flags;
}

void Controller::Torrent::Configuration::setFlags(const quint64 & flags) {
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

Controller::Torrent::Torrent() {

}

Controller::Torrent::Torrent(const libtorrent::sha1_hash & infoHash,
                             const std::string & name,
                             const std::string & savePath,
                            const std::vector<char> & resumeData,
                            quint64 flags,
                            libtorrent::torrent_info * torrentInfo,
                            ExpectedEvent event)
    :_infoHash(infoHash)
    ,_name(name)
    ,_savePath(savePath)
    ,_resumeData(resumeData)
    ,_flags(flags)
    ,_torrentInfo(torrentInfo)
    ,_event(event) {
}

libtorrent::torrent_info * Controller::Torrent::torrentInfo() const {
    return _torrentInfo;
}

void Controller::Torrent::setTorrentInfo(libtorrent::torrent_info *torrentInfo) {
    _torrentInfo = torrentInfo;
}

quint64 Controller::Torrent::flags() const {
    return _flags;
}

void Controller::Torrent::setFlags(const quint64 & flags) {
    _flags = flags;
}

std::vector<char> Controller::Torrent::resumeData() const {
    return _resumeData;
}

void Controller::Torrent::setResumeData(const std::vector<char> & resumeData) {
    _resumeData = resumeData;
}

std::string Controller::Torrent::savePath() const {
    return _savePath;
}

void Controller::Torrent::setSavePath(const std::string & savePath) {
    _savePath = savePath;
}

std::string Controller::Torrent::name() const {
    return _name;
}

void Controller::Torrent::setName(const std::string & name) {
    _name = name;
}

libtorrent::sha1_hash Controller::Torrent::infoHash() const {
    return _infoHash;
}

void Controller::Torrent::setInfoHash(const libtorrent::sha1_hash &infoHash){
    _infoHash = infoHash;
}

Controller::Torrent::ExpectedEvent Controller::Torrent::event() const {
    return _event;
}

void Controller::Torrent::setEvent(const ExpectedEvent & event) {
    _event = event;
}

/**
 * Controller::Configuration
 */

#include "exceptions/InvalidBitSwaprStateEntryException.hpp"
#include "Config.hpp"
#include "Utilities.hpp"

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

    /* user_agent:
    *	the client identification to the tracker. The recommended format of
    *	this string is: "ClientName/ClientVersion libtorrent/libtorrentVersion".
    *	This name will not only be used when making HTTP requests, but also when
    *	sending extended headers to peers that support that extension.
    */
    sessionSettings.user_agent = CLIENT_FINGERPRINT "/" ;// BITSWAPR_VERSION_MAJOR BITSWAPR_VERSION_MINOR, "LT/" LIBTORRENT_VERSION;

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
    sessionSettings.allow_multiple_connections_per_ip = true;

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
                                         const QVector<Torrent::Configuration> & torrents,
                                         const QString walletFile)
                                :_libtorrentSessionSettingsEntry(libtorrentSessionSettingsEntry)
                                ,_portRange(portRange)
                                ,_dhtRouters(dhtRouters)
                                ,_torrents(torrents)
                                ,_walletFile(walletFile){
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

QString Controller::Configuration::walletFile() const {
    return _walletFile;
}

void Controller::Configuration::setWalletFile(const QString & walletFile) {
    _walletFile = walletFile;
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

//#include "TorrentStatus.hpp"
#include "Config.hpp"
//#include "view/addtorrentdialog.hpp"
#include "controller/Exceptions/ListenOnException.hpp"
//#include "controller/TorrentConfiguration.hpp"
#include "extension/Alert/TorrentPluginStatusAlert.hpp"
#include "extension/Alert/PluginStatusAlert.hpp"
//#include "extension/Request/SetConfigurationTorrentPluginRequest.hpp"
//#include "extension/Request/StartPluginTorrentPluginRequest.hpp"
#include "extension/Request/StartTorrentPlugin.hpp"

#include <libtorrent/alert_types.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/peer_connection.hpp>

#include <QObject>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QtGlobal>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QThread>

#ifndef Q_MOC_RUN
#include <boost/bind.hpp>
#endif Q_MOC_RUN

// Register types for signal and slots: LATER CHECK WHICH ONE OF THESE ARE ACTUALLY REQUIRED
Q_DECLARE_METATYPE(libtorrent::sha1_hash)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(libtorrent::error_code)
Q_DECLARE_METATYPE(std::vector<libtorrent::torrent_status>)
Q_DECLARE_METATYPE(libtorrent::torrent_status)

// Register type for QMetaObject::invokeMethod
Q_DECLARE_METATYPE(const libtorrent::alert*)

Controller::Controller(const Configuration & configuration, bool showView, QNetworkAccessManager & manager, QString bitcoindAccount, QLoggingCategory & category)
    :_state(State::normal)
    ,_session(libtorrent::fingerprint(CLIENT_FINGERPRINT, BITSWAPR_VERSION_MAJOR, BITSWAPR_VERSION_MINOR, 0, 0),
              libtorrent::session::add_default_plugins,
              libtorrent::alert::error_notification +
              libtorrent::alert::tracker_notification +
              libtorrent::alert::debug_notification +
              libtorrent::alert::status_notification +
              libtorrent::alert::progress_notification +
              libtorrent::alert::performance_warning +
              libtorrent::alert::stats_notification)
    ,_wallet(configuration.walletFile(), true) // add autosave to configuration later?? does user even need to control that?
    ,_category(category)
    ,_manager(manager)
    ,_plugin(new Plugin(this, _manager, bitcoindAccount, _category))
    ,_portRange(configuration.getPortRange())
    ,_view(this, &_wallet, _category) {

    // Register types for signal and slots
    qRegisterMetaType<libtorrent::sha1_hash>();
    qRegisterMetaType<std::string>();
    qRegisterMetaType<libtorrent::error_code>();
    qRegisterMetaType<std::vector<libtorrent::torrent_status>>();
    qRegisterMetaType<libtorrent::torrent_status>();

    // Register type for QMetaObject::invokeMethod
    qRegisterMetaType<const libtorrent::alert*>();

    // Set libtorrent to call processAlert when alert is created
    _session.set_alert_dispatch(boost::bind(&Controller::libtorrent_alert_dispatcher_callback, this, _1));

	// Set session settings - these acrobatics with going back and forth seem to indicate that I may have done it incorrectly
	std::vector<char> buffer;
    libtorrent::bencode(std::back_inserter(buffer), configuration.getLibtorrentSessionSettingsEntry());
	libtorrent::lazy_entry settingsLazyEntry;
    libtorrent::error_code lazyBdecodeEc;
	libtorrent::lazy_bdecode(&buffer[0], &buffer[0] + buffer.size(), settingsLazyEntry, lazyBdecodeEc);
    _session.load_state(settingsLazyEntry);

	// Add DHT routing nodes
    const std::vector<std::pair<std::string, int>> & dhtRouters = configuration.getDhtRouters();

    for(std::vector<std::pair<std::string, int>>::const_iterator i = dhtRouters.begin(),
            end(dhtRouters.end());i != end; ++i)
        _session.add_dht_router(*i); // Add router to session

    // Add plugin extension
    boost::shared_ptr<libtorrent::plugin> plugin_shared_ptr(_plugin);
    _session.add_extension(plugin_shared_ptr);

    // Start DHT node
    _session.start_dht();

    // Start timer which calls session.post_torrent_updates at regular intervals
    _statusUpdateTimer.setInterval(POST_TORRENT_UPDATES_DELAY);

    QObject::connect(&_statusUpdateTimer,
                     SIGNAL(timeout()),
                     this,
                     SLOT(callPostTorrentUpdates()));

    _statusUpdateTimer.start();

	// Start listening
	boost::system::error_code listenOnErrorCode;
    _session.listen_on(_portRange, listenOnErrorCode);

    // Throw
	if(listenOnErrorCode)
		throw ListenOnException(listenOnErrorCode);

    // Add all torrents, but this ust be AFTER session.listen_on(),
    // because otherwise adding to session won't work.
    QVector<Torrent::Configuration> & torrents = configuration.torrents();

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

    // Show view
    if(showView) {
        _view.show();

        // Set port in title if we are logging
        if(QString(_category.categoryName()).compare("default"))
            _view.setWindowTitle(_view.windowTitle() + ", Port:" + QString::number(_session.listen_port()));
    }
}

void Controller::callPostTorrentUpdates() {
    _session.post_torrent_updates();
}

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

void Controller::libtorrent_alert_dispatcher_callback(std::auto_ptr<libtorrent::alert> alertAutoPtr) {

    /**
     * CRITICAL: Do not under any circumstance make a new call to libtorrent in this routine, since the network
     * thread in libtorrent will be making this call, and a new call will result in a dead lock.
     */

    // Grab alert pointer and release the auto pointer, this way the alert is not automatically
    // deleted when alertAutoPtr goes out of scope. Registering auto_ptr with MOC is not worth trying.
    const libtorrent::alert * a = alertAutoPtr.release();

    // Tell bitswapr thread to run processAlert later with given alert as argument
    QMetaObject::invokeMethod(this, "processAlert", Q_ARG(const libtorrent::alert*, a));
}

void Controller::processAlert(const libtorrent::alert * a) {

    // Check that alert has been stuck in event queue and corresponds to recenty
    // removed torrent.

    // if(something)
    //    something;

    //qCDebug(_category) << a->what();

    // In each case, tell bitswapr thread to run the given method
    if(libtorrent::metadata_received_alert const * p = libtorrent::alert_cast<libtorrent::metadata_received_alert>(a)) {
        //qCDebug(_category) << "Metadata received alert";
        processMetadataReceivedAlert(p);
    } else if(libtorrent::metadata_failed_alert const * p = libtorrent::alert_cast<libtorrent::metadata_failed_alert>(a)) {
        qCDebug(_category) << "Metadata failed alert";
        processMetadataFailedAlert(p);
    } else if(libtorrent::add_torrent_alert const * p = libtorrent::alert_cast<libtorrent::add_torrent_alert>(a)) {
        //qCDebug(_category) << "Add torrent alert";
        processAddTorrentAlert(p);
    } else if (libtorrent::torrent_finished_alert const * p = libtorrent::alert_cast<libtorrent::torrent_finished_alert>(a)) {
        //qCDebug(_category) << "torrent_finished_alert";
        processTorrentFinishedAlert(p);
    } else if (libtorrent::torrent_paused_alert const * p = libtorrent::alert_cast<libtorrent::torrent_paused_alert>(a)) {
        //qCDebug(_category) << "Torrent paused alert.";
        processTorrentPausedAlert(p);
    } else if (libtorrent::state_update_alert const * p = libtorrent::alert_cast<libtorrent::state_update_alert>(a)) {
        //qCDebug(_category) << "State update alert.";
        processStatusUpdateAlert(p);
    } else if(libtorrent::torrent_removed_alert const * p = libtorrent::alert_cast<libtorrent::torrent_removed_alert>(a)) {
        //qCDebug(_category) << "Torrent removed alert.";
        processTorrentRemovedAlert(p);
    } else if(libtorrent::save_resume_data_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_alert>(a)) {
        //qCDebug(_category) << "Save resume data alert.";
        processSaveResumeDataAlert(p);
    } else if(libtorrent::save_resume_data_failed_alert const * p = libtorrent::alert_cast<libtorrent::save_resume_data_failed_alert>(a)) {
        //qCDebug(_category) << "Save resume data failed alert.";
        processSaveResumeDataFailedAlert(p);
    } else if(libtorrent::torrent_checked_alert const * p = libtorrent::alert_cast<libtorrent::torrent_checked_alert>(a)) {
        //qCDebug(_category) << "Torrent checked alert.";
        processTorrentCheckedAlert(p);
    } else if(TorrentPluginStatusAlert const * p = libtorrent::alert_cast<TorrentPluginStatusAlert>(a)) {
        //qCDebug(_category) << "Torrent plugin status alert.";
        processTorrentPluginStatusAlert(p);
    } else if(PluginStatusAlert const * p = libtorrent::alert_cast<PluginStatusAlert>(a)) {
        //qCDebug(_category) << "Plugin status alert.";
        processPluginStatusAlert(p);
    }

    // Delete alert
    delete a;
}

/**
// NB!: Alreayd assumes session is paused and does not check sourceForLastResumeDataCall and numberOfOutstandingResumeDataCalls before starting,
// assumes they equal NONE,0 respectively.
*/

int Controller::makeResumeDataCallsForAllTorrents() {

    // Get all handles (should be same torrents as int addTorrentParameters, but we dont check this here
    std::vector<libtorrent::torrent_handle> handles = _session.get_torrents();

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
        Torrent & torrent = _torrents[infoHash];

        // Dont save data if
        if (torrent.event() != Torrent::ExpectedEvent::nothing // are in wrong state
            || !h.is_valid() // dont have valid handle
            || !h.need_save_resume_data() // dont need to
            || !h.status().has_metadata) // or dont have metadata
            continue;

        // Save resume data
        h.save_resume_data();

        // Count call
        resumeCallsMade++;

        // Change expected event of torrent
        _torrents[infoHash].setEvent(Torrent::ExpectedEvent::save_resume_data_alert);
    }

    return resumeCallsMade;
}

void Controller::processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p) {

    // Save resume data if we can!
    //if(_sourceForLastResumeDataCall == NONE) {

        // Check that we have a valid state
        //if(_numberOfOutstandingResumeDataCalls != 0) {
//
//            qCCritical(_category) << "Invalid state, sourceForLastResumeDataCall == NONE && numberOfOutstandingResumeDataCalls != 0, can't save resume data.";
//            return;
//        }

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

        _torrents[info_hash].setEvent(Torrent::ExpectedEvent::save_resume_data_alert);

    //} else
    //    qCWarning(_category) << "Could not save resume data, so we won't.";
}

void Controller::processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p) {

    /*
     * NOTICE: Docs say p->handle may be invalid at this time,
     * so we must use p->info_hash instead.
     */

    // Get torrent info hash
    libtorrent::sha1_hash info_hash = p->info_hash;

    // Remove from view
    _view.removeTorrent(info_hash);

    qCDebug(_category) << "Found match and removed it.";
}

void Controller::processMetadataReceivedAlert(libtorrent::metadata_received_alert const * p) {

    // Get handle for torrent
    libtorrent::torrent_handle h = p->handle;

    // Process if handle is valid
    if (h.is_valid()) {

        // get torrent info
        boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();

        //const libtorrent::torrent_info & torrentInfo = h.get_torrent_info();

        // USE THIS INFORMATION FOR SOMETHING LATER

    } else
        qCDebug(_category) << "Invalid handle for received metadata.";
}

void Controller::processMetadataFailedAlert(libtorrent::metadata_failed_alert const * p) {
    // WHAT DO WE DO HERE?
}

void Controller::processAddTorrentAlert(libtorrent::add_torrent_alert const * p) {

    // Name of torrent
    std::string name("N/A");
    int totalSize = 0;
    if(p->params.ti.get() != 0) {
        name = p->params.ti->name();
        totalSize = (p->params.ti)->total_size();
    } else
        name = p->params.name;

    // Check if there was an error
    if (p->error) {
        _view.addTorrentFailed(name, p->params.info_hash, p->error);
    } else {

        /*
		h.set_max_connections(max_connections_per_torrent);
		h.set_max_uploads(-1);
		h.set_upload_limit(torrent_upload_limit);
		h.set_download_limit(torrent_download_limit);
		h.use_interface(outgoing_interface.c_str());
        */

        // Add torrent to view
        _view.addTorrent(p->handle.info_hash(), QString(name.c_str()), totalSize);
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

    //qCCritical(_category) << "Number of state_update_alert alerts" << p->status.size();
    _view.updateTorrentStatus(p->status);
}

void Controller::processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p) {

    // Get info hash for torrent
    libtorrent::sha1_hash info_hash = p->handle.info_hash();

    // Grab torrent
    Q_ASSERT(_torrents.contains(info_hash));
    Torrent & torrent = _torrents[info_hash];

    // Check that alert was expected
    Q_ASSERT(torrent.event() == Torrent::ExpectedEvent::save_resume_data_alert);

    // Reset expected event
    torrent.setEvent(Torrent::ExpectedEvent::nothing);

    // Create resume data buffer
    std::vector<char> resumeData;

    // Write new content to it
    bencode(std::back_inserter(resumeData), *(p->resume_data));

    // Save resume data in torrent
    torrent.setResumeData(resumeData);

    // If this is part of closing client, then
    if(_state == State::waiting_for_resume_data_while_closing) {

        // Check if all there are any pending resume data requests
        for(QMap<libtorrent::sha1_hash, Torrent>::const_iterator i = _torrents.begin(),
                end(_torrents.end()); i != end;i++) {

            // End processing if alert is expected
            if(i.value().event() == Torrent::ExpectedEvent::save_resume_data_alert)
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
        libtorrent::sha1_hash info_hash = h.info_hash();

        // Make sure the torrent exists
        Q_ASSERT(_torrents.contains(info_hash));

        // Grab torrent
        Torrent & torrent = _torrents[info_hash];

        // Assert that torrent_checked_alert was expected
        Q_ASSERT(torrent.event() == Torrent::ExpectedEvent::torrent_checked_alert);

        // if a configuration was saved, then we just use it
        if(_pendingConfigurations.contains(info_hash)) {

            // Remove torrent plugin configuration
            const TorrentPlugin::Configuration * configuration = _pendingConfigurations.take(info_hash);

            // Send configuration to plugin
            _plugin->submitPluginRequest(new StartTorrentPlugin(info_hash, configuration));

            // Reset event
            torrent.setEvent(Torrent::ExpectedEvent::nothing);

        } else { // otherwise prompt user

            // Get torrent information
            boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfoIntrusivePtr = h.torrent_file();
            const libtorrent::torrent_info * torrentInfoPtr = torrentInfoIntrusivePtr.get();
            libtorrent::torrent_info torrentInfo = *torrentInfoPtr; //h.get_torrent_info();

            // Get torrent status
            libtorrent::torrent_status torrentStatus = h.status();

            // Expect user to set configurations
            torrent.setEvent(Torrent::ExpectedEvent::torrent_plugin_configuration_from_user);

            /**
             * LATER THIS SHOULD BE ASYNC.
             * and tell view to prompt user based on this
            */
            _view.showAddTorrentPluginConfigurationDialog(torrentInfo, torrentStatus);
        }

    } else
        qCDebug(_category) << "Invalid handle for checked torrent.";
}

void Controller::processTorrentPluginStatusAlert(const TorrentPluginStatusAlert * p) {
    _view.updateTorrentPluginStatus(p);
}

void Controller::processPluginStatusAlert(const PluginStatusAlert * p) {
    _view.updatePluginStatus(p);
}

bool Controller::removeTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle & torrentHandle = _session.find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        return false;

    // Remove from session
    // Session will send us torrent_removed_alert alert when torrent has been removed
    // at which point we can remove torrent from model in alert handler
    _session.remove_torrent(torrentHandle);

    // It worked
    return true;
}

bool Controller::pauseTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle & torrentHandle = _session.find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        return false;

    // Turn off auto managing
    torrentHandle.auto_managed(false);

    // Pause
    // We save resume data when the pause alert is issued by libtorrent
    torrentHandle.pause(libtorrent::torrent_handle::graceful_pause);

    // It worked
    return true;
}

bool Controller::startTorrent(const libtorrent::sha1_hash & info_hash) {

    // Find corresponding torrent
    libtorrent::torrent_handle & torrentHandle = _session.find_torrent(info_hash);

    // Check that there actually was such a torrent
    if(!torrentHandle.is_valid())
        return false;

    // Turn on auto managing
    torrentHandle.auto_managed(true);

    // Start
    torrentHandle.resume();

    // It worked
    return true;
}

bool Controller::addTorrent(const Torrent::Configuration & configuration, bool promptUserForTorrentPluginConfiguration) {

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
    Torrent torrent(info_hash,
                    configuration.name(),
                    configuration.name(),
                    configuration.resumeData(),
                    configuration.flags(),
                    configuration.torrentInfo(),
                    Torrent::ExpectedEvent::torrent_checked_alert);

    // Warn user if torrent has already been added
    if(_torrents.contains(info_hash)) {

        qCCritical(_category) << "Torrent with given info_hash has already been added.";
        return false;
    }

     // Save torrent
     _torrents[info_hash] = torrent;

     // If we dont prompt user for plugin configuration,
     // then we save the one given in configuration
     if(!promptUserForTorrentPluginConfiguration) {

         Q_ASSERT(!_pendingConfigurations.contains(info_hash));

         // Save plugin configuration
         _pendingConfigurations[info_hash] = configuration.torrentPluginConfiguration();
     }

    // Add torrent to session
    _session.async_add_torrent(params);

    // Indicate that we added to session
    return true;
}

void Controller::startTorrentPlugin(const libtorrent::sha1_hash & info_hash, const TorrentPlugin::Configuration * configuration) {

    // Check that torrent exists with given info hash
    if(!_torrents.contains(info_hash)) {
        qCCritical(_category) << "No torrent registered with given info hash.";
        return;
    }

    // Grab torrent
    Torrent & torrent = _torrents[info_hash];

    // It must always have given event expected
    Q_ASSERT(torrent.event() == Torrent::ExpectedEvent::torrent_plugin_configuration_from_user);

    // Send configuration to plugin
    _plugin->submitPluginRequest(new StartTorrentPlugin(info_hash, configuration));

    // Reset event
    torrent.setEvent(Torrent::ExpectedEvent::nothing);
}

void Controller::saveStateToFile(const char * file) {

    // NOT DONE!

    /**

	// Save present state of session in entry
	libtorrent::entry libtorrentSessionSettingsEntry;
    _session.save_state(libtorrentSessionSettingsEntry);

    // Torrent configurations
    std::vector<TorrentConfiguration *> torrentConfigurations;

    // Grab torrent handles
    std::vector<libtorrent::torrent_handle> torrents = _session.get_torrents();

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

void Controller::begin_close() {

    // Note that controller is being stopped,
    // this prevents any processing of Qt events
    // from libtorrent or view pending in event queue.
    //stoppingController = true;

    // Pause all torrents
    _session.pause();

    // Save state of controller (includes full libtorrent state) to parameter file
    QString file = QDir::current().absolutePath () + QDir::separator() + PARAMETER_FILE_NAME;
    saveStateToFile(file.toStdString().c_str());

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

void Controller::finalize_close() {

    qCDebug(_category) << "finalize_close() run.";

    // Stop timer
    _statusUpdateTimer.stop();

    // Tell runner that controller is done
    emit closed();
}
