
#include "ControllerState.hpp"
#include "Exceptions/InvalidBitSwaprStateEntryException.hpp"

#include <libtorrent/bencode.hpp>

#include <fstream>

//const std::string ControllerState::DEFAULT_FILE_NAME = "state.txt";

ControllerState::ControllerState(const libtorrent::entry & libtorrentSessionSettingsEntry_,
								const std::pair<int, int> & portRange_,
								const std::vector<libtorrent::add_torrent_params> & addTorrentParameters_,
								const std::vector<std::pair<std::string, int>> & dhtRouters_) 
								: libtorrentSessionSettingsEntry(libtorrentSessionSettingsEntry_)
								, portRange(portRange_)
								, addTorrentParameters(addTorrentParameters_)
                                , dhtRouters(dhtRouters_) {}

ControllerState::ControllerState() 
				: libtorrentSessionSettingsEntry()
				, portRange()
				, addTorrentParameters()
                , dhtRouters() {

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
	sessionSettings.user_agent = "BR/" LIBTORRENT_VERSION;

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
	dhtSettings.privacy_lookups = true;

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
	dummySession.save_state(libtorrentSessionSettingsEntry);

	// Set port range
	portRange = std::make_pair(6881, 6889);

	// Set dht routers
	dhtRouters.push_back(std::make_pair(std::string("router.bittorrent.com"), 6881));
	dhtRouters.push_back(std::make_pair(std::string("router.utorrent.com"), 6881));
	dhtRouters.push_back(std::make_pair(std::string("router.bitcomet.com"), 6881));
}

ControllerState::ControllerState(const char * fileName) {

	// Open file at the end, so we can get size
	std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ifstream::ate);

	// Get size of file
	int sizeOfFile = (int)file.tellg();

	// Rewind position
	file.seekg(0);

	// Allocate space for bencoded state entry
	std::vector<char> bencodedBitSwaprStateEntry(sizeOfFile, 0);

	// Read entire file into vector
	file.read(&bencodedBitSwaprStateEntry[0], bencodedBitSwaprStateEntry.size());
	
	// Close file
	file.close();

	/* 
	// Check that entire file was read
	if(!file)
		throw something.., only file.gcount() written
	*/

	// Bendecode entry
	libtorrent::entry bitSwaprStateEntry = libtorrent::bdecode(bencodedBitSwaprStateEntry.begin(), bencodedBitSwaprStateEntry.end());
	const libtorrent::entry::dictionary_type & bitSwaprStateDictionaryEntry = bitSwaprStateEntry.dict();

	// Load using this dictionary
	loadFromDictionaryEntry(bitSwaprStateDictionaryEntry);
}

void ControllerState::loadFromDictionaryEntry(const libtorrent::entry::dictionary_type & bitSwaprStateDictionaryEntry) {

	// Check that libtorrentSettings key is present, and then parse
    if(bitSwaprStateDictionaryEntry.count("libtorrentSettings") == 1)
		libtorrentSessionSettingsEntry = bitSwaprStateDictionaryEntry.find("libtorrentSettings")->second;
	else
		throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "There is not exactly one libtorrentSettings key.");

	// Check that portRange key is present, and then parse
    if(bitSwaprStateDictionaryEntry.count("portRange") == 1) {

        const libtorrent::entry & portRangeEntry = bitSwaprStateDictionaryEntry.find("portRange")->second;

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
					portRange = std::make_pair(static_cast<int>(firstInteger), static_cast<int>(secondInteger));

				} else
					throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "elements in portRange entry are not of type entry::int_type.");
			} else
				throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "portRange entry is not a list of length 2.");
		} else
			throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "portRange key is not of type entry::list_type."); 
	} else
		throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "libtorrentSettings key should have .count == 1.");
	
	// Check that addTorrentParameters key is present, and then parse
    if(bitSwaprStateDictionaryEntry.count("addTorrentParameters") == 1) {

		const libtorrent::entry & addTorrentParametersEntry = bitSwaprStateDictionaryEntry.find("addTorrentParameters")->second;

		// Check that entry is of type entry::list_typ
		if(addTorrentParametersEntry.type() == libtorrent::entry::data_type::list_t) {

			// Use as list
			const libtorrent::entry::list_type & addTorrentParametersListEntry = addTorrentParametersEntry.list();

			// Iterate list and parse add_torrent_params structs
			for(libtorrent::entry::list_type::const_iterator i = addTorrentParametersListEntry.begin(),
					end(addTorrentParametersListEntry.end()); i != end;i++) {

				// Get list entry element
				const libtorrent::entry & addTorrentParametersListElementEntry = *i;

				// Check that it is a dictionary
				if(addTorrentParametersListElementEntry.type() == libtorrent::entry::data_type::dictionary_t) {

					// Use as dictionary
					const libtorrent::entry::dictionary_type & addTorrentParametersDictionaryEntry = addTorrentParametersListElementEntry.dict();

					// Convert to add_torrent_params struct
					libtorrent::add_torrent_params addTorrentParametersStruct;
					dictionaryEntryToAddTorrentParams(addTorrentParametersStruct, addTorrentParametersDictionaryEntry);

					// Add to torrentAddTorrentParameters
					addTorrentParameters.push_back(addTorrentParametersStruct);

				} else
					throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "addTorrentParameters list element entry is not of type entry::dict_type.");
			}

		} else
			throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "addTorrentParameters entry is not of type entry::list_type.");
	} else
		throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "addTorrentParameters key should have .count == 1.");

	// Check that dhtRouters key is present, and then parse
    if(bitSwaprStateDictionaryEntry.count("dhtRouters") == 1) {

		const libtorrent::entry & dhtRoutersEntry = bitSwaprStateDictionaryEntry.find("dhtRouters")->second;

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
							dhtRouters.push_back(std::make_pair(hostNameStringEntry.c_str(), (int)portIntegerEntry));

						} else
							throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "elements in dhtRouters entry are not of type <entry::string_type, entry::int_type>.");
					} else
						throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "dhtRouters entry is not a list of length 2.");
				} else
					throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "dhtRouters list element entry is not of type entry::list_type.");
			}

		} else
			throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "dhtRouters entry is not of type entry::list_type.");
	} else
		throw InvalidBitSwaprStateEntryException(bitSwaprStateDictionaryEntry, "dhtRouters key should have .count == 1.");
}

void ControllerState::saveToDictionaryEntry(libtorrent::entry::dictionary_type & bitSwaprStateDictionaryEntry) {
	
	// Add "libtorrentSettings" key
    bitSwaprStateDictionaryEntry["libtorrentSettings"] = libtorrentSessionSettingsEntry;

	// Add "portRange" key
	libtorrent::entry::list_type portRangeListEntry;
    portRangeListEntry.push_back(portRange.first);
    portRangeListEntry.push_back(portRange.second);

    bitSwaprStateDictionaryEntry["portRange"] = portRangeListEntry;

	// Add "addtorrentParameters" key
	libtorrent::entry::list_type addtorrentParametersListEntry;

	// Populate list entry
	for(std::vector<libtorrent::add_torrent_params>::const_iterator i = addTorrentParameters.begin(),
		end(addTorrentParameters.end()); i != end; i++) {

        // Convert params for this torrent to dictonary entry
        libtorrent::entry::dictionary_type d = addTorrentParamsToDictionaryEntry(*i);

		// Add to list entry
        addtorrentParametersListEntry.push_back(d);
	}

	// Add list entry to map
    bitSwaprStateDictionaryEntry["addTorrentParameters"] = addtorrentParametersListEntry;

	// Add "dhtRouters" key
	libtorrent::entry::list_type dhtRoutersListEntry;

	for(std::vector<std::pair<std::string, int>>::const_iterator i = dhtRouters.begin(),
		end(dhtRouters.end());i != end; i++) {

		// Create entry list for dht pair
		libtorrent::entry::list_type routerEntry;

        routerEntry.push_back((*i).first);
        routerEntry.push_back((*i).second);

		// Add to list entry
		dhtRoutersListEntry.push_back(routerEntry);
	}

	// Add list entry to map
    bitSwaprStateDictionaryEntry["dhtRouters"] = dhtRoutersListEntry;
}

void ControllerState::saveToFile(const char * fileName) {

	// Save state to entry
	libtorrent::entry::dictionary_type BitSwaprStateEntry;

	saveToDictionaryEntry(BitSwaprStateEntry);

	// Bencode entry
	std::vector<char> bencodedBitSwaprStateEntry;
	bencode(std::back_inserter(bencodedBitSwaprStateEntry), BitSwaprStateEntry);

	// Open file, overwrite if present
	std::ofstream file(fileName, std::ios::out | std::ios::binary | std::ios::trunc);

	// Save to file
	file.write(reinterpret_cast<const char*>(&bencodedBitSwaprStateEntry[0]), bencodedBitSwaprStateEntry.size());

	/* 
	// Check that entire file was written
	if(!file)
		throw something.., only file.gcount() written
	*/

	// Close file
	file.close();
}

libtorrent::entry ControllerState::getLibtorrentSessionSettingsEntry() const {
	return libtorrentSessionSettingsEntry;
}

std::pair<int, int> ControllerState::getPortRange() const {
	return portRange;
}

std::vector<libtorrent::add_torrent_params> ControllerState::getTorrentParameters() const {
	return addTorrentParameters;
}

std::vector<std::pair<std::string, int>> ControllerState::getDhtRouters() const {
	return dhtRouters;
}

libtorrent::entry::dictionary_type ControllerState::addTorrentParamsToDictionaryEntry(const libtorrent::add_torrent_params & addTorrentParams) {

    // Entry to return copy of
    libtorrent::entry::dictionary_type dictionaryEntry;

	// info_hash
    dictionaryEntry["info_hash"] = libtorrent::entry::string_type(addTorrentParams.info_hash.to_string());

	// name
    dictionaryEntry["name"] = libtorrent::entry::string_type(addTorrentParams.name);

	// save_path
    dictionaryEntry["save_path"] = libtorrent::entry::string_type(addTorrentParams.save_path);

	// resume_data
    /*
	const std::vector<char> & resume_data = addTorrentParams.resume_data;

	std::string resume_data_string;
	for(std::vector<char>::const_iterator i = resume_data.begin(),
		end(resume_data.end());i != end;i++)
			resume_data_string.append(&(*i));

    dictionaryEntry["resume_data"] = libtorrent::entry::string_type(resume_data_string);
    */

	// flags
    dictionaryEntry["flags"] = libtorrent::entry::integer_type(addTorrentParams.flags);

    // Return result
    return dictionaryEntry;
}

void ControllerState::dictionaryEntryToAddTorrentParams(libtorrent::add_torrent_params & addTorrentParams, const libtorrent::entry::dictionary_type & dictionaryEntry) {

	// Check that info_hash is present
	if(dictionaryEntry.count("info_hash") == 1) {

		// Get entry
		libtorrent::entry infoHashEntry = dictionaryEntry.find("info_hash")->second;

		// Check that entry is of type entry::string_type
		if(infoHashEntry.type() == libtorrent::entry::data_type::string_t) {

			// Use as entry::string_type
			libtorrent::entry::string_type infoHashStringEntry = infoHashEntry.string();

			// set .info_hash
			libtorrent::sha1_hash infoHash(infoHashStringEntry);
			addTorrentParams.info_hash = infoHash;

		} else
			throw InvalidBitSwaprStateEntryException(dictionaryEntry, "info_hash key is not of type entry::string_type."); 

	} else
		throw InvalidBitSwaprStateEntryException(dictionaryEntry, "info_hash key should have .count == 1.");

	// Check that name is present
	if(dictionaryEntry.count("name") == 1) {

		// Get entry
		libtorrent::entry nameEntry = dictionaryEntry.find("name")->second;

		// Check that entry is of type entry::string_type
		if(nameEntry.type() == libtorrent::entry::data_type::string_t) {

			// Use as entry::string_type
			libtorrent::entry::string_type nameStringEntry = nameEntry.string();

			// set name
			addTorrentParams.name = nameStringEntry;

		} else
			throw InvalidBitSwaprStateEntryException(dictionaryEntry, "name key is not of type entry::string_type.");
	}
	else
		throw InvalidBitSwaprStateEntryException(dictionaryEntry, "name key should have .count == 1.");

	// Check that save_path is present
	if(dictionaryEntry.count("save_path") == 1) {

		// Get entry
		libtorrent::entry savePathEntry = dictionaryEntry.find("save_path")->second;

		// Check that entry is of type entry::string_type
		if(savePathEntry.type() == libtorrent::entry::data_type::string_t) {

			// Use as entry::string_type
			libtorrent::entry::string_type savePathStringEntry = savePathEntry.string();

			// set name
			addTorrentParams.save_path = savePathStringEntry;

		} else
			throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key is not of type entry::string_type.");
	}
	else
		throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key should have .count == 1.");

    /*
	// Check that resume_data is present
	if(dictionaryEntry.count("resume_data") == 1) {

		// Get entry
		libtorrent::entry resumeDataEntry = dictionaryEntry.find("resume_data")->second;

		// Check that entry is of type entry::string_type
		if(resumeDataEntry.type() == libtorrent::entry::data_type::string_t) {

			// Use as entry::string_type
			libtorrent::entry::string_type resumeDataStringEntry = resumeDataEntry.string();

			// set resume_data by iterating string and appending
			addTorrentParams.resume_data = std::vector<char>();

			for(std::string::const_iterator i = resumeDataStringEntry.begin(),
					end(resumeDataStringEntry.end());i != end;i++)
					addTorrentParams.resume_data.push_back(*i);

		} else
			throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key is not of type entry::string_type.");
	}
	else
		throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key should have .count == 1.");
    */

	// Check that flags is present
	if(dictionaryEntry.count("flags") == 1) {

		// Get entry
		libtorrent::entry flagsEntry = dictionaryEntry.find("flags")->second;

		// Check that entry is of type entry::integer_type
		if(flagsEntry.type() == libtorrent::entry::data_type::int_t) {

			// Use as entry::integer_type
			libtorrent::entry::integer_type flagsIntegerEntry = flagsEntry.integer();

			// set .flags
			addTorrentParams.flags = flagsIntegerEntry;

		} else
			throw InvalidBitSwaprStateEntryException(dictionaryEntry, "flags key is not of type entry::integer_type.");
	} else
		throw InvalidBitSwaprStateEntryException(dictionaryEntry, "flags key should have .count == 1.");		
}
